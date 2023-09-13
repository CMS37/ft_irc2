#include "../Parser.hpp"

void Parser::cmd_kick()
{
    if (this->_tokens.size() < 3)
        this->_server.send_message_to_client_with_code(this->_client, "461", "KICK :Not enough parameters");
    Channel *channel = this->_server.getChannel(this->_tokens[1]);
    Client *client = this->_server.getClient(this->_tokens[2]);
	if (channel == NULL)
    {
		this->_server.send_message_to_client_with_code(_client, "403", this->_tokens[1] + " :No such channel");
        return ;
    }
    if (client == NULL)
    {
        this->_server.send_message_to_client_with_code(_client, "401", this->_tokens[2] + " :No such nick");
        return ;
    }
    if (!channel->is_invited(client->getNickname()))
    {
        this->_server.send_message_to_client_with_code(_client, "441", this->_tokens[2] + " " + this->_tokens[1] + " :They aren't on that channel");
        return ;
    }
    if (!channel->is_operator(this->_client))
    {
        this->_server.send_message_to_client_with_code(_client, "482", this->_tokens[1] + " :You're not channel operator");
        return ;
    }
    if (client->getNickname() == this->_client.getNickname())
    {
        this->_server.send_message_to_client_with_code(_client, "482", this->_tokens[1] + " :You can't kick yourself");
        return ;
    }
    std::string msg = ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + this->_client.getHostname() +" KICK " + this->_tokens[1] + " " + this->_tokens[2] + " " + this->_tokens[3]+ "\r\n";
    this->_server.send_message_to_channel(this->_tokens[1], msg);
    channel->deleteClient(client->getNickname());
}

// 3.2.8 Kick command

//       Command: KICK
//    Parameters: <channel> *( "," <channel> ) <user> *( "," <user> )
//                [<comment>]

//    The KICK command can be used to request the forced removal of a user
//    from a channel.  It causes the <user> to PART from the <channel> by
//    force.  For the message to be syntactically correct, there MUST be
//    either one channel parameter and multiple user parameter, or as many
//    channel parameters as there are user parameters.  If a "comment" is
//    given, this will be sent instead of the default message, the nickname
//    of the user issuing the KICK.

//    The server MUST NOT send KICK messages with multiple channels or
//    users to clients.  This is necessarily to maintain backward
//    compatibility with old client software.

//    Numeric Replies:

//            ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//            ERR_BADCHANMASK                 ERR_CHANOPRIVSNEEDED
//            ERR_USERNOTINCHANNEL            ERR_NOTONCHANNEL

//    Examples:

//    KICK &Melbourne Matthew         ; Command to kick Matthew from
//                                    &Melbourne

//    KICK #Finnish John :Speaking English
//                                    ; Command to kick John from #Finnish
//                                    using "Speaking English" as the
//                                    reason (comment).

//    :WiZ!jto@tolsun.oulu.fi KICK #Finnish John
//                                    ; KICK message on channel #Finnish
//                                    from WiZ to remove John from channel