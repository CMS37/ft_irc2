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
    // this->_tokens = split(_str, ' ');
    std::stringstream ss(str);
    std::string temp;
    while(ss >> temp)
        this->_tokens.push_back(temp);
}

Parser::~Parser() {}


void Parser::what_is_this()
{
    try
    {
        // if(!this->_client.getIsRegistered())
        //     this->_server.send_message_to_client_with_code(this->_client, "451", ":You have not registered");
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
        else if(this->_tokens[0] == "PRIVMSG")
            cmd_privmsg();
        else if(this->_tokens[0] == "TOPIC")
            cmd_topic();
        else if(this->_tokens[0] == "KICK")
            cmd_kick();
        else if(this->_tokens[0] == "INVITE")
            cmd_invite();
        else if(this->_tokens[0] == "PART")
            cmd_part();
    //     else if(this->_tokens[0] == "LIST")
    //         cmd_list();
    //     else if(this->_tokens[0] == "NAMES")
    //         cmd_names();
    //     else if(this->_tokens[0] == "HELP")
    //         cmd_help();
    //     else if(this->_tokens[0] == "TIME")
    //         cmd_time();
    //     else if(this->_tokens[0] == "VERSION")
    //         cmd_version();
    //     else if(this->_tokens[0] == "MOTD")
    //         cmd_motd();
    //     else if(this->_tokens[0] == "EXIT")
    //         cmd_exit();
    }
    catch(const std::exception& e)
    {
        _server.send_message_to_fd(this->_client.getFd(), e.what());
    }

}

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
    // else if (this->_tokens[1] == "END")
    // {
    //     this->_server.send_message_to_fd(this->_client.getFd(), "CAP * END :End of CAP LS negotiation\n");
    // }
    // else
    //     throw std::invalid_argument("Wrong argument\n");
}

void Parser::cmd_join()
{

    if(!this->_client.getIsRegistered())
        this->_server.send_message_to_fd(this->_client.getFd(), "451 :You have not registered\n");

    std::string channelName(this->_tokens[1]);
    if (this->_tokens.size() == 2) // /join #channel
        this->_client.joinChannel(this->_tokens[1], "");
    else if (this->_tokens.size() == 3) // /join #channel key
        this->_client.joinChannel(this->_tokens[1], this->_tokens[2]);
    else if (this->_tokens.size() == 4)
        this->_client.joinChannel(this->_tokens[1], this->_tokens[2]);
}

void Parser::cmd_part()
{
    if(!this->_client.getIsRegistered())
        this->_server.send_message_to_fd(this->_client.getFd(), "451 :You have not registered\n");
    if(this->_tokens.size() != 2)
    {
        this->_server.send_message_to_client_with_code(this->_client, "461", "PART :Not enough parameters");
        return;
    }

    Channel *channel = this->_server.getChannel(this->_tokens[1]);
    if (channel == NULL)
    {
        this->_server.send_message_to_client_with_code(this->_client, "403", this->_tokens[1] + " :No such channel");
        return ;
    }
    if (!channel->is_invited(this->_client.getNickname()))
    {
        this->_server.send_message_to_client_with_code(this->_client, "442", this->_tokens[1] + " :You're not on that channel");
        return ;
    }
    
    channel->deleteClient(this->_client.getNickname());
    
    std::string msg = ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + this->_server.getHostname() + " PART " + this->_tokens[1] + "\r\n";
    this->_server.send_message_to_channel(this->_tokens[1], msg);
    this->_server.send_message_to_fd(this->_client.getFd(), msg);

    // msg = this->_client.getNickname() + " Channel :Users Name\r\n";
    // this->_server.send_message_to_client_with_code(this->_client, "321", msg);
    // this->_server.send_message_to_channel_with_code(this->_tokens[1], this->_client, "321", msg);

    // msg = this->_client.getNickname() + " " + this->_tokens[1] + "0 :is\r\n";
    // this->_server.send_message_to_client_with_code(this->_client, "322", msg);
    // this->_server.send_message_to_channel_with_code(this->_tokens[1], this->_client, "322", msg);

    // msg = this->_client.getNickname() + " :End of /LIST\r\n";
    // this->_server.send_message_to_client_with_code(this->_client, "323", msg);
    // this->_server.send_message_to_channel_with_code(this->_tokens[1], this->_client, "323", msg);



//     Command is: |PART|[ #here ]
// Response to send is 
// |:siykim!siyoungkim@:ft_irc.de PART #here
// | to: siykim

// Message to the channel is 
// |:siykim!siyoungkim@:ft_irc.de PART #here
// | to: #here

// Response to send is 
// |:ft_irc.de 321 siykim Channel :Users Name
// :ft_irc.de 322 siykim #here 0 :is
// :ft_irc.de 323 siykim End of /LIST
// | to: siykim

// Message to the channel is 
// |:ft_irc.de 321 siykim Channel :Users Name
// :ft_irc.de 322 siykim #here 0 :is
// :ft_irc.de 323 siykim End of /LIST
// | to: #here
}

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
        if(this->_client.getNickname() == "")
            this->_server.send_message_to_fd(this->_client.getFd(), ":! NICK " + nickname + "\r\n");
        else
            this->_server.send_message_to_fd(this->_client.getFd(), ":" + this->_client.getNickname() + "!" + this->_client.getHostname() + " NICK " + nickname + "\r\n");
        this->_client.setNickname(nickname);
		this->_server.registrate(this->_client);
    }
}

