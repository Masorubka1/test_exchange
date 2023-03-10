//#ifndef ORDER_HPP
//#define ORDER_HPP
#pragma once

#include <string>
#include <iostream>
#include <memory>
#include <cassert>
#include <common/json.hpp>

namespace common {

enum class OrderType {Ask, Bid};
enum class OrderStatus {GOT, WAITING, FINISHED};
enum class ExchangeType {SPOT, SWAP, FUTURE};


NLOHMANN_JSON_SERIALIZE_ENUM(
    OrderType, 
    {
        {OrderType::Ask, "Ask"},  {OrderType::Bid, "Bid"}
    })

NLOHMANN_JSON_SERIALIZE_ENUM(
    OrderStatus,
    {
        {OrderStatus::GOT, "GOT"}, {OrderStatus::WAITING, "WAITING"}, {OrderStatus::FINISHED, "FINISHED"}
    })

NLOHMANN_JSON_SERIALIZE_ENUM(ExchangeType,
{
    {ExchangeType::SPOT, "SPOT"}, {ExchangeType::SWAP, "SWAP"}, {ExchangeType::FUTURE, "FUTURE"}
});

struct Order {
    std::string instrument;
    std::string hash_client;
    OrderType order_type;
    OrderStatus order_status;
    ExchangeType exchange_type;
    int64_t timestamp_user;
    int64_t timestamp_exchange;
    double volume = 0;
    int price = 0;
    bool is_limit = false;

    friend OrderType string2OrderType(std::string str);
    friend ExchangeType string2ExchangeType(std::string str);
    
    bool operator==(const Order &order) const {
        return std::tie(instrument, hash_client, order_type, exchange_type, price, volume) == 
            std::tie(order.instrument, order.hash_client, order.order_type, order.exchange_type, order.price, order.volume);
    }
};


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Order, instrument, hash_client, order_type, order_status, exchange_type, timestamp_user, timestamp_exchange, volume, price, is_limit);

inline OrderType string2OrderType(std::string str) {
    if (str == "Ask") {
        return OrderType::Ask;
    } else if (str == "Bid") {
        return OrderType::Bid;
    } else {
        assert(false);
    }
}

inline ExchangeType string2ExchangeType(std::string str) {
    if (str == "SPOT") {
        return ExchangeType::SPOT;
    } else if (str == "SWAP") {
        return ExchangeType::SWAP;
    } else if (str == "FUTURE") {
        return ExchangeType::FUTURE;
    } else {
        assert(false);
    }
}

struct InfoOrder {
    std::string instrument;
    double volume = 0;
    int price = 0;
    Order full_order;
    
    explicit InfoOrder(Order&& order) : instrument(order.instrument), volume(order.volume), price(order.price), full_order(order) {}

    friend bool operator<(const InfoOrder& a, const InfoOrder& b);
    friend std::ostream& operator<<(std::ostream& os, const InfoOrder& order);
    
    bool operator==(const InfoOrder &order) const {
        return std::tie(instrument, volume, price) == 
            std::tie(order.instrument, order.price, order.volume);
    }
};

inline int hash(const InfoOrder* order)
{
    int h1 = std::hash<std::string>()(order->instrument);
    int h2 = std::hash<double>()(order->volume);
    int h3 = std::hash<int>()(order->price);
    return h1 ^ h2;
}

inline bool operator<(const InfoOrder& a, const InfoOrder& b) {
        return a.price < b.price;
}

inline std::ostream& operator<<(std::ostream& os, const InfoOrder& order) {
    os << order.price << ":" << order.volume << " ";
    return os;
}

}

//#endif