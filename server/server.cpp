#include "../headers/server.hpp"

Server::Server(int port)
{
    this->port = port;
    this->name = "irc.com";
}

Server::~Server()
{
}
