//#ifndef MATCHER_HPP
//#define MATCHER_HPP
#pragma once

#include "common/orderbook.hpp"

#include <memory>
#include <mutex>

namespace server {

class Matcher : public common::OrderBook {
public:
    void add(common::InfoOrder& order) noexcept;
    void remove(common::InfoOrder& order) noexcept;
private:
    std::mutex m_mutex;
    void resolve_orders();
    std::unique_ptr<common::OrderBook> orderbook;
};

}

//#endif