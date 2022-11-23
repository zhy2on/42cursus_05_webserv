#include "server_socket.hpp"
#include "client_socket.hpp"
#include "kqueue_handler.hpp"
#include <vector>
#include <typeinfo>

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
            if (socket->GetType() == Socket::SERVER_TYPE) {
                ClientSocket client(socket->AcceptClient());
            }
            else if (socket->GetType() == Socket::CLIENT_TYPE) {

            }
        }
    }

}
