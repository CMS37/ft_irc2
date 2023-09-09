#include "../Parser.hpp"

void Parser::cmd_pass()
{
    if(this->_tokens.size() != 2)
        this->_server.send_message_to_client_with_code(this->_client, "461", "PASS :Not enough parameters");
    else if(this->_client.getIsRegistered())
        this->_server.send_message_to_client_with_code(this->_client, "462", " :You may not reregister");

    std::string password = parse_pw(this->_tokens[1]);
    if (password != _server.getPassword())
        this->_server.send_message_to_client_with_code(this->_client, "464", " :Password incorrect");
    else
        this->_client.setIsPasswordAllowed(true);
}

// 3.1 Connection Registration

//    The commands described here are used to register a connection with an
//    IRC server as a user as well as to correctly disconnect.

//    A "PASS" command is not required for a client connection to be
//    registered, but it MUST precede the latter of the NICK/USER
//    combination (for a user connection) or the SERVICE command (for a
//    service connection). The RECOMMENDED order for a client to register
//    is as follows:

//                            1. Pass message
//            2. Nick message                 2. Service message
//            3. User message

//    Upon success, the client will receive an RPL_WELCOME (for users) or
//    RPL_YOURESERVICE (for services) message indicating that the
//    connection is now registered and known the to the entire IRC network.
//    The reply message MUST contain the full client identifier upon which
//    it was registered.