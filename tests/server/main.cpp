#include "common/orderbook.hpp"
#include "common/json.hpp"
#include "server_common/matcher.hpp"
#include "server_common/clients.hpp"

#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <iostream>

//fix situations with same ts_ex

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
            "volume": 9,
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
            "price": 10,
            "timestamp_user": 1678366069726431000,
            "timestamp_exchange": 1678366069726431003,
            "volume": 1,
            "is_limit": false
        }
    )"_json;
    if (f == 0) {
        return order_ask;
    } else if (f == 1) {
        return order_bid_1;
    } else {
        return order_bid_2;
    }
}

TEST(matcher, resolve_orders) {
    auto order_ask = common::InfoOrder{js().get<common::Order>()};
    auto order_bid_1 = common::InfoOrder{js(1).get<common::Order>()};
    auto order_bid_2 = common::InfoOrder{js(2).get<common::Order>()};
    server_common::Matcher matcher;
    ASSERT_EQ(matcher.is_empty(), 1);
    matcher.add(order_ask);
    ASSERT_EQ(matcher.is_empty(), 0);
    matcher.add(order_bid_1);
    matcher.resolve_orders();
    ASSERT_EQ(matcher.is_empty(), 0);
    matcher.add(order_bid_2);
    matcher.resolve_orders();
    ASSERT_EQ(matcher.is_empty(), 1);
}

int main(int argc, char *argv[]) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}