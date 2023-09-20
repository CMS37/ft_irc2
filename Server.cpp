#include "Server.hpp"
#include "utils.hpp"
#include <cstdio>

bool running;

Server::Server(): hostname("irc.42.kr")
{

}

Server::~Server()
{
	for (size_t i = 0; i < fds.size(); i++)
		close(fds[i].fd);
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
		delete it->second;
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		delete it->second;
	clients.clear();
	channels.clear();
	std::cout << "Server is down" << std::endl;
}

Server::Server(const Server &f)
{
	*this = f;
}

Server &Server::operator=(const Server &f)
{
	if(this != &f)
	{
		this->fd = f.fd;
		this->port = f.port;
		this->password = f.password;
		this->fds = f.fds;
	}
	return (*this);
}

void Server::input(int argc, char **argv)
{
	if (argc != 3)
		throw std::invalid_argument("Input <port> <password>");
	else
	{
		this->port = atoi(argv[1]);
		if (this->port < 6660 || this->port > 6669)
			throw std::invalid_argument("port number is invalid");
		this->password = argv[2];
	}
}

void Server::init_pollfd(void)
{
	struct pollfd pfd;

	pfd.fd = this->fd;
	pfd.events = POLLIN;
	pfd.revents = 0;
	fds.push_back(pfd);
}

void Server::accept_client(void)
{
	int client_fd;
    struct sockaddr_in client_addr;
    socklen_t len;

    len = sizeof(client_addr);
    client_fd = accept(fd, (struct sockaddr *)&client_addr, &len);
    if (client_fd == -1)
		std::cerr << "accept client error" << std::endl;
	else
	{
		std::cout << "Accept client_fd: " << client_fd << std::endl;
		if (fcntl(client_fd, F_SETFL, O_NONBLOCK) == -1)
		{
			std::cerr << "fcntl error" << std::endl;
			close(client_fd);
		}
		else
		{
			struct pollfd new_fd;
			
			new_fd.fd = client_fd;
			new_fd.events = POLLIN | POLLOUT;
			new_fd.revents = 0;
			fds.push_back(new_fd);

			Client *client = new Client(client_fd, *this);
			clients.insert(std::pair<int, Client *>(client_fd, client));
		}
	}
}

void Server::client_disconnect_from_channel(Client* cli)
{
	std::map<std::string, Channel *>::iterator it;

	for(it = this->channels.begin(); it != this->channels.end(); it++)
	{
		it->second->deleteClient(cli->getNickname());
	}
}

void Server::client_disconnect(size_t i)
{
	int fd = fds[i].fd;

	delete clients[fd];
	clients.erase(fd);
	close(fd);
	fds.erase(fds.begin() + i);
}

std::vector<std::string> Server::parse_recieved(std::string &sbuf, Client &cli)
{
	std::string str = cli.getMessageBuffer() + sbuf;
	cli.setMessageBuffer("");
	std::vector<std::string> lines = split(str, '\n');

	if(sbuf.substr(sbuf.length() - 2, 2) != "\r\n")
	{
		cli.setMessageBuffer(lines.back());
		lines.pop_back();
	}
	return (lines);
}

void Server::read_client_data(size_t i)
{
	char buf[1024];
	bzero(buf, sizeof(buf));
	ssize_t bytes_read = recv(fds[i].fd, buf, sizeof(buf) - 1, 0);
	buf[bytes_read] = '\0';

	if (bytes_read == -1)
		std::cerr << "recv error" << std::endl;
	else if (bytes_read == 0)
	{
		std::cout << "client disconnected" << std::endl;
		client_disconnect(i);
	}
	else
	{
		std::string sbuf(buf);
		std::vector<std::string> lines = parse_recieved(sbuf, *clients[fds[i].fd]);

		std::cout << "================================\n";
		std::cout << "     <Recvieced string>\n" << sbuf << std::endl;
		std::cout << "================================\n";

		for(size_t j = 0; j < lines.size(); j++)
		{
			std::cout << "line: " << lines[j] << std::endl;
			Parser parser(*this , *clients[fds[i].fd], lines[j]);
			parser.what_is_this();
		}
	}
}

