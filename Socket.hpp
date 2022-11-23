#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <netinet/in.h>

class Socket {
   protected:
	int type_;
	int sock_d_;
	struct sockaddr_in address_;

   public:
	const enum {
		CLIENT_TYPE,
		SERVER_TYPE
	};

	const int &GetType() const;
	const int &GetSocketDescriptor() const;
};

#endif
