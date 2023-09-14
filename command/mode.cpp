#include "../Parser.hpp"

void Parser::cmd_mode_channel(const std::vector<std::string> &str)
{
	Channel *channel = _server.getChannel(str[0]);
	if (channel == NULL)
	{
		// code 403 "<channel> :No such channel"
		this->_server.send_message_to_client_with_code(_client, "403", str[0] + " :No such channel");
		return ;
	}
	std::string ch = channel->getName();
	if (ch[0] == '+')
	{
		this->_server.send_message_to_channel(channel->getName(), ":" + _client.getNickname() + " MODE " + channel->getName() + " :Can't support MODE in this channel\r\n");
		return ;
	}
	if (!channel->is_invited(_client.getNickname()))
	{
		std::cout << _client.getNickname() << std::endl;
		// code 441 "<nickname> <channel> :They aren't on that channel"
		this->_server.send_message_to_client_with_code(_client, "441", _client.getNickname() + " " + str[0] + " :They aren't on that channel");
		return ;
	}
	else if (!channel->is_operator(_client))
	{
		// code 482 "<channel> :You're not channel operator"
		this->_server.send_message_to_client_with_code(_client, "482", str[0] + " :You're not channel operator");
		return ;
	}
	std::vector<std::string> seting;
	std::string argv;
	std::string set = str[1];
	std::string::iterator it = set.begin() + 1;
	size_t i = 2;
	bool pos = set[0] == '+'; // true: +, false: -

	for  ( ; it != set.end(); ++it)
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
					this->_server.send_message_to_client_with_code(_client, "461", "MODE :Not enough parameters (o)");
					return ;
				}
				else if (!channel->is_invited(str[i]))
				{
					this->_server.send_message_to_client_with_code(_client, "441", str[i] + " " + str[0] + " :They aren't on that channel");
					return ;
				}
				else
				{
					Client *client = _server.getClient(str[i]);
					if (pos)
					{
						seting.push_back("+o"); 
						channel->setOperator(*client);
					}
					else
					{
						seting.push_back("-o");
						channel->unsetOperator(*client);
					}
				}
				argv += str[i] + " ";
				i++;
				break;
			case 'i':
				if (pos)
				{
					seting.push_back("+i");
					channel->setInviteOnly(true);
				}
				else
				{
					seting.push_back("-i");
					channel->setInviteOnly(false);
				}
				break;
			case 'k':
				if (str.size() < i + 1)
				{
					this->_server.send_message_to_client_with_code(_client, "461", "MODE :Not enough parameters (k)");
					return ;
				}
				else
				{
					if (pos)
					{
						channel->setKey(str[i]);
						seting.push_back("+k");
						argv += str[i] + " ";
						i++;
					}
					else
					{
						channel->unsetKey();
						seting.push_back("-k");
					}
				}
				break;
			case 't':
				if (pos)
				{
					seting.push_back("+t");
					channel->setTopic("");
				}
				else
				{
					seting.push_back("-t");
					channel->unsetTopic();
				}
				i++;
				break;
			case 'l':
				if (!pos)
				{
					seting.push_back("-l");
					channel->unsetLimit();
				}
				else
				{
					if (str.size() < i + 1)
					{
						this->_server.send_message_to_client_with_code(_client, "461", "MODE :Not enough parameters (+l)");
						return ;
					}
					else if (!isNum(str[1]))
					{
						int limit_num = atoi(str[i].c_str());
						if (limit_num < 1)
						{
							this->_server.send_message_to_client_with_code(_client, "461", "MODE :Invalid number (+l)");
							return ;
						}
						channel->setLimit(limit_num);
						seting.push_back("+l");
						argv += str[i] + " ";
						i++;
					}
				}
				break;
			case 'e':
				break;
			default:
				// code 472 "<char> :is unknown mode char to me"
				std::string tmp = std::string(1, *it);
				std::string msg = tmp + " :is unknown mode char to me";
				this->_server.send_message_to_client_with_code(_client, "472", msg);
				return ;
		}
	}
	std::vector<std::string> &setmodes = channel->getSetModes();
	for (std::vector<std::string>::iterator it = seting.begin(); it != seting.end(); ++it)
	{
		if ((*it)[0] == '+')
		{
			bool flag = false;
			for (std::vector<std::string>::iterator it2 = setmodes.begin(); it2 != setmodes.end(); ++it2)
			{
				if (*it == *it2)
				{
					flag = true;
					break ;
				}
			}

			if (!flag)
				setmodes.push_back(*it);
		}
	}

	std::string tmp;
	for (std::vector<std::string>::iterator it = seting.begin(); it != seting.end(); ++it)
		tmp.append(*it + " ");
	std::cout << "=============MODE=============" << std::endl;
	std::cout << "seting: " << tmp << std::endl;
	std::cout << "argv: " << argv << std::endl;
	std::cout << "==============================" << std::endl;
	this->_server.send_message_to_channel(channel->getName(), ":" + _client.getNickname() + "!" + _client.getUsername() + "@" + _client.getHostname() +" MODE " + channel->getName() + " " + tmp + " :" + argv + "\r\n");
}

