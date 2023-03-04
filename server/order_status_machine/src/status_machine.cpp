#include "status_machine.hpp"

#include "common/order.hpp"

#include <optional>

namespace server {

StatusMachine::StatusMachine() {}

void StatusMachine::add(common::InfoOrder order) {
	all_orders.insert(order);
}

void StatusMachine::remove(common::InfoOrder order) {
	all_orders.erase(order);
}

std::optional<common::InfoOrder> StatusMachine::check(common::InfoOrder order) {
	if (all_orders.find(order) != all_orders.end()) {
		return Some(order);
	}
	return std::nullopt;

}

}