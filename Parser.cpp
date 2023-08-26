#include"Parser.hpp"
// /list
// /join
// /part
// /users
// /names
// /msg
// /quit
// /help
// /time
// /version
// /motd
// /invite
// /kick
// /topic
// /mode

Parser::Parser(Server& server, Client& client, std::string str) : _server(server), _client(client), _str(str)
{
    // while (str.find("\r\n") != std::string::npos)
	// 	str.replace(str.find("\r\n"), 2, "\n");
    // 스플릿구분자 '\n'으로 해야되는거같아욥 \r\n?이나..?아닐수도있습니당 :(
    this->_tokens = split(_str, ' ');
}

Parser::~Parser() {}


void Parser::what_is_this()
{
    try
    {
        /*
            이게더 나을꺼같아요 이건 본인 편하신대로 하셔요 확인하구 지우셔도됩니다.
            map(명령어, 함수포인터)
            for (std::map<std::string, void (Parser::*)().iterator it = _cmd.begin(); it != _cmd.end(); it++)
            {
                if (this->_tokens[0] == it->first)
                    (this->*(it->second))(_tokens[1]]);
            }
            파싱할때 커맨드는 첫줄바꿈 전까지만 받아서 토큰으로 나눠서 저장해놓고
            토큰[0]이랑 맵의 키랑 비교해서 함수포인터를 실행시키는 방식으로
            토큰[1]은 안나누고 그대로 전달해서 커맨드에서 처리하게
        */
        // for(unsigned int i = 0; i < this->_tokens.size(); i++)
        // std::cout << "token " << i << ": " << this->_tokens[i] << std::endl;

    if (this->_tokens[0] == "CAP")
            cmd_cap();
        else if(this->_tokens[0] == "JOIN")
            cmd_join();
        else if(this->_tokens[0] == "NICK")
            cmd_nick();
        else if(this->_tokens[0] == "PING")
            cmd_ping();
        else if(this->_tokens[0] == "USER")
            cmd_user();
        else if(this->_tokens[0] == "MODE")
        {
			std::string modeArgs;
			for (unsigned long i = 1; i < _tokens.size(); i++)
			{
				modeArgs += _tokens[i];
				if (i < _tokens.size() - 1)
        		    modeArgs += " ";
			}
			cmd_mode(modeArgs);
		}
        else if(this->_tokens[0] == "QUIT")
            cmd_quit();
        else if(this->_tokens[0] == "PASS")
            cmd_pass();
    //     else if(this->_tokens[0] == "LIST")
    //         cmd_list();
    //     else if(this->_tokens[0] == "PART")
    //         cmd_part();
    //     else if(this->_tokens[0] == "NAMES")
    //         cmd_names();
    //     else if(this->_tokens[0] == "MSG")
    //         cmd_msg();
    //     else if(this->_tokens[0] == "HELP")
    //         cmd_help();
    //     else if(this->_tokens[0] == "TIME")
    //         cmd_time();
    //     else if(this->_tokens[0] == "VERSION")
    //         cmd_version();
    //     else if(this->_tokens[0] == "MOTD")
    //         cmd_motd();
    //     else if(this->_tokens[0] == "INVITE")
    //         cmd_invite();
    //     else if(this->_tokens[0] == "KICK")
    //         cmd_kick();
    //     else if(this->_tokens[0] == "TOPIC")
    //         cmd_topic();
    //     else if(this->_tokens[0] == "EXIT")
    //         cmd_exit();
    }
    catch(const std::exception& e)
    {
        _server.send_message_to_fd(this->_client.getFd(), e.what());
    }

}

// std::vector<std::string> Parser::split(std::string input, char delimiter)
// {
//     std::vector<std::string> answer;
//     std::stringstream ss(input);
//     std::string temp;

//     while(getline(ss, temp, delimiter))
//         answer.push_back(temp);

//     return answer;
// }

void Parser::setString(std::string str)
{
    this->_str = str;
    this->_tokens = split(str, ' ');
}

//===========COMMANDS===========//

void Parser::cmd_cap()
{
    if(this->_tokens[1] == "LS")
    {
        this->_server.send_message_to_fd(this->_client.getFd(), "CAP * LS :End of CAP LS negotiation\n");
    }
    else
        throw std::invalid_argument("Wrong argument\n");
}

