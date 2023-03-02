#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <memory>

namespace client {

struct Client {
    std::string_view name;
    std::string_view hash_password;
    int64_t balance = 0;
    bool is_valid = false;
};

std::size_t hash(const Client& client) {
    std::size_t h1 = std::hash<std::string_view>()(client->name);
    std::size_t h2 = std::hash<std::string_view>()(client->hash_password);
    return h1 ^ h2;
}

struct InfoClient {
    std::shared_ptr<Client> client;
    size_t hash_client;
    InfoClient(Client&& client) : client(std::make_shared<Client>(std::move(client))), hash_client(hash(this->client.get())) {}
};

}

#endif