#include "../Parser.hpp"

void Parser::cmd_who(void)
{
	if (_tokens.size() < 2)
	{
		this->_server.send_message_to_client_with_code(_client, "461", "WHO :Not enough parameters");
		return ;
	}

	Channel *channel = this->_server.getChannel(_tokens[1]);
	if (channel == NULL)
	{
		this->_server.send_message_to_client_with_code(_client, "403", _tokens[1] + " :No such channel");
		return ;
	}

	std::string msg;
	msg.append(":" +  _server.getHostname() + " 352 " + _client.getNickname() + " " + _tokens[1]);

	std::vector<Client *> clients = channel->getInvited();
	for (std::vector<Client *>::iterator it = clients.begin(); it != clients.end(); ++it)
	{
		std::string msg2;
		msg2.append(msg + " " + (*it)->getUsername() + " * :" + _server.getHostname() + " " + (*it)->getNickname() + " :0 " + (*it)->getRealname() + "\r\n");
		this->_server.send_message_to_fd_buffer(_client, msg2);
	}
	_server.send_message_to_client_with_code(_client, "315", "* :End of /WHO list");
}

// 4.5.1 Who query

//       Command: WHO
//    Parameters: [<name> [<o>]]

//    The WHO message is used by a client to generate a query which returns
//    a list of information which 'matches' the <name> parameter given by
//    the client.  In the absence of the <name> parameter, all visible
//    (users who aren't invisible (user mode +i) and who don't have a
//    common channel with the requesting client) are listed.  The same
//    result can be achieved by using a <name> of "0" or any wildcard which

// Oikarinen & Reed                                               [Page 33]
// RFC 1459              Internet Relay Chat Protocol              May 1993

//    will end up matching every entry possible.

//    The <name> passed to WHO is matched against users' host, server, real
//    name and nickname if the channel <name> cannot be found.

//    If the "o" parameter is passed only operators are returned according
//    to the name mask supplied.

//    Numeric Replies:

//            ERR_NOSUCHSERVER
//            RPL_WHOREPLY                    RPL_ENDOFWHO

//    Examples:

//    WHO *.fi                        ; List all users who match against
//                                    "*.fi".

//    WHO jto* o                      ; List all users with a match against
//                                    "jto*" if they are an operator.