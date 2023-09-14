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

Parser::~Parser()
{

}


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
        else if(this->_tokens[0] == "WHO")
            cmd_who();
        else if(this->_tokens[0] == "WHOIS")
            cmd_whois();
    }
    catch(const std::exception& e)
    {
        _server.send_message_to_fd_buffer(_client, e.what());
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
        this->_server.send_message_to_fd_buffer(_client, ":!@" + this->_server.getHostname() + " CAP * LS :End of CAP LS negotiation\n");
    }
}
