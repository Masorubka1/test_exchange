//#ifndef xz_HPP
//#define xz_HPP
#pragma once

#include "common/order.hpp"

#include <optional>
#include <set>

namespace machine {

class statusMachine {
public:
	void add(common::InfoOrder order);
	void remove(common::InfoOrder order);
	std::optional<common::InfoOrder> check(common::InfoOrder order);
	static statusMachine& inst();
private:
	statusMachine() {};
	std::set<common::InfoOrder> all_orders;
};

}

//#endif
