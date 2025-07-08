#include "../headers/server.hpp"
#include "../headers/client.hpp"

std::string	get_current_time()
{
	char	buff[100];
	std::time_t now = std::time(NULL);
	std::tm* time_info = std::localtime(&now);
	std::strftime(buff, sizeof(buff), "%a %b %d %Y %H:%M:%S", time_info);
	return (std::string(buff));
}

void	Server::send_msg(Client& c, std::string msg)
{
	std::string	message;

	message = ":" + this->name + " " + msg + "\r\n";
	send(c.get_fd(), message.c_str(), message.length(), 0);
}

void	Server::welcome_msg(Client& c)
{
	std::string nick = c.get_nick();
	std::string	user = c.get_user();
	std::string name = this->name;
	std::string	version = "1.0";
	std::string date = get_current_time();
	std::string	user_modes = "i";
	std::string	chan_modes = "it";
	send_msg(c, "001 " + nick + " :Welcome to the Internet Relay Network " + nick + "!" + user + "@" + name);
	send_msg(c, "002 " + nick + " :Your host is " + name + ", running version " + version);
	send_msg(c, "003 " + nick + " :This server was created " + date);
	send_msg(c, "004 " + nick + " " + name + " " + version + " " + user_modes + " " + chan_modes);
}

void	Server::handle_line(Client& c, const std::string& line)
{
	std::stringstream	ss;
	ss << line;
	std::string	cmd;
	ss >> cmd;
	if (cmd == "PASS")
	{
		std::string	pass;
		ss >> pass;
		if (pass.empty())
		{
			send_msg(c, "461 * PASS :Not enough parameters");
			return;
		}
		if (c.is_registered())
		{
			send_msg(c, "462 * :You may not reregister");
			return;
		}
		if (password != pass)
		{
			send_msg(c, "464 * :Password incorrect");
			return;
		}
		c.set_pass(pass);
	}
	else if (cmd == "NICK")
	{
		std::string	nick;
		ss >> nick;
		if (nick.empty())
		{
			send_msg(c, "461 * NICK :Not enough parameters");
			return;
		}
		if (c.is_registered())
		{
			send_msg(c, "462 * :You may not reregister");
			return;
		}
		c.set_nick(nick);
		c.set_has_nick(true);
	}
	else if (cmd == "USER")
	{
		std::string	user, param1, param2, realname;
		ss >> user >> param1 >> param2;
		std::getline(ss, realname);
		if(realname.empty() || realname[0] != ' ' || realname[1] != ':')
		{
			send_msg(c, "461 * :need more params");
			return;
		}
		realname = realname.substr(2);
		if (user.empty())
		{
			send_msg(c, "461 * USER :Not enough parameters");
			return;
		}
		if (c.is_registered())
		{
			send_msg(c, "462 * :You may not reregister");
			return;
		}
		c.set_user(user);
		c.set_realname(realname);
		c.set_has_user(true);
	}
	if (c.get_has_nick() && c.get_has_user() && !c.is_registered())
	{
		c.set_registered(true);
		welcome_msg(c);
	}
}

void	Server::handle_buff_line(Client& c, const std::string& buff)
{
	c.buffer += buff;
	std::string	del = "\r\n";
	size_t		pos;

	while ((pos = c.buffer.find(del)) != std::string::npos)
	{
		std::string line = c.buffer.substr(0, pos);
		c.buffer.erase(0, pos + del.length());
		handle_line(c, line);
	}
}

void    Server::init_socket()
{
	struct	sockaddr_in server;

	this->socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (this->socket_fd == -1)
	{
		std::cerr << "socket failed!" << std::endl;
		exit(1);
	}
	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	server.sin_addr.s_addr = INADDR_ANY;
	if (bind(this->socket_fd, (struct sockaddr*)&server, sizeof(server)) != 0)
	{
		std::cerr << "bind failed!" << std::endl;
		exit(1);
	}
	if (listen(this->socket_fd, SOMAXCONN) != 0)
	{
		std::cerr << "listen failed!" << std::endl;
		exit(1);
	}
	struct pollfd listener;
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
					std::cout << "A new client just connected!" << std::endl;
				}
			}
			for (long unsigned int i = 1; i < poll_fds.size(); ++i)
			{
				if (poll_fds[i].revents & POLLIN)
				{
					char buff[512];
					ssize_t read_size;
					read_size = recv(poll_fds[i].fd, buff, sizeof(buff) - 1, 0);
					if (read_size <= 0)
					{
						close(poll_fds[i].fd);
						poll_fds.erase(poll_fds.begin() + i);
						--i;
						continue;
					}
					buff[read_size] = '\0';
					
					handle_buff_line(clients[poll_fds[i].fd], buff);
				}
				else if (poll_fds[i].revents & (POLLHUP | POLLERR | POLLNVAL))
				{
					std::cerr << "client error!" << std::endl;
					close(poll_fds[i].fd);
					poll_fds.erase(poll_fds.begin() + i);
					--i;
					continue;
				}
			}
		}
	}
}

