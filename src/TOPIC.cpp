/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TOPIC.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moamzil <moamzil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 22:33:07 by moamzil           #+#    #+#             */
/*   Updated: 2025/01/31 23:02:39 by moamzil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_commands.hpp"


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
    else if (command_args.size() == 2 && !channel->getTopic().empty()) {
        if (channel->isUserInChannel(&client))
            sendMessageToClient(client, "Current topic for " + channelName + ": " + channel->getTopic() + '\n');
        else
            sendMessageToClient(client, "Error: You Must be a channel member to display the channel Topic\n");
    } 
    else if (channel->getTopic().empty() && command_args.size() == 2) {
        sendMessageToClient(client, "No Topic is set for channel yet\n");
    }
    else if (!channel->isUserInChannel(&client) && command_args.size() >= 3) {
        sendMessageToClient(client, "Error: You Must be a member to change or display the channel Topic\n");
    }
    else if (!channel->isOperator(&client) && command_args.size() >= 3) {
        sendMessageToClient(client, "Error: Only Channel operators can change Channel Topic\n");
    }
    else if (channel->getTopicRestricted()) {
        sendMessageToClient(client, "Error: Topic Restricted Mode is Activated on Channel cannot change\n");
    }
    else
    {
        std::string newTopic = command_args[2];
        channel->setTopic(&client, newTopic);
        sendMessageToClient(client, "Topic changed for " + channelName + "." + '\n');
        std::cout << "Log: " << client.getNickname() << " Set Topic to "  << channel->getTopic() << " To The channel name " << channel->getName() << std::endl;
    }
    command_args.clear();
}
