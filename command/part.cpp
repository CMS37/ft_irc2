#include "../Parser.hpp"

void Parser::cmd_part()
{
    if(!this->_client.getIsRegistered())
        this->_server.send_message_to_fd_buffer(this->_client, "451 :You have not registered\n");
    if(this->_tokens.size() != 2)
    {
        this->_server.send_message_to_client_with_code(this->_client, "461", "PART :Not enough parameters");
        return;
    }

    Channel *channel = this->_server.getChannel(this->_tokens[1]);
    if (channel == NULL)
    {
        this->_server.send_message_to_client_with_code(this->_client, "403", this->_tokens[1] + " :No such channel");
        return ;
    }
    if (!channel->is_invited(this->_client.getNickname()))
    {
        this->_server.send_message_to_client_with_code(this->_client, "442", this->_tokens[1] + " :You're not on that channel");
        return ;
    }

    std::string msg = ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + this->_client.getHostname() + " PART " + this->_tokens[1] + "\r\n";
    this->_server.send_message_to_channel(this->_tokens[1], msg);
    this->_server.send_message_to_client_with_code(this->_client, "442", this->_tokens[1] + " :Left channel");
}


// 3.2.2 Part message

//       Command: PART
//    Parameters: <channel> *( "," <channel> ) [ <Part Message> ]

//    The PART command causes the user sending the message to be removed
//    from the list of active members for all given channels listed in the
//    parameter string.  If a "Part Message" is given, this will be sent
//    instead of the default message, the nickname.  This request is always
//    granted by the server.

//    Servers MUST be able to parse arguments in the form of a list of
//    target, but SHOULD NOT use lists when sending PART messages to
//    clients.

//    Numeric Replies:

//            ERR_NEEDMOREPARAMS              ERR_NOSUCHCHANNEL
//            ERR_NOTONCHANNEL

//    Examples:

//    PART #twilight_zone             ; Command to leave channel
//                                    "#twilight_zone"

//    PART #oz-ops,&group5            ; Command to leave both channels
//                                    "&group5" and "#oz-ops".

//    :WiZ!jto@tolsun.oulu.fi PART #playzone :I lost
//                                    ; User WiZ leaving channel
//                                    "#playzone" with the message "I
//                                    lost".
