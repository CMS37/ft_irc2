#include "../Parser.hpp"

void Parser::cmd_privmsg()
{
    if(!this->_client.getIsRegistered())
    {
        this->_server.send_message_to_fd_buffer(this->_client, "451 :You have not registered\n");
        return;
    }
    std::string target_channel = this->_tokens[1];
    std::string message;
    Channel &channel = this->_server.getChannelset(target_channel);

    for(unsigned int i = 2; i < this->_tokens.size(); i++)
    {
        if (channel.is_bot())
        {
            if (this->_tokens[2] == ":!ban" && i != 2)
            {
                channel.getBot()->addban(this->_tokens[i]);
                std::string msg = ":bot!bot@bot PRIVMSG " + target_channel + " :Ban " + this->_tokens[i] + "\r\n";
                this->_server.send_message_to_channel(target_channel, msg);
            }
            else if (!channel.getBot()->checkmessage(this->_tokens[i]))
            {
                message += "*";
                for (unsigned int j = 0; j < this->_tokens[i].size(); j++)
                    message += "*";
                message += "*";
            }
            else
                message += this->_tokens[i];
        }
        else
            message += this->_tokens[i];
        if(i < this->_tokens.size() - 1)
            message += " ";
    }
    Client* target_client = this->_server.getClient(target_channel);
    if(target_client)
    {
        this->_server.send_message_to_fd_buffer(*target_client, ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + this->_client.getHostname() + " PRIVMSG " + target_channel + " " + message + "\r\n");
        return;
    }

    this->_server.send_message_to_channel_except_myself(this->_client, target_channel, 
    ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + this->_client.getHostname() + " PRIVMSG " + target_channel + " " + message + "\r\n");

    if (message == ":!bot create")
    {
        if (channel.is_operator(this->_client) == false)
        {
            this->_server.send_message_to_fd_buffer(this->_client, "481 :Permission Denied- You're not an IRC operator\r\n");
            return;
        }
        if (channel.is_bot())
        {
            this->_server.send_message_to_fd_buffer(this->_client, "403 :Bot already exists\r\n");
            return;
        }
        channel.setBot();
        std::string msg = ":bot!bot@bot PRIVMSG " + target_channel + " :Ban bot created\r\n";
        this->_server.send_message_to_channel(target_channel, msg);
    }
    else if (message == ":!bot destroy")
    {
        if (channel.is_operator(this->_client) == false)
        {
            this->_server.send_message_to_fd_buffer(this->_client, "481 :Permission Denied- You're not an IRC operator\r\n");
            return;
        }
        if (channel.is_bot() == false)
        {
            this->_server.send_message_to_fd_buffer(this->_client, "403 :Bot does not exist\r\n");
            return;
        }
        channel.unsetBot();
        std::string msg = ":bot!bot@bot PRIVMSG " + target_channel + " :Ban bot destroyed\r\n";
        this->_server.send_message_to_channel(target_channel, msg);
    }
}

// 3.3.1 Private messages

//       Command: PRIVMSG
//    Parameters: <msgtarget> <text to be sent>

//    PRIVMSG is used to send private messages between users, as well as to
//    send messages to channels.  <msgtarget> is usually the nickname of
//    the recipient of the message, or a channel name.

//    The <msgtarget> parameter may also be a host mask (#<mask>) or server
//    mask ($<mask>).  In both cases the server will only send the PRIVMSG
//    to those who have a server or host matching the mask.  The mask MUST
//    have at least 1 (one) "." in it and no wildcards following the last
//    ".".  This requirement exists to prevent people sending messages to
//    "#*" or "$*", which would broadcast to all users.  Wildcards are the
//    '*' and '?'  characters.  This extension to the PRIVMSG command is
//    only available to operators.

//    Numeric Replies:

//            ERR_NORECIPIENT                 ERR_NOTEXTTOSEND
//            ERR_CANNOTSENDTOCHAN            ERR_NOTOPLEVEL
//            ERR_WILDTOPLEVEL                ERR_TOOMANYTARGETS
//            ERR_NOSUCHNICK
//            RPL_AWAY

//    Examples:

//    :Angel!wings@irc.org PRIVMSG Wiz :Are you receiving this message ?
//                                    ; Message from Angel to Wiz.

//    PRIVMSG Angel :yes I'm receiving it !
//                                    ; Command to send a message to Angel.

//    PRIVMSG jto@tolsun.oulu.fi :Hello !
//                                    ; Command to send a message to a user
//                                    on server tolsun.oulu.fi with
//                                    username of "jto".

//    PRIVMSG kalt%millennium.stealth.net@irc.stealth.net :Are you a frog?
//                                    ; Message to a user on server
//                                    irc.stealth.net with username of
//                                    "kalt", and connected from the host
//                                    millennium.stealth.net.

//    PRIVMSG kalt%millennium.stealth.net :Do you like cheese?
//                                    ; Message to a user on the local
//                                    server with username of "kalt", and
//                                    connected from the host
//                                    millennium.stealth.net.

//    PRIVMSG Wiz!jto@tolsun.oulu.fi :Hello !
//                                    ; Message to the user with nickname
//                                    Wiz who is connected from the host
//                                    tolsun.oulu.fi and has the username
//                                    "jto".

//    PRIVMSG $*.fi :Server tolsun.oulu.fi rebooting.
//                                    ; Message to everyone on a server
//                                    which has a name matching *.fi.

//    PRIVMSG #*.edu :NSFNet is undergoing work, expect interruptions
//                                    ; Message to all users who come from
//                                    a host which has a name matching
//                                    *.edu.