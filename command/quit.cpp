#include "../Parser.hpp"

void Parser::cmd_quit()
{
    std::string msg;
    msg.append(":" + _client.getNickname() + "!" + _client.getUsername() + "@" + _client.getServername() + " QUIT :\r\n");
    _client.getServer().send_message_to_fd(_client.getFd(), msg);
    _client.quitAll();
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