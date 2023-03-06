#include "order_status_machine/status_machine.hpp"

#include "common/order.hpp"

#include <optional>

namespace machine {

//statusMachine::statusMachine() {}

void statusMachine::add(common::InfoOrder order) {
	all_orders.insert(order);
}

void statusMachine::remove(common::InfoOrder order) {
	all_orders.erase(order);
}

std::optional<common::InfoOrder> statusMachine::check(common::InfoOrder order) {
	if (all_orders.find(order) != all_orders.end()) {
		return order;
	}
	return std::nullopt;
}

statusMachine& statusMachine::inst() {
	static statusMachine instance = statusMachine();
	return instance;
}

}