#include "include/ft_irc.hpp"

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cout << "Usage: ./ircserv <port> <password>";
        exit (EXIT_FAILURE);
    }

    int  port = std::stoi(av[1]);
    ft_irc  server(port, av[2]);
    server.start();
}