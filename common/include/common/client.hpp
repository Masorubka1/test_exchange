//#ifndef CLIENT_HPP
//#define CLIENT_HPP
#pragma once

#include <string>
#include <memory>
#include <iostream>

namespace client {

struct Client {
    std::string name;
    std::string hash_password;
    int64_t balance = 0;
    bool is_valid = false;
};

inline int hash(const Client* client) {
    int h1 = std::hash<std::string>()(client->name);
    int h2 = std::hash<std::string>()(client->hash_password);
    return h1 ^ h2;
}

struct InfoClient {
    std::shared_ptr<Client> client;
    int hash_client;
    InfoClient(Client&& client) : client(std::make_shared<Client>(std::move(client))), hash_client(hash(this->client.get())) {}
    InfoClient() : client(nullptr), hash_client(-1) {}
};

}

//#endif