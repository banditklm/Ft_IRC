#include "headers/client.hpp"

Client::Client(int fd)
{
    this->fd = fd;
    registered = false;
}

Client::~Client()
{
}