void Parser::cmd_join()
{
    if(!this->_client.getIsRegistered())
        this->_server.send_message_to_fd(this->_client.getFd(), "451 :You have not registered\n");

    this->_tokens[1].erase(_tokens[1].length() - 1);
    if (this->_tokens.size() == 2) // /join #channel
        this->_client.joinChannel(this->_tokens[1], "");
    else if (this->_tokens.size() == 3) // /join #channel key
        this->_client.joinChannel(this->_tokens[1], this->_tokens[2]);
    else if (this->_tokens.size() == 4)
        this->_client.joinChannel(this->_tokens[1], this->_tokens[2]);
}

// void Parser::cmd_part()
// {
//     this->_client->partChannel(this->_tokens[1]);
// }

// void Parser::cmd_names()
// {
//     this->_client->showNames(this->_tokens[1]);
// }

void Parser::cmd_list()
{
    this->_server.showChannels();
}

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

void Parser::cmd_ping()
{
    if (this->_tokens.size() < 2)
        this->_server.send_message_to_client_with_code(this->_client, "461", "PING :Not enough parameters");
	else if (this->_tokens.size() == 2)
        this->_server.send_message_to_fd(this->_client.getFd(), ":PONG " + this->_server.getHostname() + " " + this->_client.getHostname() +  "\r\n");
	// else
	// {
	// 	string args;
	// 	for (unsigned int i = 0; i < c.getArgs().size(); i++)
	// 		args += c.getArgs()[i] + " ";
	// 	sendResponse("PONG :message from " + hostname + " " + args, user);
	// }
}

void Parser::cmd_user()
{
    if(this->_tokens.size() < 5)
        this->_server.send_message_to_client_with_code(this->_client, "461", "USER :Not enough parameters");
    
    this->_client.setUsername(this->_tokens[1]);
    this->_client.setHostname(this->_tokens[2]);
    this->_client.setServername(this->_tokens[3]);
    this->_client.setRealname(this->_tokens[4]);

    this->_server.registrate(this->_client);

    //이후 호스트네임 확인작업 하는듯, 만약 적절한 거 못 찾으면 ip 주소로 대체
    //해당 내용은 클라이언트에 메세지 줌
    //ex)*** Looking up your hostname...
    //ex)*** Could not resolve your hostname: Request timed out; using your IP address (127.0.0.1) instead.
    
    // pass -> nick -> user 순으로 입력되고 승인되야 웰컴 메세지 출력하고 다른 명령어 사용가능
    // ex) 001 : Welcome to the Internet Relay Network <nickname>!<username>@<host>


}

void Parser::cmd_pass()
{
    if(this->_tokens.size() != 2)
        this->_server.send_message_to_client_with_code(this->_client, "461", "PASS :Not enough parameters");
    else if(this->_client.getIsRegistered())
        this->_server.send_message_to_client_with_code(this->_client, "462", ":You may not reregister");

    std::string password = parse_pw(this->_tokens[1]);
    if (password != _server.getPassword())
        this->_server.send_message_to_client_with_code(this->_client, "464", ":Password incorrect");
    else
        this->_client.setIsPasswordAllowed(true);
}

void Parser::cmd_msg()
{

}

void Parser::cmd_quit()
{
    this->_server.client_disconnect_from_channel(&(this->_client));
    this->_server.client_disconnect(this->_client.getFd());
    
    // if(this->_tokens.size() == 2)
        //메세지 전송, token[1]이 메세지
    // else
        //기본 메세지 전송, 닉네임
}

void Parser::cmd_help()
{
    //필요없을듯
}

void Parser::cmd_time()
{
	std::string msg;
    time_t startTime = _server.getStartTime();

	if(this->_tokens.size() == 2)
		msg = std::string(ctime(&startTime)) + "; check the time on the server \"" + _server.getHostname() + "\"";
	else
		//유저가 서버에 접속한시간?  (<USER> TIME <SERVER>)?!
	this->_server.send_message_to_fd(this->_client.getFd(), msg);
}

void Parser::cmd_version()
{

}

void Parser::cmd_motd()
{

}

void Parser::cmd_invite()
{

}

void Parser::cmd_kick()
{

}

void Parser::cmd_topic()
{
    this->_server.setTopic(this->_tokens[1], this->_tokens[2]);    
}

/*
ex)
No topic set for #here
14:52 -!- siykm changed the topic of #here to: this_is_topic
14:52 -!- Topic for #here: this_is_topic
14:52 -!- Topic set by siykm [root@127.0.0.1] [Tue Aug 22 14:52:18 2023]
14:52 -!- Topic for #here: this_is_topic
14:52 -!- Topic set by siykm [root@127.0.0.1] [Tue Aug 22 14:52:18 2023]
*/


void Parser::cmd_exit()
{

}
