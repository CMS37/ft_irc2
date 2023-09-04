#include "../Parser.hpp"

void Parser::cmd_quit()
{
    this->_server.client_disconnect_from_channel(&(this->_client));
    this->_server.client_disconnect(this->_client.getFd());
    
    // if(this->_tokens.size() == 2)
        //메세지 전송, token[1]이 메세지
    // else
        //기본 메세지 전송, 닉네임
}

// 3.1.7 Quit

//       Command: QUIT
//    Parameters: [ <Quit Message> ]

//    A client session is terminated with a quit message.  The server
//    acknowledges this by sending an ERROR message to the client.

//    Numeric Replies:

//            None.

//    Example:

//    QUIT :Gone to have lunch        ; Preferred message format.

//    :syrk!kalt@millennium.stealth.net QUIT :Gone to have lunch ; User
//                                    syrk has quit IRC to have lunch.