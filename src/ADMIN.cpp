#include "../include/ft_irc.hpp"

void    CommandHandler::execute_ADMIN(Client& client) {
    if (client.getAuth() == 0) {
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
    }
    command_args.clear();
}