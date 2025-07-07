#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <cstdlib>

class Client
{
	public :
	int			fd;
	std::string	nickname;
	std::string	username;
	std::string	password;
	std::string buffer;
	bool		has_nick;
	bool		has_user;
	bool		registered;
	std::vector<std::string> joinedChannels;

	Client();
	Client(int fd);
	~Client();
	void	set_nick(std::string nickname);
	void	set_user(std::string username);
	void	set_pass(std::string password);
	
};

#endif