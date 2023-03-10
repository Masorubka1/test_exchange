#include "common/orderbook.hpp"
#include "gtest/gtest.h"
#include <cstddef>
#include <iterator>
#include <memory>


namespace common {

OrderBookLevel::OrderBookLevel(OrderType t) : type(t) {};

void OrderBookLevel::add(const InfoOrder& order) {
    level[order.full_order.timestamp_exchange] = std::move(std::make_unique<InfoOrder>(order));
}

size_t OrderBookLevel::size() const noexcept {
    return level.size();
}

void OrderBookLevel::remove(int n) noexcept {
    if (this->size() <= n) {
        level.clear();
    } else {
        std::map<int64_t, std::unique_ptr<InfoOrder>>::iterator it = level.begin();
        while (n > 0) {
            level.erase(it++);
            --n;
        }
    }
}

void OrderBookLevel::remove(InfoOrder& order) noexcept {
    level.erase(order.full_order.timestamp_exchange);
}

void OrderBookLevel::remove(const InfoOrder* order) noexcept {
    level.erase(order->full_order.timestamp_exchange);
}

int OrderBookLevel::getPrice() noexcept {
    return this->getBest()->price;
}

InfoOrder* OrderBookLevel::getBest() noexcept {
    if (level.begin() != level.end()) {
        return level.begin()->second.get();
    }
    assert(false);
    //return reinpreter_cast<InfoOrder>(nullptr); // ha-ha
}

void OrderBookLevel::clear() noexcept {
    level.clear();
}

void OrderBook::add_(InfoOrder& order) noexcept {
    if (order.full_order.order_type == OrderType::Ask) {
        if (ask.find(order.price) == ask.end()) {
            ask[order.price] = std::move(std::make_unique<OrderBookLevel>(OrderType::Ask));
        }
        ask[order.price]->add(order);
    } else {
        if (bid.find(order.price) == bid.end()) {
            bid[order.price] = std::move(std::make_unique<OrderBookLevel>(OrderType::Bid));
        }
        bid[order.price]->add(order);
    }
}

void OrderBook::remove_(InfoOrder& order) noexcept {
    /*std::cout << "\nrm_Here_ask: ";
    for (auto& [k, v] : ask) {
        std::cout << *v << " ";
    }
    std::cout << "\nrm_Here_bid: ";
    for (auto& [k, v] : bid) {
        std::cout << *v << " ";
    }
    std::cout << "\n";*/
    if (order.full_order.order_type == OrderType::Ask) {
        if (ask.find(order.price) != ask.end()) {
            ask[order.price]->remove(order);
            if (ask[order.price]->size() == 0) {
                ask.erase(order.price);
            }
        } else {
            assert(false);
        }
    } else {
        if (bid.find(order.price) != bid.end()) {
            bid[order.price]->remove(order);
            if (bid[order.price]->size() == 0) {
                bid.erase(order.price);
            }
        } else {
            assert(false);
        }
    }
}

bool OrderBook::is_empty() const {
    int cnt = 0;
    for (auto& [k, v] : ask) {
        cnt += v->size();
    }
    for (auto& [k, v] : bid) {
        cnt += v->size();
    }
    return (cnt == 0);
}

void OrderBook::add(InfoOrder& order) {
    this->add_(order);
}

void OrderBook::remove(InfoOrder& order) {
    this->remove_(order);
}

int OrderBook::getPrice()
{
    if (ask.begin() == ask.end() && bid.begin() == bid.end()) {
        return -1;
    } else if (ask.begin() == ask.end()) {
        auto bid_order_level = bid.begin()->second.get();
        return bid_order_level->getPrice();
    } else if (bid.begin() == bid.end()) {
        auto ask_order_level = ask.begin()->second.get();
        return ask_order_level->getPrice();
    } else {
        auto ask_order_level = ask.begin()->second.get();
        auto bid_order_level = bid.begin()->second.get();
        return (ask_order_level->getPrice() + bid_order_level->getPrice()) / 2;
    }
}

OrderBookLevel* OrderBook::getLevel(OrderType t) {
    switch (t) {
        case common::OrderType::Ask:
            if (ask.begin() != ask.end()) {
                return ask.begin()->second.get();
            } else {
                return nullptr;
            }
        case common::OrderType::Bid:
            if (ask.begin() != ask.end()) {
                return bid.begin()->second.get();
            } else {
                return nullptr;
            }
        default:
            assert(false);
    }
}

std::ostream& operator<<(std::ostream& os, const OrderBookLevel& xz) {
    for (const auto& order: xz.level) {
        os << *(order.second.get()) << " ";
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