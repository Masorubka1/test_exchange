#include "server_common/clients.hpp"
#include <cppkafka/message.h>
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

void MapClients::poll() {
    static cppkafka::Consumer cons_(conf::config_user);
    cons_.subscribe({"UserEvents"});
    cppkafka::Message msg = cons_.poll();
    std::cout << "clients1: \n";
    if (!msg) {
        std::cout << "clients2: \n";
        return;
    }
	if (msg.get_error()) {
        // librdkafka provides an error indicating we've reached the
        // end of a partition every time we do so. Make sure it's not one
        // of those cases, as it's not really an error
        if (!msg.is_eof()) {
            // Handle this somehow...
        }
        return;
    }

    nlohmann::json json_dump = nlohmann::json::parse(msg.get_payload());
    std::cout << "json_dump_clients: " << json_dump << "\n";
    client::InfoClient client = client::InfoClient{json_dump.get<client::Client>()};

    if (msg.get_key() == "Add") {
        add(client);
    } else if (msg.get_key() == "Remove") {
        remove(client);
    } else {
		// ...
    }
    std::cout << "size: " << registered_users.size() << "\n";
    return;
}

MapClients& MapClients::inst() {
	static MapClients instance = MapClients();
	return instance;
}

}