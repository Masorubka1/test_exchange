//#ifndef ORDERBOOK_HPP
//#define ORDERBOOK_HPP
#pragma once

#include "order.hpp"

#include <cstddef>
#include <set>
#include <map>
#include <iostream>
#include <optional>
#include <iostream>
#include <cassert>
#include <vector>

namespace common {

/*struct OrderBookCMP {
    bool operator()(const std::shared_ptr<common::InfoOrder> a, const std::shared_ptr<common::InfoOrder> b) const
    {
        switch (a->full_order.order_type) {
            case common::OrderType::Ask:
                return a->price < b->price || (a->price == b->price && a->full_order.timestamp_exchange < b->full_order.timestamp_exchange);
                break;
            case common::OrderType::Bid:
                return a->price > b->price || (a->price == b->price && a->full_order.timestamp_exchange < b->full_order.timestamp_exchange);
                break;
            default:
                assert(false);
        }
    }
};*/

//}

//namespace common {

class OrderBookLevel {
public:
    explicit OrderBookLevel(OrderType t);
    OrderBookLevel() : type(OrderType::Ask) {};
    void add(const InfoOrder& a);
    size_t size() const noexcept;
    void remove(InfoOrder& a) noexcept;
    void remove(const InfoOrder* order) noexcept;
    void remove(int n) noexcept;
    void modify(int64_t ts, InfoOrder& order);
    void clear() noexcept;
    std::optional<int> getPrice() noexcept;
    InfoOrder* getBest() noexcept;
    friend std::ostream& operator<<(std::ostream& os, const OrderBookLevel& book);
    ~OrderBookLevel() {
        level.clear();
    }
private:
    OrderType type;
    std::map<int64_t, std::shared_ptr<InfoOrder>> level;
};

class OrderBook {
public:
    bool is_empty() const;
    void add(InfoOrder& order);
    void remove(InfoOrder& order);
    std::optional<int> getPrice();
    friend std::ostream& operator<<(std::ostream& os, const OrderBook& book);
    OrderBook() = default;
protected:
    std::map<int, OrderBookLevel> ask;  //will use price as key
    std::map<int, OrderBookLevel> bid;
    void add_(InfoOrder& order) noexcept;
    void remove_(InfoOrder& order) noexcept;
    std::optional<OrderBookLevel> getLevel(OrderType t);
};

}
//#endif