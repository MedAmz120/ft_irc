/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychihab <ychihab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:45:12 by ychihab           #+#    #+#             */
/*   Updated: 2025/01/25 22:32:56 by ychihab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <vector>
#include "Channel.hpp"
#include "Client.hpp"

int main()
{
    std::vector<Client *> clients;

    int numClients;
    std::cout << "Enter the number of clients you want to create: ";
    std::cin >> numClients;

    std::cout << "\n=== Creating Clients ===" << std::endl;
    for (int i = 0; i < numClients; ++i)
    {
        std::string nickname, username, hostname;
        int socket;

        std::cout << "\nEnter details for Client " << (i + 1) << ":" << std::endl;
        std::cout << "Nickname: ";
        std::cin >> nickname;
        std::cout << "Username: ";
        std::cin >> username;
        std::cout << "Hostname: ";
        std::cin >> hostname;
        std::cout << "Socket: ";
        std::cin >> socket;

        Client *newClient = new Client(nickname, username, hostname, socket);
        clients.push_back(newClient);
    }

    std::string channelName;
    std::cout << "\nEnter the name of the channel to create: ";
    std::cin >> channelName;

    Channel *channel = Channel::createChannel(channelName, clients[0]);
    if (!channel)
    {
        std::cerr << "Failed to create the channel '" << channelName << "'." << std::endl;
        return 1;
    }

    std::cout << "Channel '" << channelName << "' created successfully by " << clients[0]->nickname << "." << std::endl;

    std::cout << "\n=== Adding Clients to the Channel ===" << std::endl;
    for (std::size_t i = 1; i < clients.size(); ++i)
    {
        if (channel->addClient(clients[i], ""))
        {
            std::cout << clients[i]->nickname << " has joined the channel '" << channelName << "'." << std::endl;
        }
        else
        {
            std::cerr << "Error: Failed to add " << clients[i]->nickname << " to the channel." << std::endl;
        }
    }

    channel->setInviteOnly(true);
    std::cout << "\nThe channel is now in invite-only mode." << std::endl;

    if (!channel->addClient(clients.back(), ""))
    {
        std::cout << clients.back()->nickname << " could not join the channel '" << channelName << "' because it's invite-only." << std::endl;
    }

    channel->inviteClient(clients[0], clients.back());
    if (channel->addClient(clients.back(), ""))
    {
        std::cout << clients.back()->nickname << " has joined the channel '" << channelName << "' after being invited." << std::endl;
    }

    std::string newTopic;
    std::cout << "\nEnter a new topic for the channel '" << channelName << "': ";
    std::cin.ignore();
    std::getline(std::cin, newTopic);
    channel->setTopic(clients[0], newTopic);
    std::cout << "The channel's topic is now: " << channel->getTopic() << std::endl;

    std::string reason;
    std::cout << "\nEnter a reason to kick the second client (" << clients[1]->nickname << "): ";
    std::getline(std::cin, reason);
    channel->kickClient(clients[0], clients[1], reason);
    if (!channel->isClientInChannel(clients[1]))
    {
        std::cout << clients[1]->nickname << " has been kicked from the channel '" << channelName << "'." << std::endl;
    }

    Channel::deleteChannel(channelName);
    std::size_t i = 0;
    while (i < clients.size())
    {
        delete clients[i];
        i++;
    }

    std::cout << "\nProgram finished successfully. All resources have been freed." << std::endl;
    return 0;
}