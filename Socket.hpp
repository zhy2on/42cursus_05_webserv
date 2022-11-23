#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>

class Socket {
   protected:
	int sock_d_;
	struct sockaddr_in address_;
};

#endif