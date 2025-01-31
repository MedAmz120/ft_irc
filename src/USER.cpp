/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   USER.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moamzil <moamzil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 22:33:15 by moamzil           #+#    #+#             */
/*   Updated: 2025/01/31 22:33:16 by moamzil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_commands.hpp"

bool CommandHandler::isValidCharacter(unsigned char c) {
    return isdigit(c) || isalpha(c) || c == '_' || c == '-' || c == '.' || ':';
}

bool    CommandHandler::check_username(const std::string& str) {
    if (str.empty())
        return false;
    if (str.size() < 6)
        return false;

    size_t digit_count = 0;
    size_t numeric_count = 0;

    for (size_t i = 0; i < str.size(); i++) {
        if (!isValidCharacter(static_cast<unsigned char>(str[i])))
            return false;
        if (isdigit(static_cast<unsigned char>(str[i])))
            digit_count++;
        if (!isalpha(static_cast<unsigned char>(str[i])) && !isdigit(static_cast<unsigned char>(str[i])) && str[i] != ':')
            numeric_count++; 
    }

    if (digit_count == str.size())
        return false;
    if (numeric_count == str.size())
        return false;
    return true;
}

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
        for (int i = 1; i <= 4; i++)
        {
            if (!check_username(command_args[i])) {
                sendMessageToClient(client, "Error: Invalid format for " + command_args[i] + '\n');
                command_args.clear();
                return ;
            }
        }
        client.SetUserName(command_args[1]);
        client.SetHostName(command_args[2]);
        client.SetServerName(command_args[3]);
        client.SetRealName(command_args[4]);
        sendMessageToClient(client, "Personal Info set successfully !\n");
        client.SetAuth();
        std::cout << "Log: "<<  "User " << client.getClientFd() << " Set Username to "  << client.getUser() << std::endl;
    }
    command_args.clear();
}