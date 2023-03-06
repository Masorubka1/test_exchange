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
    double price = 0;
    bool is_limit = false;

    /*explicit Order(std::string instrument_, 
    std::string hash_client_,
    OrderType order_type_, 
    OrderStatus order_status_, 
    ExchangeType exchange_type_, 
    int64_t timestamp_user_,
    int64_t timestamp_exchange_,
    double volume_,
    double price_,
    bool is_limit_) : instrument(instrument_),  hash_client(hash_client_),
    order_type(order_type_), order_status(order_status_), timestamp_user(timestamp_user_),
    timestamp_exchange(timestamp_exchange_), volume(volume_), price(price_), is_limit(is_limit_) {}*/

    friend OrderType string2OrderType(std::string str);
    friend ExchangeType string2ExchangeType(std::string str);
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


/*struct hash
{
    std::size_t operator() (const Client &client) const
    {
        std::size_t h1 = std::hash<string_view>()(node.instrument);
        std::size_t h2 = std::hash<string_view>()(node.client_hash);
        return h1 ^ h2;
    }
};*/

struct InfoOrder {
    std::string_view instrument;
    double volume = 0;
    double price = 0;
    std::shared_ptr<Order> full_order;
    
    explicit InfoOrder(Order&& order) : instrument(order.instrument), volume(order.volume), price(order.price), full_order(std::make_shared<Order>(std::move(order))) {}

    friend bool operator<(const InfoOrder& a, const InfoOrder& b);
    friend std::ostream& operator<<(std::ostream& os, const InfoOrder& order);
};

inline bool operator<(const InfoOrder& a, const InfoOrder& b) {
        return a.price < b.price;
}

inline std::ostream& operator<<(std::ostream& os, const InfoOrder& order) {
    os << order.price << ":" << order.volume << " ";
    return os;
}

}

//#endif