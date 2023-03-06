#ifndef EXCHANGE_HPP
#define EXCHANGE_HPP

#include "server_common/matcher.hpp"
#include "server_common/timer.hpp"
#include "server_common/server_connector.hpp"

namespace server {

class Exchange {
public:
    void run();
    Exchange() { instrument_server_.run();};

private:
    std::unique_ptr<server_common::Matcher> matcher_;
    server_common::Timer timer_;
    server_common::NewIOServer instrument_server_; 
};

}
#endif