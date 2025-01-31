/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   QUIT.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moamzil <moamzil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/31 22:32:36 by moamzil           #+#    #+#             */
/*   Updated: 2025/01/31 22:32:40 by moamzil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_commands.hpp"

void    CommandHandler::execute_QUIT(Client& client, Server& server) {
    sendMessageToClient(client, "By By :( !\n");
    std::cout << "Log: " <<  "User " << client.getClientFd() << " Has Quit the server " << std::endl;
    close(client.getClientFd());
    server.close_client_session(client.getClientFd());
    command_args.clear();
}