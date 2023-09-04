#include "../Parser.hpp"

void Parser::cmd_invite()
{
    if (this->_tokens.size() < 3)
        this->_server.send_message_to_client_with_code(this->_client, "461", "INVITE :Not enough parameters");
    Client *client = this->_server.getClient(this->_tokens[1]);
    Channel *channel = this->_server.getChannel(this->_tokens[2]);
    if (channel == NULL)
    {
        this->_server.send_message_to_client_with_code(_client, "403", this->_tokens[2] + ":No such channel");
        return ;
    }
    if (client == NULL)
    {
        this->_server.send_message_to_client_with_code(_client, "401", this->_tokens[1] + ":No such nick");
        return ;
    }
    if (!channel->is_operator(this->_client))
    {
        this->_server.send_message_to_client_with_code(_client, "482", this->_tokens[2] + ":You're not channel operator");
        return ;
    }
    if (channel->is_invited(client->getNickname()))
    {
        this->_server.send_message_to_client_with_code(_client, "443", this->_tokens[2] + " " + this->_tokens[1] + " :is already on channel");
        return ;
    }
    this->_server.send_message_to_client_with_code(_client, "341", this->_tokens[1] + " " + this->_tokens[2] + " :Inviting " + this->_tokens[1] + " to " + this->_tokens[2]);
    try
    {
        channel->addClient(*client);
    }
    catch(const std::exception& e)
    {
        this->_server.send_message_to_fd(this->_client.getFd(), e.what());
        return ;
    }
    std::string msg = ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + this->_server.getHostname() +" INVITE " + this->_tokens[1] + ":" + this->_tokens[2] + "\r\n";
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

// ERR_NEEDMOREPARAMS    461 (command)
// ERR_NOSUCHNICK(nick) (nick + " :No such nick") 401
// ERR_NOTONCHANNEL   (channel)    442
// // ERR443
// # define ERR_USERONCHANNEL (user, channel) (user + " " + channel + " :is already on channel")
// ERR_CHANOPRIVSNEEDED 482 (channel)
// RPL341
// # define RPL_INVITING (channel, nick) (nick + " is inviting you to " + channel)
// RPL_AWAY
