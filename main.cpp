#include <unistd.h>

#include <typeinfo>
#include <vector>

#include "client_socket.hpp"
#include "kqueue_handler.hpp"
#include "server_socket.hpp"

int main() {
	std::vector<int> port;
	port.push_back(8080);
	port.push_back(8181);
	port.push_back(8282);

	KqueueHandler kq_handler;
	// collect kevents
	for (int i = 0; i < port.size(); ++i) {
		ServerSocket *server = new ServerSocket(0, port[i]);
		server->ReadyToAccept();
		kq_handler.CollectEvents(server->GetSocketDescriptor(), EVFILT_READ,
								 EV_ADD, 0, 0, server);
	}
	// monitor kevents
	std::cout << "Start server" << std::endl;
	while (1) {
		// std::cout << "monitoring" << std::endl;
		std::vector<struct kevent> event_list;
		event_list = kq_handler.MonitorEvents();
		for (int i = 0; i < event_list.size(); ++i) {
			Socket *socket = reinterpret_cast<Socket *>(event_list[i].udata);
			if (event_list[i].flags & EV_EOF) {
				std::cout << "Disconnect" << std::endl;
				close(event_list[i].ident);
				// Socket is automatically removed from the kq by the kernel
			} else if (event_list[i].filter == EVFILT_READ) {
				if (socket->GetType() == Socket::SERVER_TYPE) {
					ServerSocket *server = dynamic_cast<ServerSocket *>(socket);
					ClientSocket *client = new ClientSocket(server->AcceptClient());
					kq_handler.CollectEvents(client->GetSocketDescriptor(),
											 EVFILT_READ, EV_ADD, 0, 0,
											 client);
					kq_handler.CollectEvents(client->GetSocketDescriptor(),
											 EVFILT_WRITE, EV_ADD, 0, 0,
											 client);
					std::cout << "Got connection " << client->GetSocketDescriptor() << std::endl;
				} else if (socket->GetType() == Socket::CLIENT_TYPE) {
					ClientSocket *client = dynamic_cast<ClientSocket *>(socket);
					client->ReadMessage();
					std::cout << "Get message from " << client->GetSocketDescriptor() << std::endl;
					std::cout << client->GetMessage();
				}
			}
		}
	}
}
