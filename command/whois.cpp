#include "../Parser.hpp"

void	Parser::cmd_whois(void)
{
	if (_tokens.size() < 2)
	{
		this->_server.send_message_to_client_with_code(_client, "461", "WHOIS :Not enough parameters");
		return ;
	}

	std::vector<std::string> nick = split(_tokens[1], ',');

	for (size_t i = 0; i < nick.size(); i++ )
	{
		Client *client = this->_server.getClient(nick[i]);
		if (client != NULL)
		{
			std::string msg;
			msg =client->getNickname() + " " + client->getUsername() + " " + client->getServername() + " * :" + client->getRealname();
			this->_server.send_message_to_client_with_code(_client, "311", msg);
                        time_t startTime = _server.getStartTime();
                        std::string time = std::string(ctime(&startTime));
			msg = client->getServer().getHostname() + " :" + time;
			this->_server.send_message_to_client_with_code(_client, "312", msg);
		}
		else
			this->_server.send_message_to_client_with_code(_client, "401", nick[i] + " :No such nick");
                this->_server.send_message_to_client_with_code(_client, "318", nick[i] + " :End of /WHOIS list");
	}
}

/*
4.5.2 Whois query

      Command: WHOIS
   Parameters: [<server>] <nickmask>[,<nickmask>[,...]]

   This message is used to query information about particular user.  The
   server will answer this message with several numeric messages
   indicating different statuses of each user which matches the nickmask
   (if you are entitled to see them).  If no wildcard is present in the
   <nickmask>, any information about that nick which you are allowed to
   see is presented.  A comma (',') separated list of nicknames may be
   given.

   The latter version sends the query to a specific server.  It is
   useful if you want to know how long the user in question has been
   idle as only local server (ie. the server the user is directly
   connected to) knows that information, while everything else is
   globally known.

   Numeric Replies:

           ERR_NOSUCHSERVER                ERR_NONICKNAMEGIVEN
           RPL_WHOISUSER                   RPL_WHOISCHANNELS
           RPL_WHOISCHANNELS               RPL_WHOISSERVER
           RPL_AWAY                        RPL_WHOISOPERATOR
           RPL_WHOISIDLE                   ERR_NOSUCHNICK
           RPL_ENDOFWHOIS

   Examples:

   WHOIS wiz                       ; return available user information
                                   about nick WiZ

   WHOIS eff.org trillian          ; ask server eff.org for user
                                   information about trillian
*/