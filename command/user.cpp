#include "../Parser.hpp"

void Parser::cmd_user()
{
    if(this->_tokens.size() < 5)
        this->_server.send_message_to_client_with_code(this->_client, "461", "USER :Not enough parameters");
    
    this->_client.setUsername(this->_tokens[1]);
    this->_client.setHostname(this->_tokens[2]);
    this->_client.setServername(this->_tokens[3]); //서버이름그냥 호스트네임으로?
    this->_client.setRealname(this->_tokens[4]);

    this->_server.registrate(this->_client);

    //이후 호스트네임 확인작업 하는듯, 만약 적절한 거 못 찾으면 ip 주소로 대체
    //해당 내용은 클라이언트에 메세지 줌
    //ex)*** Looking up your hostname...
    //ex)*** Could not resolve your hostname: Request timed out; using your IP address (127.0.0.1) instead.
    
    // pass -> nick -> user 순으로 입력되고 승인되야 웰컴 메세지 출력하고 다른 명령어 사용가능
    // ex) 001 : Welcome to the Internet Relay Network <nickname>!<username>@<host>
}
