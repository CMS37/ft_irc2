#include "Channel.hpp"

Channel::Channel(const std::string &name) : name(name)
{
	use_key = false;
	invite_only = false;
	topic_set = false;
	limit = false;
	topic = "";
}

Channel::Channel(const std::string &name, const std::string &key) : name(name), key(key)
{
	if (!key.empty())
		use_key = true;
	else
		use_key  = false;
	invite_only = false;
	topic_set = false;
	limit = false;
	topic = "";
}

Channel::~Channel()
{
	for (std::vector<Client *>::iterator it = invited.begin(); it != invited.end(); ++it)
		delete (*it);
	invited.clear();
}

Channel::Channel(const Channel &f)
{
	*this = f;
}

Channel &Channel::operator=(const Channel &f)
{
	if (this != &f)
	{
	}
	return (*this);
}

bool Channel::checkKey(const std::string &key)
{
	if (this->key == key)
		return (true);
	return (false);
}

void Channel::addClient(const Client &client)
{
	if (limit_set && invited.size() >= limit)
		throw std::invalid_argument("Channel is full");
	if (invite_only)
	{
		for (std::vector<Client *>::iterator it = invited.begin(); it != invited.end(); ++it)
		{
			if ((*it)->getNickname() == client.getNickname())
				throw std::invalid_argument("You are already invited");
		}
	}
	Client *new_client = new Client(client);
	invited.push_back(new_client);
}


void Channel::deleteClient(const std::string &nickname)
{
	for (std::vector<Client *>::iterator it = invited.begin(); it != invited.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
		{
			delete (*it);
			invited.erase(it);
			return ;
		}
	}
}

void Channel::printInfo(void)
{
	std::cout << "Channel name: " << name << std::endl;
	if (this->topic_set)
		std::cout << "Channel topic: " << topic << std::endl;
	if (this->limit_set)
		std::cout << "Channel limit: " << limit << std::endl;
	std::cout << "Channel invited: " << invited.size() << std::endl;
	for (std::vector<Client *>::iterator it = invited.begin(); it != invited.end(); ++it)
		std::cout << (*it)->getNickname() << std::endl;
}

/*//////////////////////////////////////////////////////////////////////////////*/
/*//                                                                          //*/
/*//                                SETTER                                    //*/
/*//                                                                          //*/
/*//////////////////////////////////////////////////////////////////////////////*/

void Channel::setTopic(const std::string &topic)
{
	this->topic = topic;
	topic_set = true;
}

void Channel::setInviteOnly(bool value)
{
	invite_only = value;
}

void Channel::setKey(const std::string &key)
{
	this->key = key;
	use_key = true;
}

void Channel::setLimit(size_t limit)
{
	this->limit = limit;
	limit_set = true;
}

void Channel::unsetTopic(void)
{
	topic_set = false;
}

void Channel::unsetKey(void)
{
	use_key = false;
}

void Channel::unsetLimit(void)
{
	limit_set = false;
}

/*//////////////////////////////////////////////////////////////////////////////*/
/*//                                                                          //*/
/*//                                GETTER                                    //*/
/*//                                                                          //*/
/*//////////////////////////////////////////////////////////////////////////////*/

std::string Channel::getName(void) const
{
	return (name);
}

std::string Channel::getTopic(void) const
{
	return (topic);
}

std::string Channel::getKey(void) const
{
	return (key);
}

bool Channel::getInviteOnly(void) const
{
	return (invite_only);
}

bool Channel::getTopicSet(void) const
{
	return (topic_set);
}

bool Channel::getLimitSet(void) const
{
	return (limit_set);
}

bool Channel::getUseKey(void) const
{
	return (use_key);
}

std::vector<Client *> Channel::getInvited(void) const
{
	return (invited);
}
