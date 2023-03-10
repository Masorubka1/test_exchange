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

void statusMachine::poll() {
	cppkafka::Message msg = cons_.poll();
	std::cout << "machine1: \n";
    if (!msg) {
		std::cout << "machine2: \n";
        return;
    }
	if (msg.get_error()) {
        // librdkafka provides an error indicating we've reached the
        // end of a partition every time we do so. Make sure it's not one
        // of those cases, as it's not really an error
        if (!msg.is_eof()) {
            // Handle this somehow...
        }
        return;
    }

    nlohmann::json json_dump = nlohmann::json::parse(msg.get_payload());
    std::cout << "json_dump_status: " << json_dump << "\n";
    common::InfoOrder order = common::InfoOrder{json_dump.get<common::Order>()};

    if (msg.get_key() == "Add") {
        add(order);
    } else if (msg.get_key() == "Finished") {
        check(order)->full_order->order_status = common::OrderStatus::FINISHED;
    } else if (msg.get_key() == "Remove") {
        remove(order);
    } else {
		// ...
    }
}

statusMachine& statusMachine::inst() {
	static statusMachine instance = statusMachine();
	return instance;
}

}