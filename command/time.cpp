#include "../Parser.hpp"

void Parser::cmd_time()
{
	std::string msg;
    time_t startTime = _server.getStartTime();

	if(this->_tokens.size() == 2)
		msg = std::string(ctime(&startTime)) + "; check the time on the server \"" + _server.getHostname() + "\"";
	else
		//유저가 서버에 접속한시간?  (<USER> TIME <SERVER>)?!
	this->_server.send_message_to_fd_buffer(this->_client, msg);
}


// 3.4.6 Time message

//       Command: TIME
//    Parameters: [ <target> ]

//    The time command is used to query local time from the specified
//    server. If the <target> parameter is not given, the server receiving
//    the command must reply to the query.

//    Wildcards are allowed in the <target> parameter.

//    Numeric Replies:

//            ERR_NOSUCHSERVER              RPL_TIME

//    Examples:
//    TIME tolsun.oulu.fi             ; check the time on the server
//                                    "tolson.oulu.fi"