void Server::addChannel(const std::string &name, const std::string &key)
{
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->first == name)
			throw std::invalid_argument("Channel name already exists");
	}
	Channel *channel = new Channel(name, key);
	channels.insert(std::pair<std::string, Channel *>(name, channel));
}

void Server::deleteChannel(const std::string &name)
{
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->first == name)
		{
			delete it->second;
			channels.erase(it);
			return ;
		}
	}
	throw std::invalid_argument("Channel name does not exist");
}

void Server::showChannels(void)
{
	std::cout << "==Channel list==" << std::endl;
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
		std::cout << it->first << std::endl;
}

bool Server::check_nickname(const std::string &nickname)
{
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second->getNickname() == nickname)
			return (true);
	}
	return (false);
}

bool Server::check_channel(const std::string &channel)
{
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (!strncmp(it->first.c_str(), channel.c_str(),channel.length()))
			return (true);
	}
	return (false);
}

void signal_handler(int signum)
{
	if (signum == SIGINT)
		running = false;
	std::cout << signum << " signal received" << std::endl;
}

void Server::run(void)
{
	std::cout << "Server is running..." << std::endl;
	setStartTime();
	fd = socket(AF_INET, SOCK_STREAM, 0); // (도매인)AF_INET:IPv4, (타입)SOCK_STREAM:TCP, (프로토콜)0:기본 
	if (fd == -1)
		throw std::runtime_error("socket error");
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port); // 포트번호
	addr.sin_addr.s_addr = INADDR_ANY; // IP주소(INADDR_ANY: 모든 IP로부터의 연결을 허용(htol으로 변환x))
	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &addr, sizeof(addr)) == -1)
		throw std::runtime_error("socket:Cant reuse address");
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("socket:blocking error");
	if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		throw std::runtime_error("socket:bind error");
	if (listen(fd, 10) == -1)
		throw std::runtime_error("socket:listen error");
	init_pollfd();
	std::cout << "Server is ready" << std::endl;
	signal(SIGINT, signal_handler);
	running = true;
	while (running)
	{
		if (poll(&fds[0], fds.size(), -1) == -1) // (배열의시작주소, 배열의크기, 타임아웃(-1:무한대기)))
			std::cerr << "poll error" << std::endl;
		for (size_t i = 0; i < fds.size(); i++)
		{
			if (fds[i].revents & POLLIN) // 이벤트 발생
			{
				if (fds[i].fd == this->fd) // 새로운 클라이언트 접속 요청
					accept_client();
				else //기존 클라이언트 이벤트 발생
					read_client_data(i);
			}
			else if(fds[i].revents & POLLOUT)
			{
				send_message(fds[i].fd);
			}
		}
	}
}

void	Server::registrate(Client &cli)
{
	if(!cli.getNickname().empty()
	&& !cli.getUsername().empty()
	&& !cli.getRealname().empty()
	&& cli.getIsPasswordAllowed()
	&& !cli.getIsRegistered())
	{
		cli.setIsRegistered(true);
		send_message_to_client_with_code(cli, "001", ":Welcome to the Internet Relay Network " + cli.getNickname() + "!" + cli.getUsername() + "@" + cli.getHostname());
		send_message_to_client_with_code(cli, "002", ":Your host is " + this->hostname + ", running version 1.0");
		send_message_to_client_with_code(cli, "003", ":This server was created sometime");
	}
}


/*//////////////////////////////////////////////////////////////////////////////*/
/*//                                                                          //*/
/*//                                SETTER                                    //*/
/*//                                                                          //*/
/*//////////////////////////////////////////////////////////////////////////////*/

void Server::setTopic(const std::string &channelName, const std::string &topic)
{
	Channel *channel = this->channels[channelName];
	if (channel->getTopicSet())
	{
		std::cout << "Topic changed" << std::endl;
		std::cout << "Before: " << channel->getTopic() << std::endl;
	}
	channel->setTopic(topic);
	std::cout << "After: " << channel->getTopic() << std::endl;
}

void Server::setStartTime(void)
{
	time(&startTime);
	tv.tv_sec = 0;
	tv.tv_usec = 0;
}

/*//////////////////////////////////////////////////////////////////////////////*/
/*//                                                                          //*/
/*//                                GETTER                                    //*/
/*//                                                                          //*/
/*//////////////////////////////////////////////////////////////////////////////*/

