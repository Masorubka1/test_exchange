#include "exchange.hpp"
#include "common/json.hpp"
#include <thread>
#include <vector>

namespace server {

void Exchange::run() {
	while (instruments.status()) {
		std::thread matcher([this] {
			matcher_->poll()
		});
		matcher.join();
	}
}

}