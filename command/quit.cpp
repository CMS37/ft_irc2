#include "../Parser.hpp"

void Parser::cmd_quit()
{
    std::string msg;
    msg.append(":" + _client.getNickname() + "!" + _client.getUsername() + "@" + _client.getHostname() + " QUIT :\r\n");
    _server.send_message_to_fd_buffer(_client, msg);
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