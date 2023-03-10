//#ifndef MATCHER_HPP
//#define MATCHER_HPP
#pragma once

#include "common/orderbook.hpp"
#include "common/Common.hpp"

#include <cppkafka/consumer.h>
#include <cppkafka/topic_partition.h>
#include <memory>
#include <mutex>

namespace server_common {

class Matcher : public common::OrderBook {
public:
    void poll();
    Matcher() : cons_(conf::config_order) {
        //cons_.subscribe({"OrderEvents"});
        cons_.assign({cppkafka::TopicPartition{"OrderEvents", 0}});
    }
    void resolve_orders();
    //void add(common::InfoOrder& order) noexcept override;
    //void remove(common::InfoOrder& order) noexcept override;
private:
    std::mutex m_mutex;
    cppkafka::Consumer cons_;
};

}

//#endif