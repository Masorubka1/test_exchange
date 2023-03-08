//#ifndef SINGLETOON_CLIENTS_HPP
//#define SINGLETOON_CLIENTS_HPP
#pragma once

#include "common/client.hpp"
#include "common/Common.hpp"

#include <cppkafka/consumer.h>
#include <map>
#include <memory>


namespace server_common {

class MapClients {
public:
	void add(const client::InfoClient& client) noexcept;
	bool check(const client::InfoClient& client) const noexcept;
	client::InfoClient getClient(const int hash_client) noexcept;
	//void check(const size_t hash_client);
	void remove(const client::InfoClient& client) noexcept;
	//void remove(const size_t hash_client);
	static MapClients& inst();
	//~MapClients();
	void poll();
private:
	MapClients() : cons_(conf::config_user) {
		//cons_ = new cppkafka::Consumer(conf::config_user);
		cons_.subscribe({"UserEvents"});
	};
	std::map<int, client::InfoClient> registered_users;
	cppkafka::Consumer cons_;
};

}

//#endif