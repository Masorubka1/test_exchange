#include "common/orderbook.hpp"
#include <cstddef>
#include <memory>


namespace common {

OrderBookLevel::OrderBookLevel(OrderType t) : type(t) {};

void OrderBookLevel::add(InfoOrder& order) {
    level.insert(std::move(order));
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
    level.erase(order);
}

double OrderBookLevel::getPrice() const noexcept
{
    if (level.size() > 0) {
        return level.begin()->price;
    }
    return -1;
}

InfoOrder OrderBookLevel::getBest() noexcept
{
    if (level.size() > 0) {

        return static_cast<InfoOrder>(*level.begin());
    }
    assert(false);
    //return reinpreter_cast<InfoOrder>(nullptr); // ha-ha
}

void OrderBookLevel::clear() noexcept {
    level.clear();
}

void OrderBook::add_(InfoOrder order) noexcept {
    auto tmp = std::move(std::make_unique<OrderBookLevel>(OrderType::Bid));
    
    if (order.full_order->order_type == OrderType::Ask) {
        if (ask.find(order.price) == ask.end()) {
            ask[order.price].reset();
            ask.insert({order.price, std::move(std::make_unique<OrderBookLevel>(OrderType::Ask))});
        }
        ask[order.price]->add(order);
    } else {
        if (bid.find(order.price) == bid.end()) {
            bid[order.price] = std::move(tmp);
            bid.insert({order.price, std::move(std::make_unique<OrderBookLevel>(OrderType::Bid))});
            //bid[order.price] = std::move(std::make_unique<OrderBookLevel>(OrderType::Bid));
        }
        bid[order.price]->add(order);
    }
}

void OrderBook::remove_(InfoOrder&& order) noexcept {
    if (order.full_order->order_type == OrderType::Ask) {
        assert(ask.find(order.price) != ask.end());
        ask[order.price]->remove(std::move(order));
    } else {
        assert(bid.find(order.price) != bid.end());
        bid[order.price]->remove(std::move(order));
    }
}

bool OrderBook::is_empty() const {
    return bid.size() + ask.size() > 0;
}

void OrderBook::add(InfoOrder& order) {
    this->add_(order);
}

void OrderBook::remove(InfoOrder& order) {
    this->remove_(std::move(order));
}

double OrderBook::get_price()
{
    return (ask.begin()->second->getPrice() + bid.begin()->second->getPrice()) / 2;
}

std::ostream& operator<<(std::ostream& os, const OrderBookLevel& xz) {
    for (const auto& order: xz.level) {
        os << order << " ";
    }
    return os;
}

std::ostream& operator<<(std::ostream &os, const OrderBook& book)
{
    os << "ASK: \n";
    for (const auto& order: book.ask) {
        os << *order.second << " ";
    }
    os << "\nBID: \n";
    for (const auto& order: book.bid) {
        os << *order.second << " ";
    }
    return os;
}

}