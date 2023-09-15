#include "Bot.hpp"

Bot::Bot() : name("bot"), username("bot"), hostname("bot"), realname("bot")
{

}

Bot::~Bot()
{
	banlist.clear();
}

void Bot::addban(const std::string& ban)
{
	for (unsigned int i = 0; i < banlist.size(); i++)
	{
		if (banlist[i] == ban)
			return;
	}
	banlist.push_back(ban);
}

bool Bot::checkmessage(const std::string& msg)
{
	for (unsigned int i = 0; i < banlist.size(); i++)
	{
		if (msg.find(banlist[i]) != std::string::npos)
			return (false);
	}
	return (true);
}

/*

!bot create
!bot destroy

!ban <word>

!list ban


*/