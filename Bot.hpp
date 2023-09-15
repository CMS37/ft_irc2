#ifndef BOT_HPP
# define BOT_HPP

#include "Server.hpp"

class Server;
class Client;
class Parser;
class Channel;

class Bot
{
	private:
		std::vector<std::string> banlist; 
		std::string name;
		std::string username;
		std::string hostname;
		std::string realname;

	public:
		Bot();
		~Bot();

		void addban(const std::string& ban);
		bool checkmessage(const std::string& msg);
};

#endif