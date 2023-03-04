//#ifndef SINGLETOON_CLIENTS_HPP
//#define SINGLETOON_CLIENTS_HPP
#pragma once

#include "common/client.hpp"

#include <map>
#include <memory>


namespace server {

class MapClients {
public:
	void add(const client::InfoClient& client) noexcept;
	bool check(const client::InfoClient& client) const noexcept;
	//void check(const size_t hash_client);
	void remove(const client::InfoClient& client) noexcept;
	//void remove(const size_t hash_client);
	static MapClients& inst();
	//~MapClients();
private:
	MapClients() {};
	std::map<size_t, client::InfoClient> registered_users;
};

}

//#endif