#ifndef CLIENT_SOCKET_HPP
#define CLIENT_SOCKET_HPP

#include <fcntl.h>

#include <cstdlib>
#include <iostream>
#include <vector>

#include "socket.hpp"

class ClientSocket : public Socket {
   public:
	ClientSocket(int sock_d);
	~ClientSocket();

   private:
	ClientSocket();
	std::vector<std::string> buffer_;
};

#endif