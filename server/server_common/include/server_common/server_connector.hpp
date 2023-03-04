//#ifndef SERVER_CONNECTOR_HPP
//#define SERVER_CONNECTOR_HPP
#pragma once

#include "server_common/clients.hpp"
#include "server_common/timer.hpp"
#include "common/order.hpp"
#include "common/json.hpp"
#include "common/Common.hpp"
#include "common/web_connector.hpp"


#include <string>
#include <iostream>
#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

namespace server {

class Session : public common::WebConnector {
public:
    Session(boost::asio::io_service& io_service);
    
    void start() override;
    void stop() override;

    void read_msg(const boost::system::error_code& error, size_t bytes_transferred) override;
    void send_msg(const boost::system::error_code& error) override;
    
    boost::asio::ip::tcp::socket& socket();
    int64_t getHashClient() const {
        return hash_client_;
    }
    
    ~Session();
private:
    std::string_view parse_event(nlohmann::json data);
    boost::asio::ip::tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
    int64_t hash_client_ = -1;
};

class Server {
public:
    Server(const std::string_view& uri = "127.0.0.1", const size_t port = 5555);
    void run();
    ~Server();
private:
    void handle_accept(Session* new_session, const boost::system::error_code& error);

    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::socket socket_;
    boost::asio::ip::tcp::acceptor acceptor_;
    std::map<size_t, Session*> clients_sessions_;
};


}


//#endif