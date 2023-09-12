#include "../Parser.hpp"

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
    else if (!channel->getTopicSet())
    {
        this->_server.send_message_to_client_with_code(_client, "331", chan_name + " :No topic is set");
    }
    else if (!channel->is_operator(this->_client))
    {
        this->_server.send_message_to_client_with_code(_client, "482", chan_name + " :You're not channel operator");
    }
    else
    {
        this->_server.send_message_to_channel(chan_name, ":" + this->_client.getNickname() + "!" + this->_client.getUsername() + "@" + this->_client.getHostname() + " TOPIC " + chan_name + " :" + topic + "\r\n");
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

// 3.2.4 Topic message

//       Command: TOPIC
//    Parameters: <channel> [ <topic> ]

//    The TOPIC command is used to change or view the topic of a channel.
//    The topic for channel <channel> is returned if there is no <topic>
//    given.  If the <topic> parameter is present, the topic for that
//    channel will be changed, if this action is allowed for the user
//    requesting it.  If the <topic> parameter is an empty string, the
//    topic for that channel will be removed.

//    Numeric Replies:

//            ERR_NEEDMOREPARAMS              ERR_NOTONCHANNEL
//            RPL_NOTOPIC                     RPL_TOPIC
//            ERR_CHANOPRIVSNEEDED            ERR_NOCHANMODES

//    Examples:

//    :WiZ!jto@tolsun.oulu.fi TOPIC #test :New topic ; User Wiz setting the
//                                    topic.

//    TOPIC #test :another topic      ; Command to set the topic on #test
//                                    to "another topic".

//    TOPIC #test :                   ; Command to clear the topic on
//                                    #test.

//    TOPIC #test                     ; Command to check the topic for
//                                    #test.