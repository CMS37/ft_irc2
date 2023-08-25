#include "Client.hpp"

Client::Client(int fd, Server &server) : fd(fd), server(server), is_registered(false)
{
}

Client::~Client() {}

Client::Client(const Client &f) : fd(f.fd), server(f.server), is_registered(f.is_registered), nickname(f.nickname)
{
}

Client &Client::operator=(const Client &f)
{
	if (this != &f)
	{
		this->fd = f.fd;
		this->nickname = f.nickname;
		this->server = f.server;
	}
	return (*this);
}

void Client::joinChannel(const std::string &name, const std::string &key)
{
	std::map<std::string, Channel *> &channels = server.getChannels();

	try
	{
		if (channels.find(name) == channels.end())
		{
			Channel *channel = new Channel(name, key);
			channels.insert(std::pair<std::string, Channel *>(name, channel));
		}
		else
		{
			Channel *channel = channels.find(name)->second;
			if (channel->getUseKey() && !channel->checkKey(key))
				throw std::invalid_argument("Wrong key");
		}
		Channel *channel = channels.find(name)->second;
		channel->addClient(*this);
		std::string msg = ":" + this->nickname + " JOIN " + name + "\r\n";
		server.send_message_to_fd(this->fd, msg);
	}
	catch(const std::exception& e)
	{
		std::string msg = ":" + this->nickname + " FAIL JOIN " + name + e.what() + "\r\n";
		server.send_message_to_fd(this->fd, msg);
	}
}

/*//////////////////////////////////////////////////////////////////////////////*/
/*//                                                                          //*/
/*//                                SETTER                                    //*/
/*//                                                                          //*/
/*//////////////////////////////////////////////////////////////////////////////*/

void Client::setNickname(const std::string &nickname)
{
	this->nickname = nickname;
}

void Client::setUsername(const std::string &username)
{
	this->username = username;
}

void Client::setHostname(const std::string &hostname)
{
	this->hostname = hostname;
}

void Client::setServername(const std::string &servername)
{
	this->servername = servername;
}

void Client::setRealname(const std::string &realname)
{
	this->realname = realname;
}

/*//////////////////////////////////////////////////////////////////////////////*/
/*//                                                                          //*/
/*//                                GETTER                                    //*/
/*//                                                                          //*/
/*//////////////////////////////////////////////////////////////////////////////*/

int Client::getFd(void) const
{
	return (this->fd);
}

Server &Client::getServer(void) const
{
	return (this->server);
}

std::string Client::getNickname(void) const
{
	return (this->nickname);
}

bool Client::getIsRegistered(void) const
{
	return (this->is_registered);
}
