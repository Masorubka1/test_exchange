#include "server_common/server_connector.hpp"

/*#include "server_common/clients.hpp"
#include "server_common/timer.hpp"
#include "common/order.hpp"
//#include "common/json.hpp"
//#include "common/client.hpp"
#include "common/Common.hpp"


#include <string>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>*/

namespace {

std::string_view new_client(nlohmann::json data) {
    // check valid data on regex
    std::string name = data["name"];
    std::string hash_password = data["has_password"];
    auto client = client::InfoClient{client::Client{name, hash_password, 0, true}};
    server::MapClients map_clients = server::MapClients::inst();
    map_clients.add(std::move(client));
    return "New Client created";
}

std::string_view new_order(nlohmann::json data) {
    // check valid data on regex
    std::string instrument = "my_exchange:USD/RUB";
    std::string hash_client = data["hash_client"];
    std::string order_type = data["order_type"];
    std::string exchange_type = data["exchange_type"];
    int64_t timestamp_user = data["timestamp_user"];
    int64_t timestamp_exchange = server::Timer::now();
    double price = data["price"];
    double volume = data["volume"];
    bool is_limit = false;
    common::Order tmp = common::Order(
            instrument, hash_client, 
            common::string2OrderType(order_type),
            common::OrderStatus::GOT, 
            common::string2ExchangeType(exchange_type), 
            timestamp_user, timestamp_exchange, volume, price, is_limit
    );
    auto order = common::InfoOrder(std::move(tmp));
    return "New Order";
}

std::string_view return_hello() {
    return "Hello, world!";
    
}

}

namespace server {

Session::Session(boost::asio::io_service& io_service) : socket_(io_service) {}


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
    socket_.close();
    delete[] &data_;
}

void Session::read_msg(const boost::system::error_code& error, size_t bytes_transferred) {
    if (!error) {
        data_[bytes_transferred] = '\0';

        auto j = nlohmann::json::parse(data_);
        auto reqType = j["ReqType"];
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

std::string_view Session::parse_event(nlohmann::json data) {
    std::string_view reply;
    std::string req_type = data["ReqType"];
    if (req_type == Requests::Registration) {
        reply = new_client(data);
    } else if (req_type == Requests::Transaction) {
        reply = new_order(data);
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



Server::Server(const std::string_view& uri, const size_t port) : acceptor_(io_service_), socket_(io_service_) {
    std::cout << "Server started! Listen " << port << " port" << std::endl;

    boost::asio::ip::tcp::resolver resolver(io_service_);
    boost::asio::ip::tcp::endpoint endpoint = *resolver.resolve(uri, std::to_string(port));
    acceptor_ = boost::asio::ip::tcp::acceptor(io_service_, endpoint);
    acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
    socket_ = boost::asio::ip::tcp::socket(io_service_);

    Session* new_session = new Session(io_service_);
    acceptor_.async_accept(new_session->socket(),
        boost::bind(&Server::handle_accept, this, new_session,
        boost::asio::placeholders::error));
};

Server::~Server() {
    for (std::map<size_t, Session*>::iterator it = clients_sessions_.begin(); it != clients_sessions_.end(); ++it) {
        delete it->second;
    }
}

void Server::run() {
    io_service_.run();
}

void Server::handle_accept(Session* new_session,
        const boost::system::error_code& error)
    {
        if (!error)
        {
            new_session->start();
            new_session = new Session(io_service_);
            while(new_session->getHashClient() == -1);
            clients_sessions_[new_session->getHashClient()] = new_session;
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