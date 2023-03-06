#ifndef EXCHANGE_HPP
#define EXCHANGE_HPP

#include "server_common/matcher.hpp"
#include "server_common/timer.hpp"
#include "server_common/server_connector.hpp"

namespace server {

class Exchange {
public:
    void run();
    Exchange() {};

private:
    std::unique_ptr<server_common::Matcher> matcher_;
    server_common::Timer timer_;
    server_common::Server instrument_server_; 
};

}
#endif