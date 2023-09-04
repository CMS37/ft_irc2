#include "../Parser.hpp"

void Parser::cmd_privmsg()
{
    if(!this->_client.getIsRegistered())
    {
        this->_server.send_message_to_fd(this->_client.getFd(), "451 :You have not registered\n");
        return;
    }

    
    std::string target_channel = this->_tokens[1];
    std::string message;
    for(unsigned int i = 2; i < this->_tokens.size(); i++)
    {
        message += this->_tokens[i];
        if(i < this->_tokens.size() - 1)
            message += " ";
    }

    this->_server.send_message_to_channel(target_channel, 
    ":" + this->_client.getNickname() + "!" + this->_client.getHostname() + "@:" + this->_server.getHostname() + " PRIVMSG " + target_channel + " :" + message + "\r\n");






        //|PRIVMSG|[ #here hi ]
        //:siykim!siyoungkim@:ft_irc.de PRIVMSG #here :hi 



    // if(this->_tokens.size() < 2)
    // {
    //     this->_server.send_message_to_client_with_code(this->_client, "461", "PRIVMSG :Not enough parameters");
    //     return;
    // }

    // std::vector<std::string> targets = split(this->_tokens[1], ',');
    // std::string message;
    // for(unsigned int i = 2; i < this->_tokens.size(); i++)
    // {
    //     message += this->_tokens[i];
    //     if(i < this->_tokens.size() - 1)
    //         message += " ";
    // }

    // for(unsigned int i = 0; i < targets.size(); i++)
    // {
    //     if(targets[i][0] == '#')
    //     {
    //         Channel *channel = this->_server.getChannel(targets[i]);
    //         if(channel == NULL)
    //             this->_server.send_message_to_client_with_code(this->_client, "401", targets[i] + " :No such nick/channel");
    //         else
    //             this->_server.send_message_to_channel(targets[i], message);
    //     }
    //     else
    //     {
    //         Client *client = this->_server.getClient(targets[i]);
    //         if(client == NULL)
    //             this->_server.send_message_to_client_with_code(this->_client, "401", targets[i] + " :No such nick/channel");
    //         else
    //             this->_server.send_message_to_fd(client->getFd(), ":" + this->_client.getNickname() + " PRIVMSG " + targets[i] + " :" + message + "\r\n");
    //     }
    // }


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