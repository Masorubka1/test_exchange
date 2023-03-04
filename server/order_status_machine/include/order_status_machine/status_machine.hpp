//#ifndef xz_HPP
//#define xz_HPP
#pragma once

#include "common/order.hpp"

#include <optional>
#include <map>

namespace server {

class StatusMachine {
public:
	StatusMachine();
	void add(common::InfoOrder order);
	void remove(common::InfoOrder order);
	std::optional<common::InfoOrder> check(common::InfoOrder order);
private:
	std::set<common::InfoOrder> all_orders;
}

}

//#endif
