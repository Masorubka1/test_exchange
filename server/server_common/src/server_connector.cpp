#include "server_common/server_connector.hpp"
#include <cppkafka/producer.h>


namespace {

std::string new_client(nlohmann::json& msg, cppkafka::Producer* const prod_) {
    nlohmann::json data = msg["Message"].get<nlohmann::json>();
    data["balance"] = 0;
    data["is_valid"] = true;
    /*std::string name = data["name"].get<std::string>();
    std::string hash_password = data["hash_password"].get<std::string>();*/
    auto client = data.get<client::Client>();
    //auto client = client::InfoClient{client::Client{name, hash_password, 0, true}};
    int hash = client::hash(&client);
    prod_->produce(cppkafka::MessageBuilder("UserEvents").key(hash).payload(data.dump()));
    //server_common::MapClients::inst().add(std::move(client));
    return "New user: " + std::to_string(hash);
}

std::string new_order(nlohmann::json& msg, cppkafka::Producer* const prod_) {
    // check valid data on regex
    nlohmann::json data = msg["Message"].get<nlohmann::json>();
    data["instrument"] = "my_exchange:USD/RUB";
    //std::string instrument = "my_exchange:USD/RUB";
    //std::string hash_client = data["hash_client"].get<std::string>();
    //std::string order_type = data["order_type"].get<std::string>();
    //std::string exchange_type = data["exchange_type"].get<std::string>();
    //int64_t timestamp_user = data["timestamp_user"].get<int64_t>();
    data["timestamp_exchange"] = server_common::Timer::now();
    //int64_t timestamp_exchange = server_common::Timer::now();
    //double price = data["price"].get<double>();
    //double volume = data["volume"].get<double>();
    data["is_limit"] = false;
    //bool is_limit = false;
    /*common::Order tmp = common::Order(
            instrument, hash_client, 
            common::string2OrderType(order_type),
            common::OrderStatus::GOT, 
            common::string2ExchangeType(exchange_type), 
            timestamp_user, timestamp_exchange, volume, price, is_limit
    );*/
    auto order = data.get<common::Order>();
    //auto order = common::InfoOrder(std::move(tmp));
    //machine::statusMachine::inst().add(order);
    prod_->produce(cppkafka::MessageBuilder("OrderEvents").key(data["hash_client"].get<std::string>()).payload(data.dump()));
    return "New Order";
}

std::string return_hello() {
    return "Hello, world!";
}

}


namespace server_common {

Session::Session(boost::asio::io_service& io_service, cppkafka::Producer* const prod) : socket_(io_service), prod_(prod) {}


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
        std::string_view reply = parse_event(j);
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
        reply = new_client(data, prod_);
        hash_client_ = reply;
    } else if (req_type == Requests::Transaction) {
        reply = new_order(data, prod_);
    } else if (req_type == Requests::Close) {
        stop();
        reply = "Stop";
    } else {
        reply = return_hello();
    }
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

    Session* new_session = new Session(io_service_, prod_);
    prod_ = new cppkafka::Producer(conf::config);
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
        new_session = new Session(io_service_, prod_);
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