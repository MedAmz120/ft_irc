/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychihab <ychihab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 20:30:55 by ychihab           #+#    #+#             */
/*   Updated: 2025/01/28 13:38:26 by ychihab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include <sstream>
#include <algorithm>
#include <sys/socket.h>

std::map<std::string, Channel *> Channel::channels;

Channel::Channel() : clientLimit(0), isInviteOnly(false), isTopicRestricted(false) {}

Channel::Channel(const std::string &name) : name(name), clientLimit(0), isInviteOnly(false), isTopicRestricted(false) {}

Channel::~Channel()
{
    std::map<int, Client *>::iterator it = clientsMap.begin();
    while (it != clientsMap.end())
    {
        if (it->second)
        {
            it->second->leaveChannel(name);
        }
        ++it;
    }
    clientsMap.clear();
}

Channel *Channel::createChannel(const std::string &name, Client *creator)
{
    if (channels.find(name) != channels.end())
        return (NULL);
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
        return it->second;
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
        client->writeBuffer += ":server 471 " + name + " :Cannot join channel (+l)\r\n";
        return false;
    }

    if (isInviteOnly && !isClientInvited(client))
    {
        client->writeBuffer += ":server 473 " + name + " :Cannot join channel (+i)\r\n";
        return false;
    }

    if (!password.empty() && clientProvidedPassword != password)
    {
        client->writeBuffer += ":server 475 " + name + " :Cannot join channel (+k)\r\n";
        return false;
    }

    if (clientsMap.find(client->socket) == clientsMap.end())
    {
        clientsMap[client->socket] = client;
        client->joinChannel(name);
        return true;
    }
    return false;
}

void Channel::removeClient(Client *client)
{
    if (!isClientInChannel(client))
    {
        client->writeBuffer += ":server 442 " + name + " :You're not on that channel\r\n";
        return;
    }

    std::map<int, Client *>::iterator it = clientsMap.find(client->socket);
    if (it != clientsMap.end())
    {
        clientsMap.erase(it);
        removeOperator(client);
        client->leaveChannel(name);
    }
}

bool Channel::isClientInChannel(Client *client) const
{
    return (clientsMap.find(client->socket) != clientsMap.end());
}

const std::vector<Client *> &Channel::getInvitedClients() const
{
    return invitedClients;
}

bool Channel::isClientInvited(Client *client) const
{
    return (std::find(invitedClients.begin(), invitedClients.end(), client) != invitedClients.end());
}

void Channel::addOperator(Client *client)
{
    if (std::find(operators.begin(), operators.end(), client) == operators.end())
        operators.push_back(client);
}

void Channel::removeOperator(Client *client)
{
    std::vector<Client *>::iterator it = std::find(operators.begin(), operators.end(), client);
    if (it != operators.end())
        operators.erase(it);
}

bool Channel::isOperator(Client *client) const
{
    return (std::find(operators.begin(), operators.end(), client) != operators.end());
}

void Channel::broadcastMessage(const std::string &message)
{
    std::map<int, Client *>::iterator it = clientsMap.begin();
    while (it != clientsMap.end())
    {
        it->second->writeBuffer += message;
        ++it;
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

void Channel::setPassword(const std::string &pwd)
{
    password = pwd;
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
    return (clientLimit > 0 && clientsMap.size() >= static_cast<size_t>(clientLimit));
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

    std::map<int, Client *>::const_iterator it = clientsMap.begin();

    while (it != clientsMap.end())
    {
        clients.push_back(it->second);
        ++it;
    }

    return clients;
}

std::vector<Client *> Channel::getOperators() const { return (operators); }

void Channel::kickClient(Client *operatorClient, Client *targetClient, const std::string &reason)
{
    if (!isOperator(operatorClient))
    {
        operatorClient->writeBuffer += ":server 482 " + name + " :You're not a channel operator\r\n";
        return;
    }

    if (!isClientInChannel(targetClient))
    {
        operatorClient->writeBuffer += ":server 441 " + targetClient->nickname + " " + name + " :They aren't on that channel\r\n";
        return;
    }

    removeClient(targetClient);
    std::string kickMsg = ":" + operatorClient->nickname + " KICK " + name + " " + targetClient->nickname + " :" + reason + "\r\n";
    broadcastMessage(kickMsg);
}

void Channel::inviteClient(Client *operatorClient, Client *targetClient)
{
    if (!isOperator(operatorClient))
    {
        operatorClient->writeBuffer += ":server 482 " + name + " :You're not a channel operator\r\n";
        return;
    }

    if (isClientInChannel(targetClient))
    {
        operatorClient->writeBuffer += ":server 443 " + targetClient->nickname + " " + name + " :is already on channel\r\n";
        return;
    }

    invitedClients.push_back(targetClient);
    std::string inviteMsg = ":" + operatorClient->nickname + " INVITE " + targetClient->nickname + " " + name + "\r\n";
    targetClient->writeBuffer += inviteMsg;
}

void Channel::setTopic(Client *client, const std::string &newTopic)
{
    if (isTopicRestricted && !isOperator(client))
    {
        client->writeBuffer += ":server 482 " + name + " :You're not a channel operator\r\n";
        return;
    }

    topic = newTopic;
    std::string topicMsg = ":" + client->nickname + " TOPIC " + name + " :" + topic + "\r\n";
    broadcastMessage(topicMsg);
}

bool Channel::getTopic(Client *client)
{
    if (!isClientInChannel(client))
    {
        client->writeBuffer += ":server 442 " + name + " :You're not on that channel\r\n";
        return (false);
    }

    std::string topicMsg = ":" + client->nickname + " 332 " + name + " :" + topic + "\r\n";
    client->writeBuffer += topicMsg;
    return (true);
}

void Channel::setMode(Client *client, const std::string &mode, const std::string &argument)
{
    if (!isOperator(client))
    {
        client->writeBuffer += ":server 482 " + name + " :You're not a channel operator\r\n";
        return;
    }

    if (mode == "+i")
        isInviteOnly = true;
    else if (mode == "-i")
        isInviteOnly = false;
    else if (mode == "+k")
        setPassword(argument);
    else if (mode == "-k")
        setPassword("");
    else if (mode == "+o" || mode == "-o")
    {
        Client *target = findClientByNickname(argument);
        if (!target)
        {
            client->writeBuffer += ":server 401 " + argument + " :No such nick\r\n";
            return;
        }
        if (mode == "+o")
        {
            addOperator(target);
        }
        else
        {
            removeOperator(target);
        }
    }
    else if (mode == "+l")
    {
        int limit;
        std::istringstream iss(argument);
        if (iss >> limit && limit > 0)
            setClientLimit(limit);
    }
    else if (mode == "-l")
        setClientLimit(0);

    std::string modeMsg = ":" + client->nickname + " MODE " + name + " " + mode + " " + argument + "\r\n";
    broadcastMessage(modeMsg);
}

Client *Channel::findClientByNickname(const std::string &nickname)
{
    std::map<int, Client *>::iterator it = clientsMap.begin();

    while (it != clientsMap.end())
    {
        if (it->second->nickname == nickname)
            return it->second;
        ++it;
    }

    return NULL;
}
