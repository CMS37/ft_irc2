#ifndef SERVER_HPP
# define SERVER_HPP

# include "Client.hpp"
# include "Channel.hpp"
# include "Parser.hpp"
# include <iostream>
# include <algorithm>
# include <cstring>
# include <cstdlib>
# include <vector>
# include <map>
# include <exception>
# include <poll.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <netinet/ip.h>
# include <csignal>
# include <ctime>

class Client;
class Channel;
class Parser;

class Server
{
	private:
		int fd;
		int port;
		struct sockaddr_in addr;
		struct timeval tv;
		time_t startTime;
		std::vector<struct pollfd> fds;
		std::map<int, Client *> clients;
		std::map<std::string, Channel *> channels;
		std::string password;

		const std::string hostname;

	public:	
		Server();
		~Server();
		Server(const Server &f);
		Server &operator=(const Server &f);

		void	input(int argc, char **argv);
		void	run(void);
		void	init_pollfd(void);
		void	accept_client(void);
		void	read_client_data(size_t i);
		void	client_disconnect_from_channel(Client* cli);
		void	client_disconnect(size_t i);
		void	showChannels(void);
		void	addChannel(const std::string &name, const std::string &key);
		void	deleteChannel(const std::string &name);
		void	setStartTime(void);
		void	registrate(Client &cli);
		std::vector<std::string>	parse_recieved(std::string &sbuf, Client &cli);

		bool	check_nickname(const std::string &nickname);
		bool	check_channel(const std::string &channel);
		
		static void setRUN(int signo);
		void setTopic(const std::string &channelName, const std::string &topic);

		int		getFd(void) const;
		int		getPort(void) const;
		time_t	getStartTime(void) const;
		std::string	getPassword(void) const;
		std::vector<struct pollfd> getFds(void) const;
		std::map<int, Client *> getClients(void) const;
		std::map<std::string, Channel *> &getChannels(void);
		Client *getClient(const std::string &name);
		Channel *getChannel(const std::string &name);
		Channel &getChannelset(const std::string &name);
		std::string getHostname(void) const;

		void send_message_to_fd_buffer(Client &cli, std::string message);
		void send_message_to_channel(std::string channel, std::string message);
		void send_message_to_channel_except_myself(Client &cli, std::string channel_name, std::string message);
		void send_message_to_channel_with_code(std::string channel_name, const Client &cli, std::string code, std::string message);
		void send_message_to_client_with_code(Client &cli, std::string code, std::string message);
		void send_system_message(Client cli, std::string msg);
		
		void send_message(int fd);
		//server time
		//send
};

#endif