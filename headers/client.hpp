#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>
#include <cstdlib>

class Client
{
	private :
	int			fd;
	std::string	nickname;
	std::string	username;
	std::string buffer;
	bool		has_nick;
	bool		has_user;
	bool		registered;
	std::vector<std::string> joinedChannels;

	public :
	Client();
	Client(int fd);
	~Client();
	void	set_nick(std::string nickname);
	void	set_user(std::string username);
	
};

#endif