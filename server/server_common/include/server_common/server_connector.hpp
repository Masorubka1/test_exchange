//#ifndef SERVER_CONNECTOR_HPP
//#define SERVER_CONNECTOR_HPP
#pragma once

#include "server_common/clients.hpp"
#include "server_common/timer.hpp"
#include "common/order.hpp"
#include "common/json.hpp"
#include "common/Common.hpp"
#include "common/web_connector.hpp"
#include "order_status_machine/status_machine.hpp"


#include <cppkafka/event.h>
#include <memory>
#include <string>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>
#include <chrono>
#include <thread>
#include <cppkafka/cppkafka.h>

namespace server_common {

class Session : public common::WebConnector, std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::io_service& io_service);
    
    void start() override;
    void stop() override;

    void read_msg(const boost::system::error_code& error, size_t bytes_transferred) override;
    void send_msg(const boost::system::error_code& error) override;
    
    boost::asio::ip::tcp::socket& socket();
    std::string getHashClient() const {
        return hash_client_;
    }

    std::string parse_event(nlohmann::json data);
    
    ~Session();
private:
    boost::asio::ip::tcp::socket socket_;
    enum { max_length = 2048 };
    char data_[max_length];
    std::string hash_client_;
    //const cppkafka::Producer& prod_;
};

class Server {
public:
    Server(boost::asio::io_service& io_service_, const std::string& uri = "127.0.0.1", const size_t port = 5555);
    void run();
    ~Server();
private:
    void handle_accept(Session* new_session, const boost::system::error_code& error);

    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    //static const cppkafka::Producer& prod_(conf::config);
};

class NewIOServer {
public:
    NewIOServer() {
        s_ = new server_common::Server(io_service_);
    }

    void run() {
        io_service_.run();
    }

    bool status() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return true;
    }
private:
    boost::asio::io_service io_service_;
    server_common::Server* s_;
};

}


//#endif