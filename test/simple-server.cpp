// Server side C/C++ program to demonstrate Socket
// programming
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/event.h>	// kqueue
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <map>
#include <set>
#include <vector>

class ServerSocket {
   private:
	int _port;

   public:
	void collectEvents(std::vector<struct kevent> &change_list, uintptr_t ident,
					   int16_t filter, uint16_t flags, uint32_t fflags,
					   intptr_t data, void *udata) {
		struct kevent event;

		EV_SET(&event, ident, filter, flags, fflags, data, udata);
		change_list.push_back(event);
	}

	void registerKevent(std::vector<struct kevent> &change_list, int kq,
						int port, std::set<int> &servers) {
		int server_socket, new_socket, valread;
		struct sockaddr_in address;
		int addrlen = sizeof(address);
		int opt = 1;
		char buffer[1024] = {0};

		address.sin_family = AF_INET;
		address.sin_addr.s_addr = INADDR_ANY;
		address.sin_port = htons(port);

		// Creating socket file descriptor
		if ((server_socket = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
			perror("socket failed");
			exit(EXIT_FAILURE);
		}
		// Forcefully attaching socket to the port
		if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt,
					   sizeof(opt))) {
			perror("setsockopt");
			exit(EXIT_FAILURE);
		}
		// Forcefully attaching socket to the port
		if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) <
			0) {
			perror("bind failed");
			exit(EXIT_FAILURE);
		}
		if (listen(server_socket, 1) < 0) {
			perror("listen");
			exit(EXIT_FAILURE);
		}
		fcntl(server_socket, F_SETFL, O_NONBLOCK);
		servers.insert(server_socket);

		// Add event for server socket
		collectEvents(change_list, server_socket, EVFILT_READ, EV_ADD, 0, 0,
					  NULL);
	}

	void monitorKevent() {
		std::map<int, std::string> clients;	 // map for client socket:data
		std::set<int> servers;
		std::vector<struct kevent> change_list;	 // kevent vector for changelist
		struct kevent event_list[8];			 // kevent array for eventlist
		struct kevent event;
		int kq;
		int n_of_events;

		if ((kq = kqueue()) < 0) {
			perror("kqueue");
			exit(EXIT_FAILURE);
		}

		registerKevent(change_list, kq, 8080, servers);
		registerKevent(change_list, kq, 8181, servers);
		registerKevent(change_list, kq, 8282, servers);

		std::cout << "server start" << std::endl;
		while (1) {
			// !!data() is C++11 func
			if ((n_of_events =
					 kevent(kq, (struct kevent *)change_list.data(),
							change_list.size(), event_list, 8, NULL)) < 0) {
				perror("kevent");
				exit(EXIT_FAILURE);
			}
			change_list.clear();
			for (int i = 0; i < n_of_events; ++i) {
				int fd = (int)event_list[i].ident;

				if (event_list[i].flags & EV_EOF) {
					std::cout << "Disconnect" << std::endl;
					close(fd);
					// Socket is automatically removed from the kq by the
					// kernel.
				} else if (event_list[i].filter == EVFILT_READ) {
					std::set<int>::iterator it;
					if ((it = servers.find(fd)) != servers.end()) {
						int client_socket;
						if ((client_socket = accept(*it, NULL, NULL)) < 0) {
							perror("accept");
							exit(EXIT_FAILURE);
						}
						fcntl(client_socket, F_SETFL, O_NONBLOCK);

						std::cout << "Got connection!" << std::endl;

						collectEvents(change_list, client_socket, EVFILT_READ,
									  EV_ADD, 0, 0, NULL);
						collectEvents(change_list, client_socket, EVFILT_WRITE,
									  EV_ADD, 0, 0, NULL);
						clients[client_socket] = "";
					} else if (clients.find(event_list[i].ident) !=
							   clients.end()) {
						/* read data from client */
						char buf[1024];
						int n = read(event_list[i].ident, buf, sizeof(buf));
						if (n <= 0) {
							if (n < 0)
								std::cerr << "client read error!" << std::endl;
							// disconnect_client(event_list[i].ident, clients);
						} else {
							buf[n] = '\0';
							clients[event_list[i].ident] += buf;
							std::cout << "received data from "
									  << event_list[i].ident << ": "
									  << clients[event_list[i].ident];
						}
					}
				}
			}
		}
	}
};

int main(int argc, char const *argv[]) {
	ServerSocket ss;

	ss.monitorKevent();

	return 0;
}
