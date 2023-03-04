#include "exchange.hpp"
#include "common/json.hpp"
#include <vector>

namespace server {

void Exchange::run() {
	instrument_server_.run();
}

}