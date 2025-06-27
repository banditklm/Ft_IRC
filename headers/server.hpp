#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sys/types.h>
#include <vector>
#include <map>
#include "client.hpp"
#include <poll.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class Server
{
	private:
	int socket_fd;
	int port;
	std::string password;
	std::vector<struct pollfd> poll_fds;
	std::map<int, Client>clients;

	public:
	Server(int port, std::string password);
	~Server();
	void    init_socket();
};

#endif