void Parser::cmd_ping()
{
    if (this->_tokens.size() < 2)
        this->_server.send_message_to_client_with_code(this->_client, "461", "PING :Not enough parameters");
	else if (this->_tokens.size() == 2)
        this->_server.send_message_to_fd(this->_client.getFd(), "PONG " + this->_server.getHostname() + "\r\n");
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
    // /invite <nickname> <channel>
    if (this->_tokens.size() < 3)
        this->_server.send_message_to_client_with_code(this->_client, "461", "INVITE :Not enough parameters");
    Client *client = this->_server.getClient(this->_tokens[1]);
    Channel *channel = this->_server.getChannel(this->_tokens[2]);
    if (channel == NULL)
    {
        this->_server.send_message_to_client_with_code(_client, "403", this->_tokens[2] + ": No such channel");
        return ;
    }
    if (client == NULL)
    {
        this->_server.send_message_to_client_with_code(_client, "401", this->_tokens[1] + ": No such nick");
        return ;
    }
    if (!channel->is_operator(this->_client))
    {
        this->_server.send_message_to_client_with_code(_client, "482", this->_tokens[2] + ": You're not channel operator");
        return ;
    }
    if (channel->is_invited(client->getNickname()))
    {
        this->_server.send_message_to_client_with_code(_client, "443", this->_tokens[2] + " " + this->_tokens[1] + " :is already on channel");
        return ;
    }
    this->_server.send_message_to_client_with_code(_client, "341", this->_tokens[1] + " " + this->_tokens[2] + " :Inviting " + this->_tokens[1] + " to " + this->_tokens[2]);
    try
    {
        channel->addClient(*client);
    }
    catch(const std::exception& e)
    {
        this->_server.send_message_to_fd(this->_client.getFd(), e.what());
        return ;
    }
    this->_server.send_message_to_fd(client->getFd(), ":" + this->_client.getNickname() + " INVITE " + this->_tokens[1] + " " + this->_tokens[2] + "\r\n");
}

void Parser::cmd_kick()
{
    if (this->_tokens.size() < 3)
        this->_server.send_message_to_client_with_code(this->_client, "461", "KICK :Not enough parameters");
    Channel *channel = this->_server.getChannel(this->_tokens[1]);
    Client *client = this->_server.getClient(this->_tokens[2]);
	if (channel == NULL)
    {
		this->_server.send_message_to_client_with_code(_client, "403", this->_tokens[1] + " :No such channel");
        return ;
    }
    if (client == NULL)
    {
        this->_server.send_message_to_client_with_code(_client, "401", this->_tokens[2] + " :No such nick");
        return ;
    }
    if (!channel->is_invited(client->getNickname()))
    {
        this->_server.send_message_to_client_with_code(_client, "441", this->_tokens[2] + " " + this->_tokens[1] + " :They aren't on that channel");
        return ;
    }
    if (!channel->is_operator(this->_client))
    {
        this->_server.send_message_to_client_with_code(_client, "482", this->_tokens[1] + " :You're not channel operator");
        return ;
    }
    channel->deleteClient(client->getNickname());
    this->_server.send_message_to_channel(this->_tokens[1], ":" + this->_client.getNickname() + " KICK " + this->_tokens[1] + " " + this->_tokens[2] + ": kick by " + _client.getNickname() + " \r\n");
}

void Parser::cmd_topic()
{
    std::string chan_name = this->_tokens[1];
    std::string topic;
    for(unsigned int i = 2; i < this->_tokens.size(); i++)
    {
        topic += this->_tokens[i];
        if(i < this->_tokens.size() - 1)
            topic += " ";
    }
    topic = topic.substr(1, topic.size() - 1);

    Channel *channel = this->_server.getChannel(chan_name);
    if (channel == NULL)
    {
        this->_server.send_message_to_client_with_code(_client, "403", chan_name + " :No such channel");
        return ;
    }
    else
    {
        this->_server.send_message_to_fd(this->_client.getFd(), ":" + this->_client.getNickname() + "!" + this->_client.getHostname() + "@" + this->_server.getHostname() + " TOPIC " + chan_name + " :" + topic + "\r\n");
    }
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
