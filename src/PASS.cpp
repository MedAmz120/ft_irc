#include "../include/ft_irc.hpp"

void    CommandHandler::execute_PASS(Client& client) {
    if (client.getAuth() == 1)
    {
        command_args.clear();
        sendMessageToClient(client, "Your are already authenticated\n");  
        return ;
    }
    else if (command_args.size() != 2) {
        command_args.clear();
        sendMessageToClient(client, "Error: Bad usage <password>\n");
    }
    else if (command_args.size() - 1 <= 2) {
        if (command_args[1] != this->password) {
            command_args.clear();
            sendMessageToClient(client, "Password Incorrect\n");
        }
        else
        {
            sendMessageToClient(client, "Welcome to the server! You are now authenticated!\n");
            client.SetAuth();
            command_args.clear();
            std::cout << "Log: "<<  "User " << client.getClientFd() << " authenticated successfully." << std::endl;
            return ;
        }
    }
    command_args.clear();
}