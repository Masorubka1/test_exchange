#include "server_common/clients.hpp"
#include <iostream>

namespace server_common {

void MapClients::add(const client::InfoClient& client) noexcept {
	if (!(this->check(client))) {
		registered_users[client.hash_client] = client;
	}
}

bool MapClients::check(const client::InfoClient& client) const noexcept	{
	return (registered_users.find(client.hash_client) != registered_users.end());
}

client::InfoClient MapClients::getClient(const int hash_client) noexcept {
	/*std::cout << "here: "<< registered_users.size() << "\n";
	for (auto& [k, v]: registered_users) {
		std::cout << k << " " << v.hash_client;
	}*/
	return registered_users.at(hash_client);
}

void MapClients::remove(const client::InfoClient& client) noexcept {
	if (this->check(client)) {
		registered_users.erase(client.hash_client);
	}	
}

MapClients& MapClients::inst() {
	static MapClients instance = MapClients();
	return instance;
}

}