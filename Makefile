NAME		=	ircserv

CXX			= 	c++
CXXFLAGS	= 	-Wall -Werror -Wextra -std=c++98

OBJS_DIR	= ./objs

SRCS =	main.cpp \
		Client.cpp \
		Server.cpp \
		Parser.cpp \
		Channel.cpp \
		Bot.cpp \
		utils.cpp \
		command/invite.cpp \
		command/join.cpp \
		command/kick.cpp \
		command/mode.cpp \
		command/nick.cpp \
		command/part.cpp \
		command/pass.cpp \
		command/ping.cpp \
		command/privmsg.cpp \
		command/quit.cpp \
		command/time.cpp \
		command/topic.cpp \
		command/user.cpp \
		command/who.cpp \
		command/whois.cpp \

OBJS := ${SRCS:%.cpp=${OBJS_DIR}/%.o}

all: ${NAME}

${OBJS_DIR}:
	@mkdir ${OBJS_DIR}
	@mkdir ${OBJS_DIR}/command

${NAME}: ${OBJS}
	@${CXX} -o ${NAME} ${OBJS}

${OBJS_DIR}/%.o: %.cpp | ${OBJS_DIR}
	@${CXX} ${CXXFLAGS} -g -c $< -o $@

clean:
	@rm -rf ${OBJS_DIR}

fclean:
	@rm -rf ${OBJS_DIR}
	@rm -f ${NAME}

re:
	@make fclean
	@make all


.PHONY: all clean fclean re