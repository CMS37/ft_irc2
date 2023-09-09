#include "../Parser.hpp"

void Parser::cmd_nick()
{
    std::string nickname(this->_tokens[1]);

    if (this ->_tokens.size() != 2)
		this->_server.send_message_to_client_with_code(this->_client, "461", "NICK : wrong number of arguments");
	else if (contains(nickname, ":/\0"))
		this->_server.send_message_to_client_with_code(this->_client, "432", nickname + " :Erroneus nickname");
	else if (this->_server.check_nickname(nickname))
		this->_server.send_message_to_client_with_code(this->_client, "433", nickname + " :Nickname is already in use");
	else
    {
        this->_client.setNickname(nickname);
		this->_server.registrate(this->_client);
    }
}

// 3.1.2 Nick message

//       Command: NICK
//    Parameters: <nickname>

//    NICK command is used to give user a nickname or change the existing
//    one.

//    Numeric Replies:

//            ERR_NONICKNAMEGIVEN             ERR_ERRONEUSNICKNAME
//            ERR_NICKNAMEINUSE               ERR_NICKCOLLISION
//            ERR_UNAVAILRESOURCE             ERR_RESTRICTED

//    Examples:

//    NICK Wiz                ; Introducing new nick "Wiz" if session is
//                            still unregistered, or user changing his
//                            nickname to "Wiz"

//    :WiZ!jto@tolsun.oulu.fi NICK Kilroy
//                            ; Server telling that WiZ changed his
//                            nickname to Kilroy.