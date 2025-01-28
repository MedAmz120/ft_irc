/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychihab <ychihab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/05 21:45:12 by ychihab           #+#    #+#             */
/*   Updated: 2025/01/28 13:29:24 by ychihab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include "Client.hpp"
#include "Channel.hpp"

int main()
{
    Client client1("KHALID", "user1", "localhost", 1);
    Client client2("MOHAMED", "user2", "localhost", 2);
    Client client3("YOUSSEF", "user3", "localhost", 3);

    Channel *generalChannel = Channel::createChannel("#general", &client1);
    if (!generalChannel)
    {
        std::cerr << "Error: The #general channel already exists or could not be created." << std::endl;
        return 1;
    }

    generalChannel->addClient(&client1);
    generalChannel->addClient(&client2);
    generalChannel->addClient(&client3);

    std::cout << "Clients in #general:" << std::endl;
    std::vector<Client *> clients = generalChannel->getClients();
    std::vector<Client *>::const_iterator it = clients.begin();
    while (it != clients.end())
    {
        std::cout << "- " << (*it)->nickname << std::endl;
        ++it;
    }

    std::cout << "\n KHALID sends a message to #general:" << std::endl;
    generalChannel->broadcastMessage(":Alice PRIVMSG #general :Hello everyone!\r\n");

    generalChannel->addOperator(&client1);
    std::cout << "\n KHALID is now an operator of #general." << std::endl;

    std::cout << "\n KHALID kicks MOHAMED out of #general:" << std::endl;
    generalChannel->kickClient(&client1, &client2, "You have been kicked!");

    std::cout << "\nRemaining clients in #general:" << std::endl;
    clients = generalChannel->getClients();
    it = clients.begin();
    while (it != clients.end())
    {
        std::cout << "- " << (*it)->nickname << std::endl;
        ++it;
    }

    std::cout << "\n KHALID changes the topic of #general:" << std::endl;
    generalChannel->setTopic(&client1, "Welcome to #general!");

    std::cout << "Topic of #general: " << generalChannel->getTopic() << std::endl;

    std::cout << "\n KHALID invites MOHAMED to join #general:" << std::endl;
    generalChannel->inviteClient(&client1, &client3);

    std::cout << "Clients invited to #general:" << std::endl;
    const std::vector<Client *> &invitedClients = generalChannel->getInvitedClients();
    std::vector<Client *>::const_iterator inviteIt = invitedClients.begin();
    while (inviteIt != invitedClients.end())
    {
        std::cout << "- " << (*inviteIt)->nickname << std::endl;
        ++inviteIt;
    }

    std::cout << "\n Deleting the #general channel..." << std::endl;
    Channel::deleteChannel("#general");

    return 0;
}
