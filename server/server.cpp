#include "headers/server.hpp"

Server::Server(int port, std::string password)
{
    this->port = port;
    this->password = password;
}

Server::~Server()
{
}
