NAME = ircserv

SRCS = main.cpp src/server.cpp src/ft_commands.cpp src/Client.cpp src/ADMIN.cpp \
		  src/USER.cpp src/PASS.cpp src/NICK.cpp src/QUIT.cpp src/WHO.cpp src/JOIN.cpp src/Channel.cpp \
		  src/PART.cpp src/PRIVMSG.cpp src/KICK.cpp src/MODE.cpp src/INVITE.cpp \
		  src/TOPIC.cpp src/TIME.cpp src/NOTICE.cpp

Include = include/server.hpp include/Channel.hpp include/ft_irc.hpp include/ft_commands.hpp include/Client.hpp

OBJS = $(SRCS:.cpp=.o)
CC = c++
RM = rm -rf
CFLAGS = -Wall -Wextra -Werror -std=c++98

%.o : %.cpp ${Include}
		$(CC) $(CFLAGS) -g -c $< -o $@

all:		$(NAME) 

$(NAME): $(OBJS)
			$(CC) $(OBJS) -o $(NAME)

clean:
				$(RM) $(OBJS) 
			
fclean:			clean
				$(RM) $(NAME)

re:				fclean $(NAME)

.PHONY: all fclean clean re