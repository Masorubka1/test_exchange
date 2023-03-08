#include "exchange.hpp"

#include "server_common/server_connector.hpp"

int main() {
    server::Exchange exchange;
    exchange.run();
    //auto server = server_common::Server();
    //auto server = server_common::NewIOServer();
    //server.run();
    //while (server.status());
    return 0;
}