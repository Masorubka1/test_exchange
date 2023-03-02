#include "clients.hpp"

namespace server {

void MapClients::add(const common::InfoClient& client) noexcept {
	if (!check(client)) {
		registered_users[client.hash_client] = client;
	}
}

bool MapClients::check(const common::InfoClient& client) noexcept const	{
	return registered_users.find(client.hash_client) != registered_users.end();
}

void MapClients::remove(const common::InfoClient& client) noexcept {
	if (check(client)) {
		registered_users.erase(client.hash_client]);
	}	
}

}