#include "server_common/matcher.hpp"
#include <cppkafka/producer.h>
#include <cstddef>
#include <iostream>

namespace server_common {

/*void Matcher::add(common::InfoOrder& order) noexcept {
    //std::scoped_lock lock(m_mutex);
    add_(order);
}

void Matcher::remove(common::InfoOrder& order) noexcept {
    //std::scoped_lock lock(m_mutex);
    remove_(order);
}*/

void Matcher::resolve_orders() {
    //static cppkafka::Producer producer(conf::config_order);
    std::scoped_lock lock(m_mutex);
    auto ask_level = getLevel(common::OrderType::Ask);
    auto bid_level = getLevel(common::OrderType::Bid);

    if (!ask_level.has_value() || !bid_level.has_value()) {
        return;
    }
    std::cout << "\nHere_ask: ";
    for (auto& [k, v] : ask) {
        std::cout << v << " ";
    }
    std::cout << "\nHere_bid: ";
    for (auto& [k, v] : bid) {
        std::cout << v << " ";
    }
    std::cout << "\n";
    while ((ask_level.has_value() && bid_level.has_value()) && (ask_level->getPrice() <= bid_level->getPrice())) {
        //std::cout << "Here:\n";
        common::InfoOrder best_ask_order = ask_level->getBest();
        common::InfoOrder best_bid_order = bid_level->getBest(); 
        if (best_ask_order.volume < best_bid_order.volume) {
            //std::cout << "\n1\n";
            best_bid_order.volume -= best_ask_order.volume;
            ask[best_bid_order.price].modify(best_bid_order.full_order.timestamp_exchange, best_bid_order);
            //std::cout << "\n2\n";
            nlohmann::json data_ask = best_ask_order.full_order;
            std::string ask = data_ask.dump();
            //producer.produce(cppkafka::MessageBuilder("OrderEvents").key("Finished").payload(ask));
            //ask_level->second->remove(*(best_ask_order.get()));
            //std::cout << "\n3\n";
            remove(best_ask_order);
            //std::cout << "\n4\n";
        } else if (best_ask_order.volume > best_bid_order.volume) {
            //std::cout << "\n2\n";
            //std::cout << "\n5\n";
            best_ask_order.volume -= best_bid_order.volume;
            ask[best_ask_order.price].modify(best_ask_order.full_order.timestamp_exchange, best_ask_order);
            //std::cout << "\n6\n";
            nlohmann::json data_bid = best_ask_order.full_order;
            //data_bid["order_status"] = common::OrderStatus::FINISHED;
            std::string bid = data_bid.dump();
            //producer.produce(cppkafka::MessageBuilder("OrderEvents").key("Finished").payload(bid));
            //bid_level->second->remove(*(best_bid_order.get()));
            //std::cout << "\n7\n";
            remove(best_bid_order);
            //std::cout << "\n8\n";
            //this->remove(best_bid_order);
        } else {
            //std::cout << "\n3\n";
            nlohmann::json data_ask = best_ask_order.full_order;
            //data_ask["order_status"] = common::OrderStatus::FINISHED;
            std::string ask = data_ask.dump();
            //producer.produce(cppkafka::MessageBuilder("OrderEvents").key("Finished").payload(ask));
            nlohmann::json data_bid = best_ask_order.full_order;
            //data_bid["order_status"] = common::OrderStatus::FINISHED;
            std::string bid = data_bid.dump();
            //producer.produce(cppkafka::MessageBuilder("OrderEvents").key("Finished").payload(bid));
            //ask_level->second->remove(*(best_ask_order.get()));
            //bid_level->second->remove(*(best_bid_order.get()));
            //std::cout << "\n9\n";
            remove(best_ask_order);
            //std::cout << "\n10\n";
            remove(best_bid_order);
            //std::cout << "\n11\n";
        }

        ask_level = getLevel(common::OrderType::Ask);
        bid_level = getLevel(common::OrderType::Bid);
        //std::cout << "Here2:\n";
    }
    std::cout << "\nHere_ask: ";
    for (auto& [k, v] : ask) {
        std::cout << v << " ";
    }
    std::cout << "\nHere_bid: ";
    for (auto& [k, v] : bid) {
        std::cout << v << " ";
    }
    std::cout << "\n";
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
    //std::cout << ask << "\n";
    //std::cout << "size: " << registered_users.size() << "\n";
    //return;
}

}