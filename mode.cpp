#include "Parser.hpp"

void Parser::cmd_mode(const std::string &mode)
{
	std::vector<std::string> str = split(mode, ' ');

	Channel *channel = _server.getChannel(str[0]);
	if (channel == NULL)
	{
		// code 403 "<channel> :No such channel"
		throw std::invalid_argument(":No such channel\n");
	}
	std::string ch = channel->getName();
	if (ch[0] == '+')
	{
		//모드,오퍼 지원안하는 채널
	}

	if (!channel->is_invited(_client.getNickname()))
	{
		std::cout << _client.getNickname() << std::endl;
		// code 441 "<nickname> <channel> :They aren't on that channel"
		throw std::invalid_argument(":You aren't on that channel\n");
	}
	else if (!channel->is_operator(_client))
	{
		// code 482 "<channel> :You're not channel operator"
		throw std::invalid_argument(":You're not channel operator\n");
	}

	std::string seting;
	std::string argv;
	std::string set = str[1];
	std::string::iterator it = set.begin() + 1;
	size_t i = 2;
	bool pos;

	if (set[0] == '+')
		pos = true;
	else if (set[0] == '-')
		pos = false;

	for (; it != set.end(); ++it)
	{
		switch(*it)
		{
			case '+':
				pos = true;
				break;
			case '-':
				pos = false;
				break;
			case 'o':
				if (str.size() < i + 1)
				{
					// code 461 "<channel> :Not enough parameters"
					throw std::invalid_argument(":Not enough parameters\n");
				}
				else if (channel->is_invited(str[i]))
				{
					// code 441 "<nickname> <channel> :They aren't on that channel"
					throw std::invalid_argument(":You aren't on that channel\n");
				}
				else
				{
					Client *client = _server.getClient(str[i]);
					if (pos)
					{
						seting += "+o "; 
						channel->setOperator(*client);
					}
					else
					{
						seting += "-o ";
						channel->unsetOperator(*client);
					}
				}
				argv = str[i] + " ";
				i++;
				break;
			case 'i':
				if (pos)
				{
					seting += "+i ";
					channel->setInviteOnly(true);
				}
				else
				{
					seting += "-i ";
					channel->setInviteOnly(false);
				}
				break;
			case 'k':
				if (str.size() < i + 1)
				{
					// code 461 "<channel> :Not enough parameters"
					throw std::invalid_argument(":Not enough parameters\n");
				}
				else
				{
					if (pos)
					{
						seting += "+k ";
						channel->setKey(str[i]);
					}
					else
					{
						seting += "-k ";
						channel->unsetKey(str[i]);
					}
				}
				argv = str[i] + " ";
				i++;
				break;
			case 't':
				if (pos)
				{
					seting += "+t ";
					channel->setTopic("");
				}
				else
				{
					seting += "-t ";
					channel->unsetTopic();
				}
				argv = str[i] + " ";
				i++;
				break;
			case 'l':
				if (!pos)
				{
					seting += "-l ";
					channel->unsetLimit();
				}
				else
				{
					if (str.size() < i + 1)
					{
						// code 461 "<channel> :Not enough parameters"
						throw std::invalid_argument(":Not enough parameters\n");
					}
					channel->setLimit(atoi(str[i].c_str()));
					seting += "+l ";
					argv = str[i] + " ";
					i++;
				}
				break;
			case 'e':
				break;
			default:
				// code 472 "<char> :is unknown mode char to me"
				throw std::invalid_argument(":is unknown mode char to me\n");
		}
	}
	std::cout << "=============MODE=============" << std::endl;
	std::cout << "seting: " << seting << std::endl;
	std::cout << "argv: " << argv << std::endl;
	std::cout << "==============================" << std::endl;
}
// /*
// //         MODE - 채널 모드 및 구성원의 권한 변경
// //		         +o : 채널 운영자 권한 부여/회수
// //               +i : 초대전용 채널 설정/해제
// //               +k : 채널 비밀번호 설정/해제
// //               +t : 채널 주제 변경 권한 설정/해제
// //               +l : 채널 최대 사용자 수 설정/해제
// */