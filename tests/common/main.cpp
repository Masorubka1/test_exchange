#include "common/orderbook.hpp"
#include "common/json.hpp"

#include <boost/regex/v5/regex_fwd.hpp>
#include <gtest/gtest.h>
#include <fstream>
#include <string>

//using namespace nlohmann::literals;

static nlohmann::json js(int f=0) {
    nlohmann::json order_ask = R"(
        {
            "instrument": "USD",
            "exchange_type": "SPOT",
            "hash_client": "1234",
            "order_status": "GOT",
            "order_type": "Ask",
            "price": 10,
            "timestamp_user": 1678366069726431000,
            "timestamp_exchange": 1678366069726431001,
            "volume": 10,
            "is_limit": false
        }
    )"_json;
    nlohmann::json order_bid_1 = R"(
        {
            "instrument": "USD",
            "exchange_type": "SPOT",
            "hash_client": "1234",
            "order_status": "GOT",
            "order_type": "Bid",
            "price": 10,
            "timestamp_user": 1678366069726431000,
            "timestamp_exchange": 1678366069726431002,
            "volume": 10,
            "is_limit": false
        }
    )"_json;
    nlohmann::json order_bid_2 = R"(
        {
            "instrument": "USD",
            "exchange_type": "SPOT",
            "hash_client": "1234",
            "order_status": "GOT",
            "order_type": "Bid",
            "price": 20,
            "timestamp_user": 1678366069726431000,
            "timestamp_exchange": 1678366069726431002,
            "volume": 10,
            "is_limit": false
        }
    )"_json;
    if (f == 0) {
        return order_ask;
    } else  if (f == 1){
        return order_bid_1;
    } else {
        return order_bid_2;
    }
}

TEST(orderbooklevel, add) {

    auto order = common::InfoOrder{js().get<common::Order>()};
    common::OrderBookLevel level(common::OrderType::Ask);
    ASSERT_EQ(level.size(), 0);
    level.add(order);
    ASSERT_EQ(level.size(), 1);
}

TEST(orderbooklevel, remove_order) {
    auto order = common::InfoOrder{js().get<common::Order>()};
    common::OrderBookLevel level(common::OrderType::Ask);
    ASSERT_EQ(level.size(), 0);
    level.add(order);
    ASSERT_EQ(level.size(), 1);
    level.remove(order);
    ASSERT_EQ(level.size(), 0);
}

TEST(orderbooklevel, remove_1) {
    auto order = common::InfoOrder{js().get<common::Order>()};
    common::OrderBookLevel level(common::OrderType::Ask);
    ASSERT_EQ(level.size(), 0);
    level.add(order);
    ASSERT_EQ(level.size(), 1);
    level.remove(1);
    ASSERT_EQ(level.size(), 0);
}

TEST(orderbooklevel, get_price) {
    auto order = common::InfoOrder{js().get<common::Order>()};
    common::OrderBookLevel level(common::OrderType::Ask);
    level.add(order);
    ASSERT_EQ(level.getPrice(), 10.0);
}

TEST(orderbooklevel, get_best) {
    auto order = common::InfoOrder{js().get<common::Order>()};
    common::OrderBookLevel level(common::OrderType::Ask);
    level.add(order);
    auto return_order = level.getBest();
    ASSERT_EQ(return_order->price, order.price);
    ASSERT_EQ(return_order->volume, order.volume);
    ASSERT_EQ(return_order->full_order, order.full_order);
}

TEST(orderbook, add) {
    auto order_ask = common::InfoOrder{js().get<common::Order>()};
    auto order_bid = common::InfoOrder{js(1).get<common::Order>()};
    common::OrderBook book;
    ASSERT_EQ(book.is_empty(), 1);
    book.add(order_ask);
    ASSERT_EQ(book.is_empty(), 0);
    book.add(order_bid);
    ASSERT_EQ(book.is_empty(), 0);
}

TEST(orderbook, remove) {
    auto order_ask = common::InfoOrder{js().get<common::Order>()};
    auto order_bid = common::InfoOrder{js(1).get<common::Order>()};
    common::OrderBook book;
    ASSERT_EQ(book.is_empty(), 1);
    book.add(order_ask);
    ASSERT_EQ(book.is_empty(), 0);
    book.remove(order_ask);
    ASSERT_EQ(book.is_empty(), 1);
    book.add(order_bid);
    ASSERT_EQ(book.is_empty(), 0);
    book.remove(order_bid);
    ASSERT_EQ(book.is_empty(), 1);
}

TEST(orderbook, getPrice) {
    auto order_ask = common::InfoOrder{js().get<common::Order>()};
    auto order_bid = common::InfoOrder{js(2).get<common::Order>()};
    common::OrderBook book;
    ASSERT_EQ(book.is_empty(), 1);
    book.add(order_ask);
    ASSERT_EQ(book.getPrice(), 10);
    book.remove(order_ask);
    ASSERT_EQ(book.is_empty(), 1);
    book.add(order_bid);
    ASSERT_EQ(book.getPrice(), 20);
    book.remove(order_bid);
    ASSERT_EQ(book.is_empty(), 1);
    book.add(order_ask);
    book.add(order_bid);
    ASSERT_EQ(book.getPrice(), 15);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}