#include "order.hpp"
#include "orderbook.hpp"

#include <ostream>


namespace common {

OrderBookLevel::OrderBookLevel(OrderType t) : type(t) {};

void OrderBookLevel::add(InfoOrder&& order) {
    level[order.full_order->timestamp_exchange] = std::move(order);
}

size_t OrderBookLevel::size() const noexcept {
    return level.size();
}

int OrderBookLevel::remove(int n) noexcept {
    if (this->size() <= n) {
        n -= this->size();
        level.clear();
    } else {
        level.erase(level.begin(), std::next(level.begin(), n));
        n = 0;
    }
    return n;
}

void OrderBookLevel::remove(const InfoOrder& order) noexcept {
    level.erase(order.full_order->timestamp_exchange);
}

double OrderBookLevel::getPrice() const noexcept
{
    if (level.size() > 0) {
        return level.begin()->second.price;
    }
    return -1;
}

void OrderBookLevel::clear() noexcept {
    level.clear();
}

void OrderBook::add_(InfoOrder&& order) noexcept {
    if (order.full_order->order_type == OrderType::Ask) {
        if (ask.size() == 0) {
            ask[order.price] = OrderBookLevel(OrderType::Ask);
        }
        ask[order.price].add(std::move(order));
    } else {
        if (bid.size() == 0) {
            bid[order.price] = OrderBookLevel(OrderType::Bid);
        }
        bid[order.price].add(std::move(order));
    }
}

void OrderBook::remove_(InfoOrder&& order) noexcept {
    if (order.full_order->order_type == OrderType::Ask) {
        assert(ask.find(order.price) != ask.end());
        ask[order.price].remove(std::move(order));
    } else {
        assert(bid.find(order.price) != bid.end());
        bid[order.price].remove(std::move(order));
    }
}

bool OrderBook::is_empty() const {
    return bid.size() + ask.size() > 0;
}

void OrderBook::add(InfoOrder& order) {
    this->add_(std::move(order));
}

void OrderBook::remove(InfoOrder& order) {
    this->remove_(std::move(order));
}

double OrderBook::get_price()
{
    return (ask.begin()->second.getPrice() + bid.begin()->second.getPrice()) / 2;
}

std::ostream& operator<<(std::ostream& os, const OrderBookLevel& xz) {
    for (const auto& order: xz.level) {
        os << order.first << order.second << " ";
    }
    return os;
}

std::ostream& operator<<(std::ostream &os, const OrderBook& book)
{
    os << "ASK: \n";
    for (const auto& order: book.ask) {
        os << order.second << " ";
    }
    os << "\nBID: \n";
    for (const auto& order: book.bid) {
        os << order.second << " ";
    }
    return os;
}

}