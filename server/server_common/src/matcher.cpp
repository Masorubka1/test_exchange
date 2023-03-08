#include "server_common/matcher.hpp"
#include <cppkafka/producer.h>

namespace server_common {

void Matcher::add(common::InfoOrder& order) noexcept {
    std::scoped_lock lock(m_mutex);
    orderbook->add(order);
}

void Matcher::remove(common::InfoOrder& order) noexcept {
    std::scoped_lock lock(m_mutex);
    std::cout << "here";
    orderbook->remove(order);
}

void Matcher::resolve_orders() {
    static cppkafka::Producer producer(conf::config_order);
    std::scoped_lock lock(m_mutex);
    auto ask_iter = ask.begin();
    auto bid_iter = bid.begin();
    if (ask_iter->second->getPrice() > get_price()) {
        return;
    }
    while (ask_iter->second->getPrice() < bid_iter->second->getPrice()) {
        if (ask_iter->second->size() == 0) {
            ++ask_iter;
            continue;
        }
        if (bid_iter->second->size() == 0) {
            ++bid_iter;
            continue;
        }
        auto best_ask_order = ask_iter->second->getBest();
        auto best_bid_order = bid_iter->second->getBest(); 
        if (best_ask_order.volume < best_bid_order.volume) {
            best_bid_order.volume -= best_ask_order.volume;
            nlohmann::json data_ask = *(best_ask_order.full_order);
            //data_ask["order_status"] = common::OrderStatus::FINISHED;
            std::string ask = data_ask.dump();
            producer.produce(cppkafka::MessageBuilder("OrderEvents").key("Finished").payload(ask));
            ask_iter->second->remove(1);
        } else if (best_ask_order.volume > best_bid_order.volume) {
            best_ask_order.volume -= best_bid_order.volume;
            nlohmann::json data_bid = *(best_ask_order.full_order.get());
            data_bid["order_status"] = common::OrderStatus::FINISHED;
            std::string bid = data_bid.dump();
            producer.produce(cppkafka::MessageBuilder("OrderEvents").key("Finished").payload(bid));
            bid_iter->second->remove(1);
        } else {
            nlohmann::json data_ask = *(best_ask_order.full_order);
            //data_ask["order_status"] = common::OrderStatus::FINISHED;
            std::string ask = data_ask.dump();
            producer.produce(cppkafka::MessageBuilder("OrderEvents").key("Finished").payload(ask));
            nlohmann::json data_bid = *(best_ask_order.full_order);
            //data_bid["order_status"] = common::OrderStatus::FINISHED;
            std::string bid = data_bid.dump();
            producer.produce(cppkafka::MessageBuilder("OrderEvents").key("Finished").payload(bid));
            ask_iter->second->remove(1);
            bid_iter->second->remove(1);
        }
    }
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
    std::cout << *orderbook << "\n";
    //std::cout << "size: " << registered_users.size() << "\n";
    //return;
}

}