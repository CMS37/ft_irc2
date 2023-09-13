#include "../Parser.hpp"

void Parser::cmd_user()
{
    if(this->_tokens.size() < 5)
        this->_server.send_message_to_client_with_code(this->_client, "461", "USER :Not enough parameters");
    
    this->_client.setUsername(this->_tokens[1]);
    this->_client.setHostname("IP.hosted-by-42");
    this->_client.setServername("irc.42.kr");
    
    std::string realname = this->_tokens[4].substr(1, this->_tokens[4].size() - 1);
    if (_tokens.size() > 5)
        realname.append(" ");
    for (size_t i = 5; i < this->_tokens.size(); i++)
    {
        realname.append(this->_tokens[i]);
        if (i < this->_tokens.size() - 1)
            realname.append(" ");
    }
    this->_client.setRealname(realname);
    this->_server.registrate(this->_client);
}
