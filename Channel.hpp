#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include "Server.hpp"
# include "Parser.hpp"
# include "Bot.hpp"
# include <iostream>
# include <vector>
# include <exception>

class Server;
class Client;
class Parser;
class Bot;


class Channel
{
	private:
		Bot *bot;
		std::vector<Client *> op;
		std::vector<Client *> invited;
		std::string name;
		std::string topic;
		std::string key;
		size_t limit;

		std::vector<std::string> setmodes;
		bool invite_only;
		bool use_key;
		bool topic_set;
		bool limit_set;
		bool bot_set;

		Channel();
		Channel(const Channel &f);
		Channel &operator=(const Channel &f);

	public:
		Channel(const std::string &name, const std::string &key);
		~Channel();

		std::string getName(void) const;
		std::string getTopic(void) const;
		std::string getKey(void) const;
		bool getInviteOnly(void) const;
		bool getTopicSet(void) const;
		bool getLimitSet(void) const;
		bool getUseKey(void) const;
		std::vector<Client *> &getInvited(void);
		std::vector<std::string> &getSetModes(void);
		Bot *getBot(void);

		void setOperator(Client &client);
		void setTopic(const std::string &topic);
		void setInviteOnly(bool value);
		void setKey(const std::string &key);
		void setLimit(size_t limit);
		void setBot(void);

		void unsetTopic(void);
		void unsetKey(void);
		void unsetLimit(void);
		void unsetOperator(Client &client);
		void unsetBot(void);
	
		bool addClient(Client &client);
		void deleteClient(const std::string &nickname);

		void printInfo(void);
		bool checkKey(const std::string &key);
		bool is_operator(const Client &client);
		bool is_invited(const std::string &nickname);
		bool is_bot();
};

#endif


/*
	모드 
    +o : 채널 운영자 권한 부여/회수
    +i : 초대전용 채널 설정/해제
    +k : 채널 비밀번호 설정/해제
	+t : 채널 주제 변경 권한 설정/해제
    +l : 채널 최대 사용자 수 설정/해제
*/