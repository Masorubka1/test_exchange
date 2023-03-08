//#ifndef xz_HPP
//#define xz_HPP
#pragma once

#include "common/order.hpp"
#include "common/Common.hpp"

#include <optional>
#include <set>
#include <cppkafka/consumer.h>
#include <cppkafka/topic_partition.h>

namespace machine {

class statusMachine {
public:
	void add(common::InfoOrder order);
	void remove(common::InfoOrder order);
	std::optional<common::InfoOrder> check(common::InfoOrder order);
	static statusMachine& inst();
	void poll();
private:
	statusMachine() : cons_(conf::config_order) {
		//cons_ = new cppkafka::Consumer();
		//cons_.subscribe({"OrderEvents"});
		cons_.assign({cppkafka::TopicPartition{"OrderEvents", 1}});
	};
	std::set<common::InfoOrder> all_orders;
	cppkafka::Consumer cons_;
};

}

//#endif
