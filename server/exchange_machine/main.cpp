#include "exchange.hpp"

#include "server_common/server_connector.hpp"

int main() {
    //server::Exchange exchange;
    //exchange.run();
    auto server = server::Server();
    server.run();
    return 0;
}