#include "client_socket.hpp"

ClientSocket::ClientSocket(int sock_d) {
	type_ = Socket::CLIENT_TYPE;
	sock_d_ = sock_d;
}

ClientSocket::~ClientSocket() {}
