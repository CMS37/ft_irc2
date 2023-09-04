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
				modeArgs.append(_tokens[i]);
				if (i < _tokens.size() - 1)
                    modeArgs.append(" ");
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
    else if (this->_tokens[1] == "END")
    {
        this->_server.send_message_to_fd(this->_client.getFd(), "CAP * END :End of CAP LS negotiation\n");
    }
    else
        throw std::invalid_argument("Wrong argument\n");
}

// void Parser::cmd_names()
// {
//     this->_client->showNames(this->_tokens[1]);
// }

void Parser::cmd_list()
{
    this->_server.showChannels();
}

void Parser::cmd_help()
{
    //필요없을듯
}

void Parser::cmd_version()
{

}

void Parser::cmd_motd()
{

}

void Parser::cmd_exit()
{

}
