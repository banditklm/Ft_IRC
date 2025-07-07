#include "../headers/client.hpp"

Client::Client()
{
}

Client::Client(int fd)
{
    this->fd = fd;
    has_nick = false;
    has_user = false;
    registered = false;
}

Client::~Client()
{
}

void    Client::set_nick(std::string nickname)
{
    this->nickname = nickname;
    has_nick = true;
}

void    Client::set_user(std::string username)
{
    this->username = username;
    has_user = true;
}
