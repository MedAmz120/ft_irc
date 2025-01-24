#include "../include/ft_irc.hpp"

void    CommandHandler::execute_PART(Client& client, Channel& Mainchannel) {
    if (client.getAuth() == 0) {
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
        command_args.clear();
        return ;
    }
    else if (client.getUser() == "NULL")
    {
        sendMessageToClient(client, "Error: You must set both a nickname and a username before.\n");
        command_args.clear();
        return ;
    }
    else if (command_args.size() != 2) {
        sendMessageToClient(client, "Error: Bad usage PART <channel>\n");
        command_args.clear();
        return ;
    }
    Channel* channel = Mainchannel.getChannel(command_args[1]);
    if (channel == nullptr) {
        sendMessageToClient(client, "Error: The specified channel does not exist.\n");
        command_args.clear();
        return ;
    }
    else {
        if (channel->isUserInChannel(&client)) {
            channel->removeUser(&client);
            sendMessageToClient(client, "You have been successfully removed from the channel.\n");
            std::cout << "Log: "<< client.getUser() << " left the channel name " << command_args[1] << std::endl;
        }
        else {
            sendMessageToClient(client, "Error: You are not a member of the specified channel.\n");
            command_args.clear();
            return ;
        }
    }
    command_args.clear();
}