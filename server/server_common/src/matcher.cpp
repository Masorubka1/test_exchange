#include "server_common/matcher.hpp"

namespace server_common {

void Matcher::add(common::InfoOrder& order) noexcept {
    orderbook->add(order);
}

void Matcher::remove(common::InfoOrder& order) noexcept {
    orderbook->remove(order);
}

void Matcher::resolve_orders() {
    std::scoped_lock lock(m_mutex);
    auto ask_iter = ask.begin();
    auto bid_iter = bid.begin();
    if (ask_iter->second->getPrice() > get_price()) {
        return;
    }
    while (ask_iter->second->getPrice() < bid_iter->second->getPrice()) {
        //auto ask_level = ask_iter->second;
        //auto bid_level = bid_iter->second;
        if (ask_iter->second->size() > bid_iter->second->size()) {
            ask_iter->second->remove(bid_iter->second->size());
            bid_iter->second->clear();
            ++bid_iter;
        } else {
            bid_iter->second->remove(ask_iter->second->size());
            ask_iter->second->clear();
            ++ask_iter;
        }
    }
}

}