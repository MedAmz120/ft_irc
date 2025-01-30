/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: moamzil <moamzil@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 20:30:55 by ychihab           #+#    #+#             */
/*   Updated: 2025/01/30 20:49:26 by moamzil          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Channel.hpp"
#include <algorithm>
#include <sys/socket.h>

std::map<std::string, Channel *> Channel::channels;

Channel::Channel() : userLimit(0), isInviteOnly(false), istopicrestricted(false) {}

Channel::Channel(const std::string &name) : name(name), userLimit(0), isInviteOnly(false), istopicrestricted(false) {}

Channel *Channel::createChannel(const std::string &name, Client *creator)
{
    if (channels.find(name) != channels.end())
    {
        return NULL;
    }
    Channel *newChannel = new Channel(name);
    newChannel->addUser(creator);
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
        return true;
    }
    return false;
}

bool Channel::addUser(Client *user)
{
    if (std::find(users.begin(), users.end(), user) == users.end())
    {
        users.push_back(user);
        user->joinChannel(name);
        return true;
    }
    return false;
}

void Channel::removeUser(Client *user)
{
    std::vector<Client *>::iterator it = std::find(users.begin(), users.end(), user);
    if (it != users.end())
    {
        users.erase(it);
        user->leaveChannel(name);
    }
}

bool Channel::isUserInChannel(Client *user) const
{
    return std::find(users.begin(), users.end(), user) != users.end();
}

void Channel::addOperator(Client *user)
{
    if (std::find(operators.begin(), operators.end(), user) == operators.end())
    {
        operators.push_back(user);
    }
}

void Channel::removeOperator(Client *user)
{
    std::vector<Client *>::iterator it = std::find(operators.begin(), operators.end(), user);
    if (it != operators.end())
    {
        operators.erase(it);
    }
}

bool Channel::isOperator(Client *user) const
{
    return std::find(operators.begin(), operators.end(), user) != operators.end();
}

void    Channel::sendMessageToClient(const Client& client, const std::string& message)
{
        send (client.getClientFd(), message.c_str(), message.size(), 0);
}

void Channel::broadcastMessage(const std::string &message)
{
    for (std::vector<Client *>::iterator it = users.begin(); it != users.end(); ++it)
    {
        // send((*it)->socket, message.c_str(), message.size(), 0);
        sendMessageToClient(**it, message);
    }
}

void Channel::setInviteOnly(bool inviteOnly)
{
    isInviteOnly = inviteOnly;
}

void Channel::setPassword(const std::string &password)
{
    this->password = password;
}

void Channel::setUserLimit(int limit)
{
    userLimit = limit;
}

std::string Channel::getName() const
{
    return name;
}

std::string Channel::getTopic() const
{
    return topic;
}

std::vector<Client *> Channel::getUsers() const
{
    return users;
}

std::vector<Client *> Channel::getOperators() const
{
    return operators;
}

void Channel::kickUser(Client *operatorUser, Client *targetUser, const std::string &reason)
{
    if (!isOperator(operatorUser))
    {
        return;
    }

    if (!isUserInChannel(targetUser))
    {
        return;
    }

    std::string kickMessage = ":" + operatorUser->nickname + " KICKED " + targetUser->nickname + "For : " + reason + "\r\n";
    broadcastMessage(kickMessage);
    removeUser(targetUser);
}

void Channel::inviteUser(Client *operatorUser, Client *targetUser)
{
    if (!isOperator(operatorUser))
    {
        std::string errorMessage = ":" + operatorUser->nickname + " 482 " + name + " :You're not a channel operator\r\n";
        send(operatorUser->socket, errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    if (isUserInChannel(targetUser))
    {
        std::string errorMessage = ":" + operatorUser->nickname + " 443 " + targetUser->nickname + " " + name + " :is already on channel\r\n";
        send(operatorUser->socket, errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    std::string inviteMessage = ":" + operatorUser->nickname + " INVITE " + targetUser->nickname + " " + name + "\r\n";
    send(targetUser->socket, inviteMessage.c_str(), inviteMessage.size(), 0);
}
bool    Channel::getTopicRestricted() {
    return istopicrestricted;
}


void Channel::setTopic(Client *user, const std::string &newTopic)
{
    if (istopicrestricted && !isOperator(user))
    {
        std::string errorMessage = ":" + user->nickname + " 482 " + name + " :You're not a channel operator\r\n";
        send(user->socket, errorMessage.c_str(), errorMessage.size(), 0);
        return;
    }

    topic = newTopic;
    std::string topicMessage = user->nickname + " changed " + name + " channel Topic to : " + topic + "\r\n";
    broadcastMessage(topicMessage);
}

void Channel::getTopic(Client *user)
{
    std::string topicMessage = ":" + user->nickname + " 332 " + name + " :" + topic + "\r\n";
    send(user->socket, topicMessage.c_str(), topicMessage.size(), 0);
}

// void Channel::setMode(Client *user, const std::string &mode, const std::string &argument)
// {
//     if (!isOperator(user))
//     {
//         std::string errorMessage = ":" + user->nickname + " 482 " + name + " :You're not a channel operator\r\n";
//         send(user->socket, errorMessage.c_str(), errorMessage.size(), 0);
//         return;
//     }

//     if (mode == "+i")
//     {
//         isInviteOnly = true;
//     }
//     else if (mode == "-i")
//     {
//         isInviteOnly = false;
//     }
//     else if (mode == "+k")
//     {
//         setPassword(argument);
//     }
//     else if (mode == "-k")
//     {
//         setPassword("");
//     }
//     else if (mode == "+o")
//     {
//         Client *targetUser = findUserByNickname(argument);
//         if (targetUser)
//         {
//             addOperator(targetUser);
//         }
//     }
//     else if (mode == "-o")
//     {
//         Client *targetUser = findUserByNickname(argument);
//         if (targetUser)
//         {
//             removeOperator(targetUser);
//         }
//     }
//     else if (mode == "+l")
//     {
//         if (!argument.empty())

//     }
//     std::string modeMessage = ":" + user->nickname + " MODE " + name + " " + mode + " " + argument + "\r\n";
//     broadcastMessage(modeMessage);
// }

Client *Channel::findUserByNickname(const std::string &nickname)
{
    std::vector<Client *>::iterator it = users.begin();
    while (it != users.end())
    {
        if ((*it)->nickname == nickname)
        {
            return *it;
        }
        ++it;
    }

    return NULL;
}

void Channel::setClientLimit(int limit)
{
    clientLimit = limit;
}

bool Channel::isClientLimitReached() const
{
    return (clientLimit > 0 && users.size() >= static_cast<size_t>(clientLimit));
}

void  Channel::setTopicChangeAbleOrNot(bool yes_no) {
    istopicrestricted = yes_no;

}


void Channel::setMode(Client *client, const std::string &mode, const std::string &argument)
{
    if (!isOperator(client))
    {
        std::string errorMessage = ":" + client->nickname + " 482 " + name + " :You're not a channel operator\r\n";
        send(client->socket, errorMessage.c_str(), errorMessage.size(), 0);
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
    else if (mode == "+t")
        setTopicChangeAbleOrNot(true);    
    else if (mode == "-t")
        setTopicChangeAbleOrNot(false);
    else if (mode == "+o" || mode == "-o")
    {
        Client *target = findUserByNickname(argument);
        if (!target)
        {
            std::string errorMessage = ":Error: No such user with this nickname\n";
            send(client->socket, errorMessage.c_str(), errorMessage.size(), 0);
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


bool Channel::getInviteOnly() const
{
    return (isInviteOnly);
}

Channel::~Channel()
{

}

// ---------- Mohamed -----------

const std::string& Channel::getPassword() {
    return this->password;
}