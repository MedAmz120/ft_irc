#include "../include/ft_commands.hpp"


bool    isValidChannelName(const std::string& name) {
    for (size_t i = 0; i < name.length(); ++i) {
        char c = name[i];
        if (!(std::isalnum(c) && c != '_' && c != '-' && c != '.') && c != '#') {
            return false;
        }
    }
    return true;
}

bool    CommandHandler::check_channel_name(const std::string& name, Client& client) {
    if (name.length() > 50) {
        sendMessageToClient(client, "Error: Channel name cannot exceed 50 characters.\n");
        return false;
    }
    else if (name[0] != '#') {
        sendMessageToClient(client, "Error: Channel name must start with # <#name>\n");
        return false;
    }
    else if (!isValidChannelName(name)) {
        sendMessageToClient(client, "Error: Channel name can only contain alphanumeric characters, underscores (_), hyphens (-), and periods (.)\n");
        return false;
    }
    else
        return true;
}

void    CommandHandler::execute_JOIN(Client& client, Channel& Mainchannel) {
    if (client.getAuth() == 0) {
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
        command_args.clear();
        return ;
    }
    else if (client.getUser() == "NULL")
    {
        sendMessageToClient(client, "Error: You must set both a nickname and a username before joining a channel.\n");
        command_args.clear();
        return ;
    }
    else if (command_args.size() > 3 || command_args.size() < 2) {
        sendMessageToClient(client, "Error: Bad usage JOIN <NickName>\n");
        command_args.clear();
        return ;
    }
    else if (!check_channel_name(command_args[1], client)) {
        command_args.clear();
        return ;
    }
    check_channel_name (command_args[1], client);
    std::string channelName = command_args[1];
    Channel* channel = Channel::createChannel(channelName, &client);


    if (channel == nullptr) {
        // Channel already exists, retrieve the existing channel
        channel = Mainchannel.getChannel(command_args[1]);
        if (!channel->getPassword().empty() && command_args.size() != 3) { // check if the channel is password protected
            sendMessageToClient(client, "Error: Cannot Join " + channelName + " The channel is password protected\n");
            command_args.clear();
            return ;
        }
        if (channel->isUserInChannel(&client)) {
            sendMessageToClient(client, "You are already in the channel: " + channelName + "\n");
        } 
        else 
        {
            if (!channel->getPassword().empty()) 
            {
                if (channel->getPassword() == command_args[2]) {
                    if (channel->addUser(&client)) {
                        sendMessageToClient(client, "You have joined the channel: " + channelName + "\n");
                        channel->broadcastMessage(client.getNickname() + " has joined the channel.\n");
                        std::cout << "Log: "<< client.getUser() << " has joined the channel " << command_args[1] << std::endl;
                    }  else {
                            sendMessageToClient(client, "Could not add you to the channel.\n");
                    }
                }
                else
                    sendMessageToClient(client, "Error: Cannot Join " + channelName + " Incorrect Password for channel\n");
            }
            else 
            {
                if (channel->addUser(&client)) {
                    sendMessageToClient(client, "You have joined the channel: " + channelName + "\n");
                    channel->broadcastMessage(client.getNickname() + " has joined the channel.\n");
                    std::cout << "Log: "<< client.getUser() << " has joined the channel " << command_args[1] << std::endl;
                }  
                else {
                        sendMessageToClient(client, "Could not add you to the channel.\n");
                }
            }
        }
    } 
    else {
        // New channel was created and the client was added
        channel->addUser(&client);
        sendMessageToClient(client, "You have successfully created and joined the channel: " + channelName + "\n");
        channel->broadcastMessage(client.getNickname() + " has created and joined the channel.\n");
        std::cout << "Log: " << client.getUser() << " has created and joined the channel " << command_args[1] << std::endl;
    }
    command_args.clear();
}