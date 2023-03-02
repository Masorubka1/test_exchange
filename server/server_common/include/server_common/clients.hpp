#ifndef SINGLETOON_CLIENTS_HPP
#define SINGLETOON_CLIENTS_HPP

#include "common/client.hpp"

#include <memory>

namespace server {

class MapClients : public std::enable_shared_from_this<MapClients> {
public:
	void add(const common::InfoClient& client) noexcept;
	void check(const common::InfoClient& client) const noexcept;
	//void check(const size_t hash_client);
	void remove(const common::InfoClient& client) noexcept;
	//void remove(const size_t hash_client);
private:
	MapClients() = delete;
	static std::shared_ptr<MapClients> instanse() {
		return this;
	}
	static std::map<size_t, common::InfoClient> registered_users;
};

}

#endif