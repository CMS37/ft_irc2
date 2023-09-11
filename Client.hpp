#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Server.hpp"
# include <iostream>
# include <exception>
# include <map>
# include <vector>

class Server;
class Channel;

class Client
{
	private:
		int fd;
		Server &server;
		std::vector<Channel *> joined_channels;
		Channel* channel;

		// std::vector<char> modes;

		bool is_registered;
		bool is_password_allowed;
		
		std::string nickname;
		std::string username;
		std::string hostname;
		std::string servername;
		std::string realname;
		std::string mode;
		std::string message_buffer;

		Client();
	public:
		Client(int fd, Server &server);
		~Client();
		Client(const Client &f);
		Client &operator=(const Client &f);

		void joinChannel(const std::string &name, const std::string &key);
		void deleteJoinedChannel(const std::string &name);
		void quitAll(void);


		int getFd(void) const;
		Server &getServer(void) const;
		bool getIsRegistered(void) const;
		bool getIsPasswordAllowed(void) const;
		std::string getNickname(void) const;
		std::string getUsername(void) const;
		std::string getHostname(void) const;
		std::string getServername(void) const;
		std::string getRealname(void) const;
		std::string getMessageBuffer(void) const;
		Channel* getChannel(void) const;

		void setNickname(const std::string &nickname);
		void setUsername(const std::string &username);
		void setHostname(const std::string &hostname);
		void setServername(const std::string &servername);
		void setRealname(const std::string &realname);
		void setIsRegistered(bool is_registered);
		void setIsPasswordAllowed(bool is_password_allowed);
		void setChannel(Channel* channel);
		void setMessageBuffer(const std::string &message_buffer);
};

#endif