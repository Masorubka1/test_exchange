#include "server_common/clients.hpp"

namespace server {

void MapClients::add(const client::InfoClient& client) noexcept {
	if (this->check(client)) {
		registered_users[client.hash_client] = client;
	}
}

bool MapClients::check(const client::InfoClient& client) const noexcept	{
	return (registered_users.find(client.hash_client) != registered_users.end());
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