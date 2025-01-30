#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include "Client.hpp"

class Client;

class Channel {
private:
    std::string name;
    std::vector<Client*> users;
    std::vector<Client*> operators;
    std::string topic;
    std::string password;
    int userLimit;
    bool isInviteOnly;
    bool istopicrestricted;
    Client* findUserByNickname(const std::string& nickname);
    static std::map<std::string, Channel*> channels;
    int clientLimit;
    
public:
    Channel();
    Channel(const std::string& name);
    ~Channel();
    static Channel* createChannel(const std::string& name, Client* creator);
    static Channel* getChannel(const std::string& name);
    static bool deleteChannel(const std::string& name);
    bool addUser(Client* user);
    void removeUser(Client* user);
    bool isUserInChannel(Client* user) const;
    void addOperator(Client* user);
    void removeOperator(Client* user);
    bool isOperator(Client* user) const;
    void broadcastMessage(const std::string& message);
    void setInviteOnly(bool inviteOnly);
    void setPassword(const std::string& password);
    void setUserLimit(int limit);
    std::string getName() const;
    std::string getTopic() const;
    std::vector<Client*> getUsers() const;
    std::vector<Client*> getOperators() const;
    void kickUser(Client* operatorUser, Client* targetUser, const std::string& reason);
    void inviteUser(Client* operatorUser, Client* targetUser);
    void setTopic(Client* user, const std::string& newTopic);
    void getTopic(Client* user);
    void setMode(Client* user, const std::string& mode, const std::string& argument = "");
    bool getInviteOnly() const;
    bool isClientLimitReached() const;
    void setClientLimit(int limit);
    void  setTopicChangeAbleOrNot(bool yes_no);

    bool    getTopicRestricted();
    // --------------- Mohamed ------------
    const std::string& getPassword();
    void sendMessageToClient(const Client& client, const std::string& message);
};