void Parser::cmd_mode_user(const std::vector<std::string> &str)
{
	if (_client.getNickname() == str[0])
	{
		std::string msg;
		// if (str[1] == "+i")
		// 	// _client->setHostname(); 서버시작할때 ip유무 물어보기
		// else if (str[1] == "-i")
		// 	// _client->setHostname(); 
		msg.append(":" + _client.getNickname() + "!" + _client.getUsername() + "@" + _client.getHostname() + " MODE " + _client.getNickname() + " " + str[1] + "\r\n");
		this->_server.send_message_to_fd_buffer(_client, msg);
	}
	else
		this->_server.send_message_to_client_with_code(_client, "502", " :Cant change mode for other users");
}

void Parser::mode_list_channel(const std::vector<std::string> &str)
{
	std::string msg;
	Channel *channel = _server.getChannel(str[0]);
	msg.append(":");
	msg.append(_server.getHostname());
	msg.append(" MODE ");
	msg.append(str[0]);
	msg.append(" :");
	std::vector<std::string> setModes = channel->getSetModes();
	std::vector<std::string>::iterator it = setModes.begin();
	for (; it != setModes.end(); ++it)
	{
		msg.append(*it);
		msg.append(" ");
	}
	msg.append("\r\n");
	this->_server.send_message_to_fd_buffer(_client, msg);
}

void Parser::cmd_mode(const std::string &mode)
{
	std::vector<std::string> str = split(mode, ' ');

	if (mode.empty())
	{
		this->_server.send_message_to_client_with_code(_client, "461", "MODE :Not enough parameters");
		return ;
	}

	if (str[0][0] == '#' || str[0][0] == '&' || str[0][0] == '!' || str[0][0] == '+')
	{
		if (str.size() == 1)
			mode_list_channel(str);
		else
			cmd_mode_channel(str);
	}
	else
		cmd_mode_user(str);
}

//         MODE - 채널 모드 및 구성원의 권한 변경
//		         +o : 채널 운영자 권한 부여/회수
//               +i : 초대전용 채널 설정/해제
//               +k : 채널 비밀번호 설정/해제
//               +t : 채널 주제 변경 권한 설정/해제
//               +l : 채널 최대 사용자 수 설정/해제

// 3.2.3 Channel mode message

//       Command: MODE
//    Parameters: <channel> *( ( "-" / "+" ) *<modes> *<modeparams> )

//    The MODE command is provided so that users may query and change the
//    characteristics of a channel.  For more details on available modes
//    and their uses, see "Internet Relay Chat: Channel Management" [IRC-
//    CHAN].  Note that there is a maximum limit of three (3) changes per
//    command for modes that take a parameter.

//    Numeric Replies:

//            ERR_NEEDMOREPARAMS              ERR_KEYSET
//            ERR_NOCHANMODES                 ERR_CHANOPRIVSNEEDED
//            ERR_USERNOTINCHANNEL            ERR_UNKNOWNMODE
//            RPL_CHANNELMODEIS
//            RPL_BANLIST                     RPL_ENDOFBANLIST
//            RPL_EXCEPTLIST                  RPL_ENDOFEXCEPTLIST
//            RPL_INVITELIST                  RPL_ENDOFINVITELIST
//            RPL_UNIQOPIS

//    The following examples are given to help understanding the syntax of
//    the MODE command, but refer to modes defined in "Internet Relay Chat:
//    Channel Management" [IRC-CHAN].

//    Examples:

//    MODE #Finnish +imI *!*@*.fi     ; Command to make #Finnish channel
//                                    moderated and 'invite-only' with user
//                                    with a hostname matching *.fi
//                                    automatically invited.

//    MODE #Finnish +o Kilroy         ; Command to give 'chanop' privileges
//                                    to Kilroy on channel #Finnish.

//    MODE #Finnish +v Wiz            ; Command to allow WiZ to speak on
//                                    #Finnish.

//    MODE #Fins -s                   ; Command to remove 'secret' flag
//                                    from channel #Fins.

//    MODE #42 +k oulu                ; Command to set the channel key to
//                                    "oulu".

//    MODE #42 -k oulu                ; Command to remove the "oulu"
//                                    channel key on channel "#42".

//    MODE #eu-opers +l 10            ; Command to set the limit for the
//                                    number of users on channel
//                                    "#eu-opers" to 10.

//    :WiZ!jto@tolsun.oulu.fi MODE #eu-opers -l
//                                    ; User "WiZ" removing the limit for
//                                    the number of users on channel "#eu-
//                                    opers".

//    MODE &oulu +b                   ; Command to list ban masks set for
//                                    the channel "&oulu".

//    MODE &oulu +b *!*@*             ; Command to prevent all users from
//                                    joining.

//    MODE &oulu +b *!*@*.edu +e *!*@*.bu.edu
//                                    ; Command to prevent any user from a
//                                    hostname matching *.edu from joining,
//                                    except if matching *.bu.edu

//    MODE #bu +be *!*@*.edu *!*@*.bu.edu
//                                    ; Comment to prevent any user from a
//                                    hostname matching *.edu from joining,
//                                    except if matching *.bu.edu

//    MODE #meditation e              ; Command to list exception masks set
//                                    for the channel "#meditation".

//    MODE #meditation I              ; Command to list invitations masks
//                                    set for the channel "#meditation".

//    MODE !12345ircd O               ; Command to ask who the channel
//                                    creator for "!12345ircd" is