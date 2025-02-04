#include "../include/ft_irc.hpp"


void CommandHandler::execute_TOPIC(Client& client, Channel& Mainchannel) {
    if (client.getAuth() == 0) {
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
        command_args.clear();
        return;
    }
    else if (client.getUser() == "NULL")
    {
        sendMessageToClient(client, "Error: You must set both a nickname and a username before joining a channel.\n");
        command_args.clear();
        return ;
    }
    if (command_args.size() < 2) {  // TOPIC <#channel>
        sendMessageToClient(client, "Error: Bad usage TOPIC <#channel> [new topic]\n");
        command_args.clear();
        return;
    }

    std::string channelName = command_args[1];
    Channel* channel = Mainchannel.getChannel(channelName);

    if (!channel) {
        sendMessageToClient(client, "Error: The specified channel does not exist.\n");
        command_args.clear();
        return;
    }
    if (command_args.size() == 2) {
        sendMessageToClient(client, "Current topic for " + channelName + ": " + channel->getTopic());
    } 
    else  
    {
        std::string newTopic = command_args[2];
        channel->setTopic(&client, newTopic);
        // sendMessageToClient(client, "Topic changed for " + channelName + "." + '\n');
        // std::cout << "Log: " << client.getNickname() << " Set Topic to "  << channel->getTopic() << " To The channel name " << channel->getName() << std::endl;
    }
    command_args.clear();
}
