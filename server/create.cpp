#include "headers/server.hpp"

void    Server::init_socket()
{
    struct sockaddr_in server;

    this->socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (this->socket_fd == -1)
    {
        std::cerr << "socket failed!" << std::endl;
        exit(1);
    }
    server.sin_family = AF_INET;
    server.sin_port = htons(6667);
    server.sin_addr.s_addr = INADDR_ANY;
    if (bind(this->socket_fd, (struct sockaddr*)&server, sizeof(server)) != 0)
    {
        std::cerr << "bind failed!" << std::endl;
        exit(1);
    }
    if (listen(this->socket_fd, 13) != 0)
    {
        std::cerr << "listen failed!" << std::endl;
        exit(1);
    }
}