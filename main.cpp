#include <typeinfo>
#include <vector>

#include "client_socket.hpp"
#include "kqueue_handler.hpp"
#include "server_socket.hpp"
#include <fcntl>

int main() {
	std::vector<int> port;
	port.push_back(8080);
	port.push_back(8181);
	port.push_back(8282);

	KqueueHandler kq_handler;
	// collect kevents
	for (int i = 0; i < port.size(); ++i) {
		ServerSocket server(0, port[i]);
		server.ReadyToAccept();
		kq_handler.CollectEvents(server.GetSocketDescriptor(), EVFILT_READ,
								 EV_ADD, 0, 0, &server);
	}
	// monitor kevents
	while (1) {
		std::vector<struct kevent> event_list;
		event_list = kq_handler.MonitorEvents();
		for (int i = 0; i < event_list.size(); ++i) {
			Socket *socket = reinterpret_cast<Socket *>(event_list[i].udata);
			if (event_list[i].flags & EV_EOF) {
				std::cout << "Disconnect" << std::endl;
				close(event_list[i].ident);
				// Socket is automatically removed from the kq by the kernel
			} else if (socket->GetType() == Socket::SERVER_TYPE) {
				ServerSocket *server = dynamic_cast<ServerSocket *>(socket);
				ClientSocket client(server->AcceptClient());
				kq_handler.CollectEvents(client.GetSocketDescriptor(),
										 EVFILT_READ, EV_ADD, 0, 0, &client);
				kq_handler.CollectEvents(client.GetSocketDescriptor(),
										 EVFILT_WRITE, EV_ADD, 0, 0, &client);
			} else if (socket->GetType() == Socket::CLIENT_TYPE) {
                
			}
		}
	}
}
