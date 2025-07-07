#include "../headers/client.hpp"

Client::Client()
{
}

Client::Client(int fd)
{
    this->fd = fd;
    registered = false;
}

Client::~Client()
{
}
