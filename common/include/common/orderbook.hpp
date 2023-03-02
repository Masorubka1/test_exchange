#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include "order.hpp"

#include <map>
#include <iostream>
#include <optional>

namespace {

struct OrderBookCMP {
    bool operator() (const common::InfoOrder& a, const common::InfoOrder& b) const
    {
        switch (a.full_order->order_type) {
            case common::OrderType::Ask:
                return a.price < b.price || (a.price == b.price && a.full_order->timestamp_exchange < b.full_order->timestamp_exchange);
                break;
            case common::OrderType::Bid:
                return a.price > b.price || (a.price == b.price && a.full_order->timestamp_exchange < b.full_order->timestamp_exchange);
                break;
            default:
                assert(false);
        }
    }
};

}

namespace common {

class OrderBookLevel {
public:
    explicit OrderBookLevel(OrderType t);
    void add(InfoOrder&& a);
    size_t size() const noexcept;
    void remove(const InfoOrder& a) noexcept;
    int remove(int n) noexcept;
    void clear() noexcept;
    double getPrice() const noexcept;
    friend std::ostream& operator<<(std::ostream& os, const OrderBookLevel& book);
private:
    OrderType type;
    std::map<int64_t, InfoOrder, OrderBookCMP> level;
};

class OrderBook {
public:
    bool is_empty() const;
    void add(InfoOrder& order);
    void remove(InfoOrder& order);
    double get_price();
    void match();
    friend std::ostream& operator<<(std::ostream& os, const OrderBook& book);
protected:
    std::map<double, OrderBookLevel&> ask;  //will use price as key
    std::map<double, OrderBookLevel&> bid;
    void add_(InfoOrder&& order) noexcept;
    void remove_(InfoOrder&& order) noexcept;
};

}
#endif