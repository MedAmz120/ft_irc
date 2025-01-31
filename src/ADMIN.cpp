/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ADMIN.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moamzil <moamzil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 22:30:52 by moamzil           #+#    #+#             */
/*   Updated: 2025/01/31 22:30:53 by moamzil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_commands.hpp"

void    CommandHandler::execute_ADMIN(Client& client) {
    if (client.getAuth() == 0) {
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
    }
    else if (client.getUser() == "NULL")
    {
        sendMessageToClient(client, "Error: You must set both a nickname and a username before joining a channel.\n");
        command_args.clear();
        return ;
    }
    sendMessageToClient(client, ":ircserv 256 " + client.getNickname() + " :Administrative info about server.name\n");
    sendMessageToClient(client, ":ircserv 257 " + client.getNickname() + " :Server located in New York, USA\n");
    command_args.clear();
}