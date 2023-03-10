#include "server_common/matcher.hpp"
#include <cppkafka/producer.h>
#include <cstddef>
#include <iostream>

namespace server_common {

void Matcher::resolve_orders() {
    static cppkafka::Producer producer(conf::config_order);
    std::scoped_lock lock(m_mutex);
    auto ask_level = getLevel(common::OrderType::Ask);
    auto bid_level = getLevel(common::OrderType::Bid);

    if (!ask_level.has_value() || !bid_level.has_value()) {
        return;
    }
    /*std::cout << "\nHere_ask: ";
    for (auto& [k, v] : ask) {
        std::cout << v << " ";
    }
    std::cout << "\nHere_bid: ";
    for (auto& [k, v] : bid) {
        std::cout << v << " ";
    }
    std::cout << "\n";*/
    while ((ask_level.has_value() && bid_level.has_value()) && (ask_level->getPrice() <= bid_level->getPrice())) {
        common::InfoOrder* best_ask_order = ask_level->getBest();
        common::InfoOrder* best_bid_order = bid_level->getBest();
        if (best_ask_order == nullptr || best_bid_order == nullptr) {
            assert(false);
        }
        if (best_ask_order->volume < best_bid_order->volume) {
            best_bid_order->volume -= best_ask_order->volume;
            nlohmann::json data_ask = *(best_ask_order->full_order.get());
            std::string ask = data_ask.dump();
            producer.produce(cppkafka::MessageBuilder("OrderEvents").partition(1).key("Finished").payload(ask));
            remove(*best_ask_order);
        } else if (best_ask_order->volume > best_bid_order->volume) {
            best_ask_order->volume -= best_bid_order->volume;
            nlohmann::json data_bid = *(best_ask_order->full_order.get());
            std::string bid = data_bid.dump();
            producer.produce(cppkafka::MessageBuilder("OrderEvents").partition(1).key("Finished").payload(bid));
            remove(*best_bid_order);
        } else {
            nlohmann::json data_ask = *(best_ask_order->full_order.get());
            std::string ask = data_ask.dump();
            producer.produce(cppkafka::MessageBuilder("OrderEvents").partition(1).key("Finished").payload(ask));
            nlohmann::json data_bid = *(best_ask_order->full_order.get());
            std::string bid = data_bid.dump();
            producer.produce(cppkafka::MessageBuilder("OrderEvents").partition(1).key("Finished").payload(bid));
            remove(*best_ask_order);
            remove(*best_bid_order);
        }

        ask_level = getLevel(common::OrderType::Ask);
        bid_level = getLevel(common::OrderType::Bid);
    }
    /*std::cout << "\nHere_ask: ";
    for (auto& [k, v] : ask) {
        std::cout << v << " ";
    }
    std::cout << "\nHere_bid: ";
    for (auto& [k, v] : bid) {
        std::cout << v << " ";
    }
    std::cout << "\n";*/
}

void Matcher::poll() {
    cppkafka::Message msg = cons_.poll();
    std::cout << "matcher1: \n";
    if (!msg) {
        std::cout << "matcher2: \n";
        return;
    }
	if (msg.get_error()) {
        // librdkafka provides an error indicating we've reached the
        // end of a partition every time we do so. Make sure it's not one
        // of those cases, as it's not really an error
        std::cout << "HERE\n";
        if (!msg.is_eof()) {
            // Handle this somehow...
        }
        return;
    }

    nlohmann::json json_dump = nlohmann::json::parse(msg.get_payload());
    std::cout << "json_dump_matcher: " << json_dump << "\n";
    common::InfoOrder order = common::InfoOrder{json_dump.get<common::Order>()};

    if (msg.get_key() == "Add") {
        add(order);
    } else if (msg.get_key() == "Remove") {
        remove(order);
    } else {
		// ...
    }
    resolve_orders();
}

}