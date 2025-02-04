NAME 	= ircserv
COMPILE = c++ -W -W -W -std=c++98
SRC		= main.cpp src/ft_irc.cpp src/ft_commands.cpp src/Client.cpp src/ADMIN.cpp \
		  src/USER.cpp src/PASS.cpp src/NICK.cpp src/QUIT.cpp src/WHO.cpp src/JOIN.cpp src/Channel.cpp \
		  src/PART.cpp src/PRIVMSG.cpp src/KICK.cpp src/MODE.cpp src/INVITE.cpp \
		  src/TOPIC.cpp
OBJ		= $(SRC:.cpp=.o)
HEADS	= include/ft_irc.hpp include/ft_commands.hpp include/Client.hpp

all: $(NAME)

$(NAME): $(OBJ)
	$(COMPILE) $(OBJ) -o $(NAME)


%.o: %.cpp $(HEADS)
	$(COMPILE) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean
