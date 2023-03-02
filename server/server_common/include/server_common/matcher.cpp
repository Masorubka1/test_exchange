#include "matcher.hpp"

namespace server {

void Matcher::add(const common::InfoOrder& order) {
    orderbook->add(order);
}

void Matcher::remove(const common::InfoOrder& order) {
    orderbook->remove(order);
}

void Matcher::resolve_orders() {
    std::scoped_lock lock(m_mutex);
    auto ask_iter = ask.begin();
    auto bid_iter = bid.begin();
    if (ask_iter->second.getPrice() > get_price()) {
        return;
    }
    while (ask_iter->second.getPrice() < bid_iter->second.getPrice()) {
        auto ask_level = ask_iter->second;
        auto bid_level = bid_iter->second;
        if (ask_level.size() > bid_level.size()) {
            ask_level.remove(bid_level.size());
            bid_level.clear();
            ++bid_iter;
        } else {
            bid_level.remove(ask_level.size());
            ask_level.clear();
            ++ask_iter;
        }
    }
}

}