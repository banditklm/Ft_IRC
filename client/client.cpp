#include "headers/client.hpp"

Client::Client(int fd, std::string nickname, std::string username)
{
    this->fd = fd;
    this->nickname = nickname;
    this->username = username;
}

Client::~Client()
{
}
