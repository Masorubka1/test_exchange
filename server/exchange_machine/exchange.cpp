#include "exchange.hpp"

namespace server {

void Exchange::run() {
	std::thread server_thread([this] {
		instrument_server_.run();
	});
	std::thread matcher_thread([this] {
		while (instrument_server_.status()) {
			matcher_.poll();
		}
	});
	matcher_thread.detach();
	std::thread clients_thread([this] {
		while (instrument_server_.status()) {
			server_common::MapClients::inst().poll();
		}
	});
	clients_thread.detach();
	std::thread machine_thread([this] {
		while (instrument_server_.status()) {
			machine::statusMachine::inst().poll();
		}
	});
	std::cout << "started:) \n";
	machine_thread.join();
}

}