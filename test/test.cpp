#include <iostream>	 // std::cout
#include <typeinfo>	 // operator typeid

class Socket {};

class ServerSocket : public Socket {
   private:
	int host;
	int port;
};

class ClientSocket : public Socket {
	std::string s;
};

int main() {
	ServerSocket ss, s2;
	ClientSocket cs;

    Socket *socket = &ss;

    std::cout << typeid(socket).name() << std::endl;
    std::cout << typeid(cs).name() << std::endl;
    std::cout << (typeid(ss) == typeid(ServerSocket)) << std::endl;
}