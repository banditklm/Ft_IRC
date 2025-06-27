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
	pollfd listener;
	listener.fd = socket_fd;
	listener.events = POLLIN;
	poll_fds.push_back(listener);
	while (true)
	{
		int res;
		res = poll(poll_fds.data(), poll_fds.size(), -1);
		if (res > 0)
		{
			if (poll_fds[0].revents & POLLIN)
			{
				int fd_client;
				fd_client = accept(socket_fd, NULL, NULL);
				if (fd_client < 0)
				{
					std::cerr << "accept failed!" << std::endl;
					continue;
				}
				else
				{
					clients[fd_client] = Client(fd_client);
					pollfd	new_client;
					new_client.fd = fd_client;
					new_client.events = POLLIN;
					poll_fds.push_back(new_client); //the new client have just added
				}
			}
			for (int i = 1; i < poll_fds.size(); ++i)
			{
				if (poll_fds[i].revents & POLLIN)
				{
					char buff[512];
					size_t read_size;
					read_size = recv(socket_fds[i].fd, buff, sizeof(buff) - 1, 0);
					if (read_size <= 0)
					{
						close(poll_fds[i].fd);
						poll_fds.erase(poll_fds.begin() + i);
						--i;
						continue;
					}
					buff[read_size] = '\0';
				}
				else if (poll_fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
				{
					std::cerr << "client error!" << std::endl;
					close(poll_fds[i].fd);
					poll_fds[i].erase(poll_fds.begin() + i);
					--i;
					continue;
				}
			}
		}
	}
	
}