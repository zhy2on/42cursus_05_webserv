#ifndef SERVER_SOCKET_HPP
#define SERVER_SOCKET_HPP

#include "Socket.hpp"
#include <fcntl.h>
#include <cstdlib>
#include <iostream>

class ServerSocket : public Socket {
   public:
	ServerSocket(int host, int port);
	~ServerSocket();

	void ReadyToAccept();
	void AcceptClient();

   private:
	ServerSocket();
	void CreateSocket();
	void BindSocket();
	void ListenSocket();
};

#endif