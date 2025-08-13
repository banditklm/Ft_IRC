#include "../headers/server.hpp"

bool nick_exist (std::map<int, Client> clients, std::string nick)
{
	for (size_t i =0; i < clients.size(); i++)
	{
		if (clients[i].get_nick() == nick)
			return true;
	}
	return false;
}

bool	check_realname(std::string realname)
{
	if (realname.length() < 2)
		return (false);
	return true;
}

void Server::auth(Client &c, std::vector<std::string> cmd)
{
	if (cmd[0] == "PASS")
	{
		if (cmd.size() != 2)
		{
			send_msg(c, "461 * PASS :Syntax error\r\n");
			return;
		}
		std::string	pass;
		pass = cmd[1];
		if (pass.empty())
		{
			send_msg(c, "461 * PASS :Not enough parameters\r\n");
			return;
		}
		if (password != pass)
		{
			send_msg(c, "464 * PASS :Password incorrect\r\n");
			return;
		}
		if (c.get_has_pass())
		{
			send_msg(c, "462 * PASS :You may not reregister\r\n");
			return;
		}
		if (c.is_registered())
		{
			send_msg(c, "462 * PASS :You may not reregister\r\n");
			return;
		}
		c.set_pass(pass);
		c.set_has_pass(true);
	}
	else if (cmd[0] == "NICK")
	{
		if(!c.get_has_pass())
		{
			send_msg(c, "ERROR :You must enter the password first\r\n");
			return;
		}
		if (cmd.size() != 2)
		{
			send_msg(c, "461 * NICK :Syntax error\r\n");
			return;
		}
		if (cmd[1][0] == '#')
		{
			send_msg(c, "461 * NICK :Syntax error\r\n");
			return ;
		}
		std::string	nick;
		nick = cmd[1];
		if (nick.empty())
		{
			send_msg(c, "461 * NICK :Not enough parameters\r\n");
			return;
		}
		if (nick_exist(clients, nick))
		{
			send_msg(c, nick + " 433 * NICK :Nickname is already in use\r\n");
			return;
		}
		if (c.is_registered())
		{
			send_msg(c, "462 * NICK :You may not reregister\r\n");
			return;
		}
		c.set_nick(nick);
		c.set_has_nick(true);
	}
	else if (cmd[0] == "USER")
	{
		if (cmd.size() < 5 || cmd[4].empty() || cmd[4][0] != ':' || !check_realname(cmd[4]))
		{
			send_msg(c, "461 * USER :Syntax error\r\n");
			return;
		}
		if(!c.get_has_pass())
		{
			send_msg(c, "ERROR :You must enter the password first\r\n");
			return;
		}
		if (cmd.size() < 5)
		{
			send_msg(c, "461 * USER :Syntax error\r\n");
			return;
		}
		std::string user = cmd[1];
		std::string hostname = cmd[2];
		std::string servername = cmd[3];
		std::string realname;
		for (size_t i = 4; i < cmd.size(); ++i)
		{
			realname += cmd[i];
			if (i != cmd.size() - 1)
				realname += " ";
		}
		if(realname.empty())
		{
			send_msg(c, "461 * USER :need more params\r\n");
			return;
		}
		realname = realname.substr(2);
		if (user.empty())
		{
			send_msg(c, "461 * USER :Not enough parameters\r\n");
			return;
		}
		if (c.is_registered())
		{
			send_msg(c, "462 * USER :You may not reregister\r\n");
			return;
		}
		c.set_user(user);
		c.set_realname(realname);
		c.set_has_user(true);
	}
	
}


std::string up(std::string in)
{
    std::string out(in.size(), ' ');
    for (size_t i = 0; i < in.size(); i++)
    {
        out[i] = toupper(in[i]);
    }
    return out;
}

void Server::execute_cmd(Client &c, std::vector<std::string> cmdList)
{
	if (!cmdList.size())
		return;
	std::string cmd = cmdList[0];
	cmd = up(cmd);
	cmdList[0] = up(cmdList[0]);
	

	static int i;
	if(cmd == "LOG")
	{
		i++;
		c.set_has_nick(true);
		c.set_has_user(true);
		c.set_has_pass(true);
		c.set_registered(true);
		c.set_nick("nick" + std::to_string(i));
		c.set_pass("pass");
		c.set_user("user" + std::to_string(i));
		c.set_realname("name" + std::to_string(i));
		std::cout << "default log for testing ..." << std::endl;
		return;
	}

	if (cmd == "PASS" || cmd == "USER" || cmd == "NICK")
	{
		auth(c, cmdList);
		if (c.get_has_pass() && c.get_has_nick() && c.get_has_user() && !c.is_registered())
		{
			c.set_registered(true);
			welcome_msg(c);
		}
	}
	else if (!c.is_registered())
	{
		send_msg(c, "451 * :You have not registered\n");
		return ;
	}
	else if (cmd == "JOIN")
	{
		ft_join(cmdList,  c);
	}
	else if (cmd == "MODE")
	{
		ft_mode(cmdList,  c);
	}
	else if (cmd == "INVITE")
	{
		ft_invite(cmdList, c);
	}
	else if (cmd == "KICK")
	{
		ft_kick(cmdList,  c);
	}
	else if (cmd == "PRIVMSG")
	{
		ft_privmsg(cmdList,  c);
	}
	else if (cmd == "TOPIC")
	{
		ft_topic(cmdList,  c);
	}
	else if (cmd == "HELP")
	{
		help(&c);
	}
	else if (cmd == "BOT")
	{
		bot(&c, cmdList);
	}
	else
	{
		send_msg(c, std::string("COMMAND NOT FOUND\r\n"));
	}
}
