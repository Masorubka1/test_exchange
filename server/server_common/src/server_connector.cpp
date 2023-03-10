#include "server_common/server_connector.hpp"


namespace {

std::string new_client(nlohmann::json& msg) {
    nlohmann::json data = msg["Message"].get<nlohmann::json>();
    data["balance"] = 0;
    data["is_valid"] = true;

    auto client = data.get<client::Client>();
    std::string hash = std::to_string(client::hash(&client));
    std::string json_dump = data.dump();
    std::cout << "New user: " << hash << " json: " << json_dump << "\n";
    static cppkafka::Producer producer(conf::config_user);
    producer.produce(cppkafka::MessageBuilder("UserEvents").key("Add").payload(json_dump));
    return "New user: " + hash;
}

std::string new_order(nlohmann::json& msg) {
    // check valid data on regex
    nlohmann::json data = msg["Message"].get<nlohmann::json>();
    data["instrument"] = "my_exchange:USD/RUB";
    data["timestamp_exchange"] = server_common::Timer::now();
    data["is_limit"] = false;
    auto order = data.get<common::Order>();
    auto json_dump = data.dump();
    static cppkafka::Producer producer(conf::config_order);
    producer.produce(cppkafka::MessageBuilder("OrderEvents").partition(0).key("Add").payload(json_dump));
    producer.produce(cppkafka::MessageBuilder("OrderEvents").partition(1).key("Add").payload(json_dump));
    return "New Order";
}

std::string return_hello() {
    return "Hello, world!";
}

}


namespace server_common {

Session::Session(boost::asio::io_service& io_service) : socket_(io_service) {}


void Session::start() {
    socket_.async_read_some(boost::asio::buffer(data_, max_length),
            boost::bind(&Session::read_msg, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
}

void Session::stop() {
    socket_.close();
    delete this;
}

Session::~Session() {
    socket_.close();
}

void Session::read_msg(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        data_[bytes_transferred] = '\0';

        auto j = nlohmann::json::parse(data_);
        auto reqType = j["ReqType"].get<std::string>();
        std::string reply = parse_event(j);
        boost::asio::async_write(socket_,
                boost::asio::buffer(reply, reply.size()),
                boost::bind(&Session::send_msg, this,
                    boost::asio::placeholders::error));
    } else {
        delete this;
    }
}

void Session::send_msg(const boost::system::error_code& error) {
    if (!error) {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            boost::bind(&Session::read_msg, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    } else {
        delete this;
    }
}

std::string Session::parse_event(nlohmann::json data) {
    std::string reply;
    std::string req_type = data["ReqType"].get<std::string>();
    if (req_type == Requests::Registration) {
        reply = new_client(data);
        hash_client_ = reply;
    } else if (req_type == Requests::Transaction) {
        reply = new_order(data);
    } else if (req_type == Requests::Close) {
        stop();
        reply = "Stop";
    } else {
        reply = return_hello();
    }
    std::cout << "reply: " << reply << "\n";
    return reply;
}

boost::asio::ip::tcp::socket& Session::socket() {
    return socket_;
}



Server::Server(boost::asio::io_service& io_service, const std::string& uri, const size_t port) : io_service_(io_service), acceptor_(io_service_) {
    std::cout << "Server started! Listen " << port << " port" << std::endl;

    boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), port);
    acceptor_ = boost::asio::ip::tcp::acceptor(io_service_, endpoint);
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));

    Session* new_session = new Session(io_service_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&Server::handle_accept, this, new_session,
        boost::asio::placeholders::error));
};

Server::~Server() {
    /*for (std::map<std::string, Session*>::iterator it = clients_sessions_.begin(); it != clients_sessions_.end(); ++it) {
        delete it->second;
    }*/
}

void Server::handle_accept(Session* new_session,
        const boost::system::error_code& error)
{
    if (!error)
    {
        new_session->start();
        new_session = new Session(io_service_);
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&Server::handle_accept, this, new_session,
                boost::asio::placeholders::error));
    }
    else
    {
        delete new_session;
    }
}

}