#ifndef SERVER_CONNECTOR
#define SERVER_CONNECTOR

#include "common/web_connector.hpp"
#include "common/client.hpp"
#include "server_common/clients.hpp"

#include <boost/bind/bind.hpp>
#include <boost/asio.hpp>

#include <map>
#include <memory>

namespace server {

class Session : public common::WebConnector, public std::enable_shared_from_this<Session> {
public:
    Session(boost::asio::io_service& io_service) : socket_(io_service) {
        socket_.async_read_some(new char[1],
            boost::bind(&Session::start, this));
    }
    
    void start() override;
    void stop() override;

    void read_msg(const boost::system::error_code& error, size_t bytes_transferred) override;
    void send_msg(const boost::system::error_code& error) const override;
    
    boost::asio::ip::tcp::socket& socket();
    int64_t getHashClient() const {
        return hash_client_;
    }
    
    ~Session();
private:
    //boost::asio::ip::tcp::endpoint& endpoint_;
    boost::asio::ip::tcp::socket& socket_;
    //boost::asio::ip::tcp::acceptor& acceptor_;
    //boost::asio::io_service& io_service_;
    //boost::asio::streambuf streambuf_;
    int32_t max_length = 1024;
    //char data_[max_length];
    char data_[1024];
    int64_t hash_client_ = -1;
};

class Server {
public:
    Server(const std::string_view& uri = "127.0.0.1", const std::string_view& port = "5555");
    //void send_message(const InfoClient& client, const std::string_view& message);
    //std::string_view read_message(const InfoClient& client);
    void run();
    void do_accept();
    ~Server();
private:
    //void set_connection(const InfoClient& client);
    //void parse_event(const std::string_view& message);

    boost::asio::io_service& io_service_;
    boost::asio::ip::tcp::acceptor& acceptor_;
    std::map<size_t, std::weak_ptr<Session>> clients_sessions_;
};


}


#endif