#include "Channel.hpp"

Channel::Channel(const std::string &name) : name(name)
{
	use_key = false;
	invite_only = false;
	topic_set = false;
	limit_set = false;
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
	limit_set = false;
	topic = "";
}

Channel::~Channel()
{
	for (std::vector<Client *>::iterator it = op.begin(); it != op.end(); ++it)
		delete (*it);
	for (std::vector<Client *>::iterator it = invited.begin(); it != invited.end(); ++it)
		delete (*it);
	op.clear();
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
		this->op = f.op;
		this->invited = f.invited;
		this->name = f.name;
		this->topic = f.topic;
		this->key = f.key;
		this->limit = f.limit;
		this->setmodes = f.setmodes;
		this->invite_only = f.invite_only;
		this->use_key = f.use_key;
		this->topic_set = f.topic_set;
		this->limit_set = f.limit_set;
	}
	return (*this);
}

bool Channel::checkKey(const std::string &key)
{
	if (this->key == key)
		return (true);
	return (false);
}

bool Channel::addClient(Client &client)
{
	if (limit_set && invited.size() >= limit)
	{
		std::string msg = ":irc_server 471 "+ client.getNickname() + " :Cannot join channel (+l)\r\n";
		client.getServer().send_message_to_fd(client.getFd(), msg);
		return (false);
	}
	if (invite_only)
	{
		for (std::vector<Client *>::iterator it = invited.begin(); it != invited.end(); ++it)
		{
			if ((*it)->getNickname() != client.getNickname() && it == invited.end() - 1)
			{
				client.getServer().send_message_to_client_with_code(client, "473", name + " :Cannot join channel (+i)");
				return (false);
			}
		}
	}
	Client *new_client = new Client(client);
	invited.push_back(new_client);

	// client.getServer().send_message_to_fd(client.getFd(), ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getServer().getHostname() + " JOIN :" + name + "\r\n");
	client.getServer().send_message_to_channel(this->getName() , ":" + client.getNickname() + "!" + client.getUsername() + "@" + client.getServer().getHostname() + " JOIN " + name + "\r\n");
	client.getServer().send_message_to_channel(name, client.getNickname() + " has joined " + this->name + "\r\n");
	if(!this->topic.empty())
		client.getServer().send_message_to_client_with_code(client, "332", name + " :" + topic);

	std::string userlist;
	for (std::vector<Client *>::iterator it = invited.begin(); it != invited.end(); ++it)
	{
		if (is_operator(**it))
			userlist.append("@" + (*it)->getNickname() + " ");
		else
			userlist.append((*it)->getNickname() + " ");
	}
	client.getServer().send_message_to_client_with_code(client, "353", "= " + name + " :" + userlist);
	client.getServer().send_message_to_client_with_code(client, "366", name + " :End of /NAMES list");

	return (true);
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

bool Channel::is_operator(const Client &client)
{
	for (std::vector<Client *>::iterator it = op.begin(); it != op.end(); ++it)
	{
		if ((*it)->getNickname() == client.getNickname())
			return (true);
	}
	return (false);
}

bool Channel::is_invited(const std::string &nickname)
{
	for (std::vector<Client *>::iterator it = invited.begin(); it != invited.end(); ++it)
	{
		if ((*it)->getNickname() == nickname)
			return (true);
	}
	return (false);
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
	std::cout << "setInviteOnly: " << value << std::endl;
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

void Channel::setOperator(const Client &client)
{
	for (std::vector<Client *>::iterator it = op.begin(); it != op.end(); ++it)
	{
		if ((*it)->getNickname() == client.getNickname())
		{
			return ;
		}
	}
	op.push_back(new Client(client));
}

void Channel::unsetOperator(const Client &client)
{
	for (std::vector<Client *>::iterator it = op.begin(); it != op.end(); ++it)
	{
		if ((*it)->getNickname() == client.getNickname())
		{
			delete (*it);
			op.erase(it);
			return ;
		}
	}
}

void Channel::unsetTopic(void)
{
	this->topic.clear();
	topic_set = false;
}

void Channel::unsetKey(void)
{
	this->key.clear();
	use_key = false;
}

void Channel::unsetLimit(void)
{
	this->limit = 0;
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

std::vector<Client *> &Channel::getInvited(void)
{
	return (invited);
}

std::vector<std::string> &Channel::getSetModes(void)
{
	return (setmodes);
}
