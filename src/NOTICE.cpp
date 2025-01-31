/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NOTICE.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moamzil <moamzil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 22:32:09 by moamzil           #+#    #+#             */
/*   Updated: 2025/01/31 23:20:05 by moamzil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_commands.hpp"

void    CommandHandler::execute_NOTICE(Client& sender, Server& server, Channel& Mainchannel) {
    // Check if the sender is authenticated
    if (!sender.getAuth()) {
        sendMessageToClient(sender, "Error: You must be authenticated to use the NOTICE command.\n");
        command_args.clear();
        return;
    }

    // Ensure the NOTICE command includes both a target nickname and a message
    if (command_args.size() < 3) {
        sendMessageToClient(sender, "Error: Bad Usage NOTICE <nickname> <message>\n");
        command_args.clear();
        return;
    }

    std::string targetNickname = command_args[1];
    std::string message = command_args[2];

    // Check if the message starts with ':' to include the entire message text
    if (command_args[2][0] == ':') {
        // Concatenate all parts of the message if split into multiple segments
        for (size_t i = 3; i < command_args.size(); i++) {
            message += " " + command_args[i];
        }
        // Remove the leading ':' for correct message formatting
        message.erase(0, 1);
    }

    // Ensure the sender does not send a NOTICE to themselves
    if (targetNickname == sender.getNickname()) {
        sendMessageToClient(sender, "Error: You cannot send a NOTICE to yourself.\n");
        command_args.clear();
        return;
    }


    if (server.isClientInServer(command_args[1]) && command_args[1][0] != '#') {
        const Client& recipient = server.getRecipient(command_args[1]);
        std::string noticeMessage = ":NOTICE from " + sender.getNickname() + " :" + message + '\n';
        sendMessageToClient(recipient, noticeMessage);
        std::cout << "NOTICE sent from " << sender.getNickname() << " to " << recipient.getNickname() << ": " << message << std::endl;
        command_args.clear();
    }
    else if (command_args[1][0] != '#') {
            Channel *channel = Mainchannel.getChannel(command_args[1]);
            if (channel == nullptr) {
                sendMessageToClient(sender, "Error: The specified channel does not exist.\n");
                command_args.clear();
                return ;
            }
            else {
                if (channel->isUserInChannel(&sender)) {
                    channel->broadcastMessage("Notice: from" + sender.getNickname() + ": "+ message);
                    std::cout << "Log: "<< sender.getNickname() << " send Notice \"" << message << "\" To " << command_args[1] << std::endl;           
                    command_args.clear();
                    return ;
                }
                else {
                    sendMessageToClient(sender, "Error: You must be a member of the channel to send Notices to it\n");
                    command_args.clear();
                }
            }        
    }
    else
        sendMessageToClient(sender, "Error: No such user or channel exists. Make sure the nickname is correct.\n");
    
    command_args.clear();
}