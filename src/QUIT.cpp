#include "../include/ft_irc.hpp"

void    CommandHandler::execute_QUIT(Client& client, ft_irc& server) {
    sendMessageToClient(client, "By By :( !\n");
    std::cout << "User " << client.getClientFd() << " Has Quit the server " << std::endl;
    close(client.getClientFd());
    server.close_client_session(client.getClientFd());
    command_args.clear();
}