#include "../include/ft_irc.hpp"

void    CommandHandler::execute_USER(Client& client) {
    if (client.getAuth() == 0) {
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
    }
    else if (client.getNickname() == "NULL") {
        sendMessageToClient(client, "You must First set your Nickname with NICK <nickname>.\n");
    }
    else if (command_args.size() != 5) {
        sendMessageToClient(client, "Error: Bad usage USER <username> <hostname> <servername> :<realname>\n");
    }
    else
    {
        client.SetUserName(command_args[1]);
        client.SetHostName(command_args[2]);
        client.SetServerName(command_args[3]);
        client.SetRealName(command_args[4]);
        sendMessageToClient(client, "Personal Info set successfully !\n");
        client.SetAuth();
        std::cout << "User " << client.getClientFd() << " Set Username to "  << client.getUser() << std::endl;
    }
    command_args.clear();
}