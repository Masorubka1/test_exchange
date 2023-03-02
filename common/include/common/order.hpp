#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <iostream>

namespace common {

enum class OrderType: int8_t {Ask, Bid};
enum class OrderStatus: int8_t {GOT, WAITING, FINISHED};
enum class ExchangeType: int8_t {SPOT, SWAP, FUTURE};

struct Order {
    std::string_view instrument;
    std::string_view hash_client;
    OrderType order_type;
    OrderStatus order_status;
    ExchangeType exchange_type;
    int64_t timestamp_user;
    int64_t timestamp_exchange;
    double volume = 0;
    double price = 0;
    bool is_limit = false;
};

OrderType string2OrderType(std::string_view str) {
    if (str == "Ask") {
        return OrderType::Ask;
    } else if (str == "Bid") {
        return OrderType::Bid;
    } else {
        assert(false);
    }
}

ExchangeType string2ExchangeType(std::string str) {
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
};

std::ostream& operator<<(std::ostream& os, const InfoOrder& order) {
    os << order.price << ":" << order.volume << " ";
    return os;
}

}

#endif