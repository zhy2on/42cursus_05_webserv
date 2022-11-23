#include "socket.hpp"

const int &Socket::GetType() const { return type_; }
const int &Socket::GetSocketDescriptor() const { return sock_d_; }
