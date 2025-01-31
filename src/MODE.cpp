/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MODE.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moamzil <moamzil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 22:31:50 by moamzil           #+#    #+#             */
/*   Updated: 2025/01/31 22:56:13 by moamzil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_commands.hpp"

bool    check_L_mode_arg(const std::string& str) // check the arguments if it s a valid number not a combination of chars
{
    for (size_t i = 0; i < str.length(); ++i) {
        if (!isdigit(static_cast<unsigned char>(str[i]))) {
            return false;  // Return false if a non-digit is found
        }
    }
    return true;  // Return true if all characters are digits
}

bool isLKOMode(const std::string& mode) {
    // Check if the string matches any of the specific modes with either '+' or '-' prefix
    return mode == "+l" || mode == "-l" ||
           mode == "+k" || mode == "-k" ||
           mode == "+o" || mode == "-o";
}

bool isTIMode(const std::string& mode) {
    // Check if the string matches any of the specific modes with either '+' or '-' prefix
    return mode == "+t" || mode == "-t" ||
           mode == "+i" || mode == "-i";
}



bool    CommandHandler::check_mode(const std::string& mode, Client& client, const std::string& command_args)
{
    if (!isLKOMode(mode) && !isTIMode(mode)) 
    {
        sendMessageToClient(client, "Error: Invalid Mode\n");
        return false;
    }
    else if ((mode == "+l" || mode == "-l") && !check_L_mode_arg(command_args)) 
    {
        sendMessageToClient(client, "Error: Bad usage MODE <#channel> <+l> <0-50...>\n");
        return false;
    }
    return true;
}


void    CommandHandler::execute_MODE(Client& client, Channel& Mainchannel, Server& server) {
    if (client.getAuth() == 0)
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
    else if (client.getUser() == "NULL")
        sendMessageToClient(client, "Error: You must set both a nickname and a username before joining a channel.\n");  
    else if ((command_args.size() != 4 && isLKOMode(command_args[2])) 
        || ((command_args.size() != 3 && isTIMode(command_args[2]))
        || command_args.size() > 4))
    {
        sendMessageToClient(client, "Error: Bad usage MODE <#channel> <+/-mode>\n");
    }
    else 
    {
        Channel *channel = Mainchannel.getChannel(command_args[1]);
        if (channel == nullptr)
        {
            sendMessageToClient(client, "Error: The specified channel does not exist.\n");
            command_args.clear();
            return ;
        }
        else 
        {
            if (channel->isOperator(&client)) 
            {
                if (check_mode(command_args[2], client, command_args[3])) {
                    // comment was here
                    if (!channel->isUserInChannel(&server.getUserToKick(command_args[3]))) {
                        sendMessageToClient(client, "Error: The specified User does not exist.\n");
                        command_args.clear();
                        return ;
                    }
                    channel->setMode(&client, command_args[2], command_args[3]);
                    std::cout << "Log: Execute: "<< client.getNickname() << " MODE " << command_args[1]  + " "<< command_args[2] + " ";
                    std::cout << command_args[3] << std::endl;
                }
            }
            else
                sendMessageToClient(client, "Error: You Must be the Channel Operator to use MODE\n");

    }             
    }
    command_args.clear();
}

/*
                    if (command_args[1] == "+l")
                        channel->setUserLimit(command_args[1]);
                    else if (command_args[1] == "+k")
                        channel->setPassword(command_args[3]);
                    else if (command_args[1] == "+o") {
                        Client& user_to_make_operator = server.getUserToKick(command_args[2]);
                        if (channel->isUserInChannel(&user_to_make_operator)) 
                        {
                            channel->addOperator(&user_to_make_operator);
                            sendMessageToClient(client, "User Is now Operator on your channel\n");
                            std::cout << "Log: "<< client.getUser() << " ADD Operator " << user_to_make_operator.getNickname() << " to ";
                            std::cout << command_args[1] << std::endl;
                        }
                        else
                            sendMessageToClient(client, "Error: User does not found in channel\n");                     
                    }
                    else if (command_args[1] == "+i")
                        channel->setInviteOnly(true);                    
                    else if (command_args[1] == "-i")
                        channel->setInviteOnly(false);
                    else if (command_args[1] == "t")
                        channel->setMode(&client, );
*/