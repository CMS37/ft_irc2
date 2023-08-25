#include "Parser.hpp"

void Parser::cmd_mode(const std::string &mode)
{
	std::vector<std::string> str = split(mode, ' ');
	std::vector<std::string> argv;
	std::string tmp;
	std::string tmp2;
	std::string target;
	std::string mode_str;

	for (std::vector<std::string>::iterator it = str.begin(); it != str.end(); ++it)
	{
		std::cout << "it: " << *it << std::endl;
		switch ((*it)[0])
		{
			case '&':
			case '!':
			case '#':
				if (!target.empty())
				{
					// code 501 "<channel> :Unknown MODE flag"
					throw std::invalid_argument(":Too many targets\n");
				}
				else
					target = *it;
				break;
			case '-':
			case '+':
				mode_str += *it;
                break;
			default:
				std::cout << "push_back: " << *it << std::endl;
				argv.push_back(*it);
                break;
		}
	}

	size_t pos = mode_str.find('-');
	if (pos != std::string::npos)
	{
		tmp = mode_str.substr(1, pos - 1);
		tmp2 = mode_str.substr(pos + 1);
	}
	else
	{
		tmp = mode_str.substr(1);
		tmp2 = "";
	}

	std::cout << ": target: " << target << std::endl;
	std::cout << ": mode: " << mode_str << std::endl;
	std::cout << ": tmp: " << tmp << std::endl;
	std::cout << ": tmp2: " << tmp2 << std::endl;
	std::cout << ": argv: ";
	for (std::vector<std::string>::iterator it = argv.begin(); it != argv.end(); ++it)
		std::cout << *it << " ";
	std::cout << std::endl;
	std::cout << "==============================" << std::endl;
	Channel *channel = _server.getChannel(target);
	if (channel == NULL)
	{
		// code 403 "<channel> :No such channel"
		throw std::invalid_argument(":No such channel\n");
	}
	if (!channel->is_operator(_client))
	{
		// code 482 "<channel> :You're not channel operator"
		throw std::invalid_argument(":You're not channel operator\n");
	}
	std::cout << "==============SET MODE FOR : " << channel->getName() << "================" << std::endl;
	for (size_t i = 0; i < tmp.size(); i++)
	{
		size_t j = 0;
		std::cout << i << ": " << tmp[i] << std::endl;
		switch(tmp[i])
		{
			case 'o':
				if (argv.size() < j + 1)
				{
					// code 461 "<channel> :Not enough parameters"
					throw std::invalid_argument(":Not enough parameters\n");
				}
				else if (channel->is_invited(argv[j]))
				{
					// code 441 "<nickname> <channel> :They aren't on that channel"
					throw std::invalid_argument(":You aren't on that channel\n");
				}
				else
				{
					Client *client = _server.getClient(argv[j]);
					channel->setOperator(*client);
				}
				break;
			case 'i':
				channel->setInviteOnly(true);
				break;
			case 'k':
				if (argv.size() < j + 1)
				{
					// code 461 "<channel> :Not enough parameters"
					throw std::invalid_argument(":Not enough parameters\n");
				}
				else
					channel->setKey(argv[j++]);
				break;
			case 't':
				if (argv.size() < j)
				{
					// code 461 "<channel> :Not enough parameters"
					throw std::invalid_argument(":Not enough parameters\n");
				}
				else
					channel->setTopic(argv[j++]);
				break;
			case 'l':
				if (argv.size() <= j)
				{
					// code 461 "<channel> :Not enough parameters"
					throw std::invalid_argument(":Not enough parameters\n");
				}
				else
					channel->setLimit(atoi(argv[j++].c_str()));
				break;
			case 'e':
				break;
			default:
				// code 472 "<char> :is unknown mode char to me"
				throw std::invalid_argument(":is unknown mode char to me\n");
		}
	}
}
	//unsetmode
	// for (size_t i = 0; i < tmp2.size(); i++)
	// {
	// 	for (std::vector<char>::iterator it = channel->getUnsetModes().begin(); it != channel->getUnsetModes().end(); ++it)
	// 	{
	// 		if (*it == tmp2[i])
	// 		{
	// 			char charTmp2[2] = { tmp2[i], '\0' };
	// 			std::string msg = ": already unset mode '-" + std::string(charTmp2) + "'";
	// 			_server.send_message_to_fd(_client.getFd(), msg);
	// 		}
	// 	}
	// 	switch(tmp2[i])
	// 	{
	// 		case 'o':
	// 		case 'i':
	// 			channel->setInviteOnly(false);
	// 		case 'k':
	// 			channel->unsetKey();
	// 		case 't':
	// 		case 'l':
	// 		default:
	// 			// code 472 "<char> :is unknown mode char to me"
	// 			throw std::invalid_argument(":is unknown mode char to me\n");
	// 	}
	// }

/*
//         MODE - 채널 모드 및 구성원의 권한 변경
//		         +o : 채널 운영자 권한 부여/회수
//               +i : 초대전용 채널 설정/해제
//               +k : 채널 비밀번호 설정/해제
//               +t : 채널 주제 변경 권한 설정/해제
//               +l : 채널 최대 사용자 수 설정/해제
*/