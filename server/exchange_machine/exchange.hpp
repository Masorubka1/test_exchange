#ifndef EXCHANGE_HPP
#define EXCHANGE_HPP

#include "server_common/matcher.hpp"
#include "server_common/timer.hpp"

namespace server {

class Exchange {
public:
    void run();
    Exchange() {};

private:
    const std::unique_ptr<server_common::Matcher> matcher_;
    const Timer timer_;
    const server::Server instrument_server_; 
};

}
#endif