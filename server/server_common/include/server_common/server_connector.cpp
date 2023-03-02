#include "server_connector.hpp"

#include "common/json.hpp"
#include "common/order.hpp"
#include "common/client.hpp"

#include "clients.hpp"
#include "timer.hpp"

#include <string>

namespace {

std::string_view new_client(nlohmann::json data) {
    // check valid data on regex
    std::string_view name = data["name"];
    std::string_view hash_password = data["has_password"];
    auto client = common::InfoClient{common::Client{name, hash_password, 0, true}};
    server::MapClients::add(std::move(client));
    return "New Client created";
}

std::string_view new_order(nlohmann::json data) {
    // check valid data on regex
    std::string_view instrument = "my_exchange:USD/RUB";
    std::string_view hash_client = data["hash_client"];
    std::string_view order_type = data["order_type"];
    std::string_view exchange_type = data["exchange_type"];
    int64_t timestamp_user = data["timestamp_user"];
    int64_t timestamp_exchange = Timer::now();
    double price = data["price"];
    double volume = data["volume"];
    bool is_limit = false;
    auto order = common::InfoOrder{
        common::Order{
            instrument, hash_password, 
            common::string2OrderType(order_type), 
            common::string2ExchangeType(exchange_type), 
            timestamp_user, timestamp_exchange, volume, price
    }};

}

std::string_view return_hello() {
    return "Hello, world!";
    
}

}

namespace server {


void Session::start() {
    boost::asio::async_read(socket_, boost::asio::buffer(data_, max_length),
                boost::bind(&Session::read_msg, this,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));
}

void Session::stop() {
    socket_.close();
    delete this;
}

Session::~Session() {
    socket_.close()
    delete socket_;
    delete[] data_;
    delete endpoint;
}

void Session::read_msg(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        data_[bytes_transferred] = '\0';

        auto j = nlohmann::json::parse(data_);
        auto reqType = j["ReqType"];
        std::string_view reply = parse_event(j);
        boost::asio::async_write(socket_,
                boost::asio::buffer(reply, reply.size()),
                boost::bind(&session::write_msg, this,
                    boost::asio::placeholders::error));
    } else {
        delete this;
    }
}

void Session::send_msg(const boost::system::error_code& error) const {
    if (!error) {
        socket_.async_read_some(boost::asio::buffer(data_, max_length),
            boost::bind(&session::read_msg, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    } else {
        delete this;
    }
}

void Session::parse_event(nlohmann::json data) {
    std::string_view reply;
    switch (data["ReqType"]) {
        case Requests::Registration:
            reply = new_client(data);
            break;
        case Requests::Transaction:
            reply = new_order(data);
            break;
        case Requests::Close:
            reply = stop(data);
            break;
        default:
            reply = return_hello();
            break;
    }
}

boost::asio::ip::tcp::socket& Session::socket() {
    return socket_;
}



Server::Server(const std::string_view& uri, const std::string_view& port) {
    io_service_ = boost::asio::io_service{};

    std::cout << "Server started! Listen " << port << " port" << std::endl;
    
    //acceptor_ = boost::asio::ip::tcp::acceptor(io_service_, tcp::endpoint(tcp::v4(), port))
    //income_main_requests_ = std::make_shared<Session>(io_service);
    //income_main_requests_->start();
    /*acceptor_.async_accept(income_main_requests_->socket(),
            boost::bind(&server::handle_accept, this, income_main_requests_,
                boost::asio::placeholders::error));*/
    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve({address, port});
    acceptor_.open(endpoint.protocol());
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    acceptor_.bind(endpoint);
    acceptor_.listen();

    do_accept();
};

void server::run() {
  io_service_.run();
}

void server::do_accept()
{
  acceptor_.async_accept(socket_,
      [this](boost::system::error_code ec)
      {
        // Check whether the server was stopped by a signal before this
        // completion handler had a chance to run.
        if (!acceptor_.is_open())
        {
          return;
        }

        if (!ec)
        {
            /*connection_manager_.start(std::make_shared<connection>(
                std::move(socket_), connection_manager_, request_handler_));*/

            auto client_connection = new Session(io_service_);
            client_connection->start();
            while (client_connection->getHashClient() == -1);
            clients_sessions_[client_connection->getHashClient()] = client_connection;
        }

        do_accept();
      });
}

/*void handle_accept(const boost::system::error_code& error)
{
    if (!error)
    {
        auto client_connection = new Session(io_service_);
        client_connection->start();
        while (client_connection->getHashClient() == -1);
        clients_sessions_[client_connection->getHashClient()] = client_connection;
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&server::handle_accept, this,
                boost::asio::placeholders::error));
    }
    else
    {
        delete new_session;
    }
}

void handle_read(const boost::system::error_code& error)
{
    if (!error)
    {
        //new_session->start();
        //new_session = new session(io_service_);
        acceptor_.async_accept(new_session->socket(),
            boost::bind(&server::handle_accept, this, new_session,
                boost::asio::placeholders::error));
    }
    else
    {
        delete new_session;
    }
}*/

}