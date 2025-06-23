#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>

class Client
{
    private :
    int fd;
    std::string nickname;
    std::string username;

    public :
    Client(int fd, std::string nickname, std::string username);
    ~Client();
    
};

#endif