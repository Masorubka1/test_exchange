//#ifndef WEB_CONNECTOR_HPP
//#define WEB_CONNECTOR_HPP
#pragma once

#include <string>
#include <boost/asio.hpp>
#include <memory>

namespace common {

class WebConnector {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void send_msg(const boost::system::error_code& error) = 0;
    virtual void read_msg(const boost::system::error_code& error, size_t bytes_transferred) = 0;
    virtual ~WebConnector() {};
    WebConnector() {};
};

}

//#endif