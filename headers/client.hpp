#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <iostream>
#include <vector>

class Client
{
	private :
	int			fd;
	std::string	nickname;
	std::string	username;
	std::string buffer;
	bool		registered;
	std::vector<std::string> joinedChannels;

	public :
	Client(int fd);
	~Client();
	
};

#endif