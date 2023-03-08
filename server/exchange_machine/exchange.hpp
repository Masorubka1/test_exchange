//#ifndef EXCHANGE_HPP
//#define EXCHANGE_HPP
#pragma once

#include "common/json.hpp"
#include "server_common/matcher.hpp"
#include "server_common/timer.hpp"
#include "server_common/server_connector.hpp"
#include "order_status_machine/status_machine.hpp"

#include <thread>
#include <vector>

namespace server {

class Exchange {
public:
    void run();
    Exchange() {};

private:
    server_common::Matcher matcher_;
    server_common::Timer timer_;
    server_common::NewIOServer instrument_server_;
};

}

//#endif