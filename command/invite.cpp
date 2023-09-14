#include "../Parser.hpp"

void Parser::cmd_invite()
{
    if (this->_tokens.size() < 3)
        this->_server.send_message_to_client_with_code(this->_client, "461", "INVITE :Not enough parameters");
    Client *client = this->_server.getClient(this->_tokens[1]);
    Channel *channel = this->_server.getChannel(this->_tokens[2]);
    if (channel == NULL)
    {
        this->_server.send_message_to_client_with_code(_client, "403", this->_tokens[2] + " :No such channel");
        return ;
    }
    if (client == NULL)
    {
        this->_server.send_message_to_client_with_code(_client, "401", this->_tokens[1] + " :No such nick");
        return ;
    }
    if (!channel->is_operator(this->_client))
    {
        this->_server.send_message_to_client_with_code(_client, "482", this->_tokens[2] + " :You're not channel operator");
        return ;
    }
    if (channel->is_invited(client->getNickname()))
    {
        this->_server.send_message_to_client_with_code(_client, "443", this->_tokens[2] + " " + this->_tokens[1] + " :is already on channel");
        return ;
    }

    try
    {
        std::vector<Client *> &inv = channel->getInvited();
        inv.push_back(client);
        this->_server.send_message_to_client_with_code(*client, "341", this->_tokens[2] + " " + this->_tokens[1] + " :has been invited by " + this->_client.getNickname());
        std::string msg = ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + this->_client.getHostname() +" INVITE " + this->_tokens[1] + " :" + this->_tokens[2] + "\r\n";
        this->_server.send_message_to_channel(channel->getName(), msg);
    }
    catch(const std::exception& e)
    {
        this->_server.send_message_to_fd_buffer(this->_client, e.what());
        return ;
    }
}

// 3.2.7 Invite message

//       Command: INVITE
//    Parameters: <nickname> <channel>

//    The INVITE command is used to invite a user to a channel.  The
//    parameter <nickname> is the nickname of the person to be invited to
//    the target channel <channel>.  There is no requirement that the
//    channel the target user is being invited to must exist or be a valid
//    channel.  However, if the channel exists, only members of the channel
//    are allowed to invite other users.  When the channel has invite-only
//    flag set, only channel operators may issue INVITE command.

//    Only the user inviting and the user being invited will receive
//    notification of the invitation.  Other channel members are not
//    notified.  (This is unlike the MODE changes, and is occasionally the
//    source of trouble for users.)

//    Numeric Replies:

//            ERR_NEEDMOREPARAMS              ERR_NOSUCHNICK
//            ERR_NOTONCHANNEL                ERR_USERONCHANNEL
//            ERR_CHANOPRIVSNEEDED
//            RPL_INVITING                    RPL_AWAY

//    Examples:

//    :Angel!wings@irc.org INVITE Wiz #Dust

//                                    ; Message to WiZ when he has been
//                                    invited by user Angel to channel
//                                    #Dust

//    INVITE Wiz #Twilight_Zone       ; Command to invite WiZ to
//                                    #Twilight_zone
