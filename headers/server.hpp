#ifndef SERVER_HPP
#define SERVER_HPP

#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <vector>
#include <map>
#include <cstdlib>
#include <fstream>
#include <unistd.h>
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
	std::string	name;
	Server(int port);
	~Server();
	void    init_socket();
	void	handle_line(Client c, std::string line);
	// void	error_reply(int fd_client, const std::string& )
};

#endif