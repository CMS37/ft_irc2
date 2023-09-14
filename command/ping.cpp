#include "../Parser.hpp"

void Parser::cmd_ping()
{
    if (this->_tokens.size() < 2)
        this->_server.send_message_to_client_with_code(this->_client, "461", "PING :Not enough parameters");
	else if (this->_tokens.size() == 2)
        this->_server.send_message_to_fd_buffer(this->_client, "PONG " + this->_server.getHostname() + " :" + this->_server.getHostname() + "\r\n");
}

// 4.6.2 Ping message

//       Command: PING
//    Parameters: <server1> [<server2>]

//    The PING message is used to test the presence of an active client at
//    the other end of the connection.  A PING message is sent at regular
//    intervals if no other activity detected coming from a connection.  If
//    a connection fails to respond to a PING command within a set amount
//    of time, that connection is closed.

//    Any client which receives a PING message must respond to <server1>
//    (server which sent the PING message out) as quickly as possible with
//    an appropriate PONG message to indicate it is still there and alive.
//    Servers should not respond to PING commands but rely on PINGs from
//    the other end of the connection to indicate the connection is alive.
//    If the <server2> parameter is specified, the PING message gets
//    forwarded there.

//    Numeric Replies:

//            ERR_NOORIGIN                    ERR_NOSUCHSERVER

//    Examples:

//    PING tolsun.oulu.fi             ; server sending a PING message to
//                                    another server to indicate it is still
//                                    alive.

//    PING WiZ                        ; PING message being sent to nick WiZ

// 4.6.3 Pong message

//       Command: PONG
//    Parameters: <daemon> [<daemon2>]

//    PONG message is a reply to ping message.  If parameter <daemon2> is
//    given this message must be forwarded to given daemon.  The <daemon>
//    parameter is the name of the daemon who has responded to PING message
//    and generated this message.

//    Numeric Replies:

//            ERR_NOORIGIN                    ERR_NOSUCHSERVER

//    Examples:

//    PONG csd.bu.edu tolsun.oulu.fi  ; PONG message from csd.bu.edu to
//                                    tolsun.oulu.fi