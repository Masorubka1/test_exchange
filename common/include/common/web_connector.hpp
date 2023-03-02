#ifndef WEB_CONNECTOR_HPP
#define WEB_CONNECTOR_HPP

#include <string>
#include <boost/asio.hpp>

namespace common {

class WebConnector {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void send_msg(const boost::system::error_code& error) const = 0;
    virtual void read_msg(const boost::system::error_code& error, size_t bytes_transferred) = 0;
    virtual void set_connection() = 0;
    virtual void parse_event(const std::string_view& message) = 0;
protected:
    WebConnector(const std::string_view& uri, const std::string_view& port){};
    ~WebConnector();
};

}

#endif