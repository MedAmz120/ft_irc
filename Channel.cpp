/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychihab <ychihab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 20:30:55 by ychihab           #+#    #+#             */
/*   Updated: 2025/01/26 00:41:56 by ychihab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <sstream>
#include <algorithm>
#include <sys/socket.h>

std::map<std::string, Channel *> Channel::channels;

Channel::Channel() : clientLimit(0), isInviteOnly(false), isTopicRestricted(false) {}

Channel::Channel(const std::string &name) : name(name), clientLimit(0), isInviteOnly(false), isTopicRestricted(false) {}

Channel *Channel::createChannel(const std::string &name, Client *creator)
{
    if (channels.find(name) != channels.end())
    {
        return (NULL);
    }
    Channel *newChannel = new Channel(name);
    newChannel->addClient(creator);
    newChannel->addOperator(creator);
    channels[name] = newChannel;
    return newChannel;
}

Channel *Channel::getChannel(const std::string &name)
{
    std::map<std::string, Channel *>::iterator it = channels.find(name);
    if (it != channels.end())
    {
        return (it->second);
    }
    return NULL;
}

bool Channel::deleteChannel(const std::string &name)
{
    std::map<std::string, Channel *>::iterator it = channels.find(name);
    if (it != channels.end())
    {
        delete it->second;
        channels.erase(it);
        return (true);
    }
    return (false);
}

bool Channel::addClient(Client *client, const std::string &clientProvidedPassword)
{
    if (isClientLimitReached())
    {
        return false;
    }

    if (isInviteOnly && !isClientInvited(client))
    {
        return false;
    }

    if (!password.empty() && clientProvidedPassword != password)
    {
        return false;
    }

    if (clientsMap.find(client->nickname) == clientsMap.end())
    {
        clientsMap[client->nickname] = client;
        client->joinChannel(name);
        return true;
    }

    return false;
}

void Channel::removeClient(Client *client)
{
    std::map<std::string, Client *>::iterator it = clientsMap.find(client->nickname);
    if (it != clientsMap.end())
    {
        clientsMap.erase(it);
        client->leaveChannel(name);
    }
}

bool Channel::isClientInChannel(Client *client) const
{
    return (clientsMap.find(client->nickname) != clientsMap.end());
}

bool Channel::isClientInvited(Client *client) const
{
    return (std::find(invitedClients.begin(), invitedClients.end(), client) != invitedClients.end());
}

void Channel::addOperator(Client *client)
{
    if (std::find(operators.begin(), operators.end(), client) == operators.end())
    {
        operators.push_back(client);
    }
}

void Channel::removeOperator(Client *client)
{
    std::vector<Client *>::iterator it = std::find(operators.begin(), operators.end(), client);
    if (it != operators.end())
    {
        operators.erase(it);
    }
}

bool Channel::isOperator(Client *client) const
{
    return (std::find(operators.begin(), operators.end(), client) != operators.end());
}

void Channel::broadcastMessage(const std::string &message)
{
    for (std::map<std::string, Client *>::iterator it = clientsMap.begin(); it != clientsMap.end(); ++it)
    {
        send(it->second->socket, message.c_str(), message.size(), 0);
    }
}

void Channel::setInviteOnly(bool inviteOnly)
{
    isInviteOnly = inviteOnly;
}

bool Channel::getInviteOnly() const
{
    return (isInviteOnly);
}

void Channel::setPassword(const std::string &password)
{
    this->password = password;
}

std::string Channel::getPassword() const
{
    return (password);
}

void Channel::setClientLimit(int limit)
{
    clientLimit = limit;
}

bool Channel::isClientLimitReached() const
{
    if (clientLimit == 0)
        return (false);
    return (clientsMap.size() >= (size_t)(clientLimit));
}

std::string Channel::getName() const
{
    return (name);
}

std::string Channel::getTopic() const
{
    return (topic);
}

std::vector<Client *> Channel::getClients() const
{
    std::vector<Client *> clients;
    for (std::map<std::string, Client *>::const_iterator it = clientsMap.begin(); it != clientsMap.end(); ++it)
    {
        clients.push_back(it->second);
    }
    return (clients);
}

std::vector<Client *> Channel::getOperators() const
{
    return (operators);
}

void Channel::kickClient(Client *operatorClient, Client *targetClient, const std::string &reason)
{
    if (!isOperator(operatorClient))
    {
        return;
    }
    if (!isClientInChannel(targetClient))
    {
        return;
    }
    removeClient(targetClient);
    std::string kickMessage = ":" + operatorClient->nickname + " KICK " + name + " " + targetClient->nickname + " :" + reason + "\r\n";
    broadcastMessage(kickMessage);
}

void Channel::inviteClient(Client *operatorClient, Client *targetClient)
{
    if (!isOperator(operatorClient))
    {
        return;
    }

    if (isClientInChannel(targetClient))
    {
        return;
    }
    invitedClients.push_back(targetClient);
    std::string inviteMessage = ":" + operatorClient->nickname + " INVITE " + targetClient->nickname + " " + name + "\r\n";
    send(targetClient->socket, inviteMessage.c_str(), inviteMessage.size(), 0);
}

void Channel::setTopic(Client *client, const std::string &newTopic)
{
    if (isTopicRestricted && !isOperator(client))
    {
        return;
    }
    topic = newTopic;
    std::string topicMessage = ":" + client->nickname + " TOPIC " + name + " :" + topic + "\r\n";
    broadcastMessage(topicMessage);
}

bool Channel::getTopic(Client *client)
{
    if (!isClientInChannel(client))
    {
        return (false);
    }
    std::string topicMessage = ":" + client->nickname + " 332 " + name + " :" + topic + "\r\n";
    send(client->socket, topicMessage.c_str(), topicMessage.size(), 0);
    return true;
}

void Channel::setMode(Client *client, const std::string &mode, const std::string &argument)
{
    if (!isOperator(client))
    {
        return;
    }
    if (mode == "+i")
    {
        isInviteOnly = true;
    }
    else if (mode == "-i")
    {
        isInviteOnly = false;
    }
    else if (mode == "+k")
    {
        setPassword(argument);
    }
    else if (mode == "-k")
    {
        setPassword("");
    }
    else if (mode == "+o")
    {
        Client *targetClient = findClientByNickname(argument);
        if (targetClient)
        {
            addOperator(targetClient);
        }
    }
    else if (mode == "-o")
    {
        Client *targetClient = findClientByNickname(argument);
        if (targetClient)
        {
            removeOperator(targetClient);
        }
    }
    else if (mode == "+l")
    {
        int limit;
        std::istringstream iss(argument);
        if (!(iss >> limit) || limit < 0)
        {
            return;
        }
        setClientLimit(limit);
    }
    else if (mode == "-l")
    {
        setClientLimit(0);
    }
    std::string modeMessage = ":" + client->nickname + " MODE " + name + " " + mode + " " + argument + "\r\n";
    broadcastMessage(modeMessage);
}

Client *Channel::findClientByNickname(const std::string &nickname)
{
    std::map<std::string, Client *>::iterator it = clientsMap.find(nickname);
    if (it != clientsMap.end())
    {
        return (it->second);
    }
    return (NULL);
}