int	Server::getFd(void) const
{
	return (this->fd);
}

int Server::getPort(void) const
{
	return (this->port);
}

std::string Server::getPassword(void) const
{
	return (this->password);
}

std::vector<struct pollfd> Server::getFds(void) const
{
	return (this->fds);
}

std::map<int, Client *> Server::getClients(void) const
{
	return (this->clients);
}

std::map<std::string, Channel *> &Server::getChannels(void)
{
	return (this->channels);
}

time_t Server::getStartTime(void) const
{
	return (this->startTime);
}

std::string Server::getHostname(void) const
{
	return (this->hostname);
}

Channel *Server::getChannel(const std::string &name)
{
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->first == name)
			return (it->second);
	}
	return (NULL);
}

Channel &Server::getChannelset(const std::string &name)
{
	for (std::map<std::string, Channel *>::iterator it = channels.begin(); it != channels.end(); ++it)
	{
		if (it->first == name)
			return (*it->second);
	}
	return (*channels.begin()->second);
}

Client *Server::getClient(const std::string &nickname)
{
	for (std::map<int, Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		if (it->second->getNickname() == nickname)
			return (it->second);
	}
	return (NULL);
}

/*//////////////////////////////////////////////////////////////////////////////*/
/*//                                                                          //*/
/*//                                SENDER                                    //*/
/*//                                                                          //*/
/*//////////////////////////////////////////////////////////////////////////////*/

void Server::send_message_to_fd_buffer(Client &cli, std::string message)
{
	std::cout << ">>>message_sent_to_fd\n" << message << std::endl;
	// send(fd, message.c_str(), message.length(), 0);
	
	cli.append_send_buffer(message);
	// this->fds[cli.getFd()].events |= POLLOUT;
}

void Server::send_message_to_client_with_code(Client &cli, std::string code, std::string message)
{
	std::string nick;
	if (cli.getNickname() == "")
		nick = "*";
	else
		nick = cli.getNickname();
	std::string msg = ":" + hostname + " " + code + " " + nick + " " + message + "\r\n";
	send_message_to_fd_buffer(cli, msg);
}

void Server::send_message_to_channel_with_code(std::string channel_name, const Client &cli, std::string code, std::string message)
{
	std::string msg = ":" + hostname + " " + code + " " + cli.getNickname() + " " + message + "\r\n";
	send_message_to_channel(channel_name, msg);
}

void Server::send_message_to_channel(std::string channel_name, std::string message)
{
	std::cout << "@@@@message_sent_to_channel->\n" << std::endl;
	std::map<std::string, Channel *>::iterator cha_it = this->channels.find(channel_name);
	
	if (cha_it == this->channels.end())
	{
		return ;
	}
	std::vector<Client *> invited = cha_it->second->getInvited();
	for(std::vector<Client *> ::iterator cli_it = invited.begin(); cli_it != invited.end(); cli_it++)
	{
		send_message_to_fd_buffer((**cli_it), message);
	}
}

void Server::send_message_to_channel_except_myself(Client &cli, std::string channel_name, std::string message)
{
	std::cout << "@@@@message_sent_to_channel_except_myself->\n" << std::endl;
	std::map<std::string, Channel *>::iterator cha_it = this->channels.find(channel_name);
	
	if (cha_it == this->channels.end())
	{
		return ;
	}
	std::vector<Client *> invited = cha_it->second->getInvited();
	for(std::vector<Client *>::iterator cli_it = invited.begin(); cli_it != invited.end(); cli_it++)
	{
		if((*cli_it)->getFd() != cli.getFd())
			send_message_to_fd_buffer(**cli_it, message);
	}
}

void Server::send_system_message(Client cli, std::string msg)
{
	std::string message = ":" + this->hostname + "!" + this->hostname + "@:" + this->hostname + " PRIVMSG " + cli.getChannel()->getName() + " :" + msg + "\r\n";
	send_message_to_fd_buffer(cli, message);
}

void Server::send_message(int fd)
{
	Client *cli = this->clients[fd];

	if (cli == NULL)
		return ;
	std::string message = cli->get_send_buffer_and_flush();
	if(message.empty())
		return ;
	send(fd, message.c_str(), message.length(), 0);
}
