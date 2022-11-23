#include "ServerSocket.hpp"

ServerSocket::ServerSocket(int host, int port) {
	address_.sin_family = AF_INET;
	address_.sin_addr.s_addr = INADDR_ANY;
	address_.sin_port = htons(port);
}

ServerSocket::~ServerSocket() {}

void ServerSocket::ReadyToAccept() {
	CreateSocket();
	BindSocket();
	ListenSocket();
}

void ServerSocket::AcceptClient() {
	
}

void ServerSocket::CreateSocket() {
	int opt;

	if ((sock_d_ = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	fcntl(sock_d_, F_SETFL, O_NONBLOCK);
	opt = 1;
	if (setsockopt(sock_d_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
}

void ServerSocket::BindSocket() {
	if (bind(sock_d_, (struct sockaddr *)&address_, sizeof(address_)) < 0) {
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
}

void ServerSocket::ListenSocket() {
	if (listen(sock_d_, 1) < 0) {
		perror("listen");
		exit(EXIT_FAILURE);
	}
}