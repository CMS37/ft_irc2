#ifndef PARSER_HPP
# define PARSER_HPP

# include<iostream>
# include <vector>
# include"Client.hpp"
# include"utils.hpp"
# include"Bot.hpp"
# include <ctime>
# include <time.h>
# include <string>
# include <sstream>


class Server;
class Client;
class Bot;


class Parser
{
    private:
        Server& _server;
        Client& _client;
        std::string _str;
        std::vector<std::string> _tokens;
        Parser();
    public:
        Parser(Server& server, Client& client, std::string str);
        ~Parser();

        void what_is_this();
        void setString(std::string str);

        void cmd_cap();
        void cmd_join();
        void cmd_list();
        void cmd_ping();
        void cmd_nick();
        void cmd_part();
        void cmd_user();
        void cmd_privmsg();
        void cmd_quit();
        void cmd_time();
        void cmd_invite();
        void cmd_kick();
        void cmd_topic();
        void cmd_mode(const std::string &mode);
        void cmd_mode_user(const std::vector<std::string> &str);
        void cmd_mode_channel(const std::vector<std::string> &str);
        void mode_list_channel(const std::vector<std::string> &str);
        void cmd_pass();
        void cmd_who();
        void cmd_whois();
};


#endif