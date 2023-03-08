#include "exchange.hpp"

namespace server {

void Exchange::run() {
	std::thread server_thread([this] {
		instrument_server_.run();
	});
	/*std::thread matcher_thread([this] {
		matcher_.poll();
	});*/
	/*std::thread clients_thread([this] {
		server_common::MapClients::inst().poll();
	});

	std::thread machine_thread([this] {
		machine::statusMachine::inst().poll();
	});*/
	while (instrument_server_.status()) {
		//machine_thread.join();
		//clients_thread.join();
		//matcher_thread.join();
		matcher_.poll();
		server_common::MapClients::inst().poll();
		machine::statusMachine::inst().poll();
		std::cout << 1 << "\n";
	}
}

}