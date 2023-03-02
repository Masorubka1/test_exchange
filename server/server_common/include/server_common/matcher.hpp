#ifndef MATCHER_HPP
#define MATCHER_HPP
#include "common/orderbook.hpp"
#include <memory>

namespace server {

class Matcher : public common::OrderBook {
public:
    void add(const common::InfoOrder& order) noexcept;
    void remove(const common::InfoOrder& order) noexcept;
private:
    std::mutex m_mutex;
    void resolve_orders();
    std::unique_ptr<common::OrderBook> orderbook;
};

}

#endif