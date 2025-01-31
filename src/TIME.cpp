/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TIME.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moamzil <moamzil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 22:33:01 by moamzil           #+#    #+#             */
/*   Updated: 2025/01/31 22:33:03 by moamzil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_commands.hpp"

void    CommandHandler::execute_TIME(Client& client) {
    // Get current server time
    std::time_t now = std::time(nullptr);
    std::string timeString = std::ctime(&now); // Convert time to string format

    // Trim the newline character that ctime adds at the end of the time string
    timeString.erase(timeString.length() - 1);

    std::string message = ":ircserv 391 " + client.getNickname() + " :Local time on the server is " + timeString;

    sendMessageToClient(client, message + '\n');
    command_args.clear();
}