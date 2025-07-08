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
#include <ctime>
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
	std::string	name;
	std::string password;
	std::vector<struct pollfd> poll_fds;
	std::map<int, Client>clients;

	public:
	Server(int port, std::string password);
	~Server();
	void    init_socket();
	void	handle_line(Client& c, const std::string& line);
	void	handle_buff_line(Client& c, const std::string& buff);
	void	welcome_msg(Client& c);
	void	send_msg(Client& c, std::string msg);
	// void	error_reply(int fd_client, const std::string& )
};

#endif