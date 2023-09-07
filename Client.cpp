#include "Client.hpp"

Client::Client(int fd, Server &server) : fd(fd), server(server), channel(NULL), is_registered(false), is_password_allowed(false)
{
}

Client::~Client()
{

}

Client::Client(const Client &f) : server(f.server)
{
	*this = f;
}

Client &Client::operator=(const Client &f)
{
	if (this != &f)
	{
		this->fd = f.fd;
		this->server = f.server;
		this->joined_channels = f.joined_channels;
		this->channel = f.channel;
		this->is_registered = f.is_registered;
		this->is_password_allowed = f.is_password_allowed;
		this->nickname = f.nickname;
		this->username = f.username;
		this->hostname = f.hostname;
		this->servername = f.servername;
		this->realname = f.realname;
		this->mode = f.mode;
		this->message = f.message;
	}
	return (*this);
}

void Client::joinChannel(const std::string &name, const std::string &key)
{
	std::map<std::string, Channel *> &channels = server.getChannels();

	if (channels.find(name) == channels.end())
	{
		Channel *channel = new Channel(name, key);
		channel->addClient(*this);
		channel->setOperator(*this);
		channels.insert(std::pair<std::string, Channel *>(name, channel));
		joined_channels.push_back(channel);
	}
	else
	{
		Channel *channel = channels.find(name)->second;
		if (channel->getUseKey() && !channel->checkKey(key))
			this->server.send_message_to_client_with_code(*this, "475", name + " :Cannot join channel (+k)");
		else
		{
			if (channel->addClient(*this))
			{
				std::vector<Channel *>::iterator it = std::find(joined_channels.begin(), joined_channels.end(), channel);
				if (it == joined_channels.end())
					joined_channels.push_back(channel);
			}
		}
	}
}

void Client::deleteJoinedChannel(const std::string &name)
{
	std::vector<Channel *>::iterator it;
	for (it = joined_channels.begin(); it != joined_channels.end(); it++)
	{
		if ((*it)->getName() == name)
		{
			joined_channels.erase(it);
			break;
		}
	}
}


void Client::quitAll(void)
{
	std::vector<Channel *>::iterator it;
	for (it = joined_channels.begin(); it != joined_channels.end(); it++)
	{
		(*it)->deleteClient(this->nickname);
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

void Client::setIsRegistered(bool is_registered)
{
	this->is_registered = is_registered;
}

void Client::setIsPasswordAllowed(bool is_password_allowed)
{
	this->is_password_allowed = is_password_allowed;
}

void Client::setChannel(Channel* channel)
{
	this->channel = channel;
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

std::string Client::getUsername(void) const
{
	return (this->username);
}

std::string Client::getHostname(void) const
{
	return (this->hostname);
}

std::string Client::getServername(void) const
{
	return (this->servername);
}

std::string Client::getRealname(void) const
{
	return (this->realname);
}

bool Client::getIsPasswordAllowed(void) const
{
	return (this->is_password_allowed);
}

bool Client::getIsRegistered(void) const
{
	return (this->is_registered);
}

Channel* Client::getChannel(void) const
{
	return (this->channel);
}


