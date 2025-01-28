/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychihab <ychihab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 18:57:22 by ychihab           #+#    #+#             */
/*   Updated: 2025/01/28 12:23:05 by ychihab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include <string>
#include <vector>
#include <map>
#include "Client.hpp"

class Channel
{
private:
    std::string name;
    std::map<int, Client *> clientsMap;
    std::vector<Client *> operators;
    std::vector<Client *> invitedClients;
    std::string topic;
    std::string password;
    int clientLimit;
    bool isInviteOnly;
    bool isTopicRestricted;
    Client *findClientByNickname(const std::string &nickname);
    static std::map<std::string, Channel *> channels;

public:
    Channel();
    explicit Channel(const std::string &name);
    ~Channel();
    static std::map<std::string, Channel *> &getChannels();
    static Channel *createChannel(const std::string &name, Client *creator);
    static Channel *getChannel(const std::string &name);
    static bool deleteChannel(const std::string &name);
    bool addClient(Client *client, const std::string &clientProvidedPassword = "");
    void removeClient(Client *client);
    bool isClientInChannel(Client *client) const;
    bool isClientInvited(Client *client) const;
    void addOperator(Client *client);
    void removeOperator(Client *client);
    bool isOperator(Client *client) const;
    void broadcastMessage(const std::string &message);
    void setInviteOnly(bool inviteOnly);
    void setPassword(const std::string &password);
    void setClientLimit(int limit);
    void setTopic(Client *client, const std::string &newTopic);
    bool getInviteOnly() const;
    std::string getPassword() const;
    bool isClientLimitReached() const;
    std::string getName() const;
    std::string getTopic() const;
    bool getTopic(Client *client);
    std::vector<Client *> getClients() const;
    std::vector<Client *> getOperators() const;
    void kickClient(Client *operatorClient, Client *targetClient, const std::string &reason);
    void inviteClient(Client *operatorClient, Client *targetClient);
    void setMode(Client *client, const std::string &mode, const std::string &argument = "");
    const std::vector<Client*>& getInvitedClients() const;
    
};

#endif