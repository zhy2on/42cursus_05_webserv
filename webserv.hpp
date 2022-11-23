#ifndef WEBSERV_HPP
#define WEBSERV_HPP

#include "client_socket.hpp"
#include "kqueue_handler.hpp"
#include "server_socket.hpp"

class Webserv {
   public:
	   void StartServer();

   private:
	   KqueueHandler kq_handler_;
};

#endif
