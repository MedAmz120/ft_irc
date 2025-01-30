#include "../include/Client.hpp"

Client::Client() {
    client_fd = -1;
    nick      = "NULL";
    user      = "NULL";
    IPaddress = "NULL";
    real_name = "NULL";
    servername= "NULL";
    hostname  = "NULL";
    auth_not  = 0;
    session_duration = 0;
    joinedchannel_not = false;

    socket        = -1; // youssef
    isOperator    = false; // youssef
    nickname        = "NULL"; // youssef
}

Client::Client(int fd) {
    client_fd = fd;
    nick      = "NULL";
    user      = "NULL";
    IPaddress = "NULL";
    real_name = "NULL";
    servername= "NULL";
    hostname  = "NULL";
    auth_not  = 0;
    session_duration = 0;
    joinedchannel_not = false;

    socket        = fd; // youssef
    isOperator    = false; // youssef
    nickname        = "NULL"; // youssef
}

Client::Client(const Client& copie) {
    if (this != &copie)
        return ;
}

Client::~Client() {}

int Client::getClientFd() const {
    return this->client_fd;
}

void    Client::SetAuth () {
    session_duration = true;
    auth_not = 1;
}

bool Client::getAuth() const {
    return this->auth_not;
}

void    Client::SetUserName (const std::string& username_it) {
    this->user = username_it;
}

void    Client::SetNickName (const std::string& nickname_it) {
    this->nick = nickname_it;
    this->nickname = nickname_it;
}

void    Client::SetRealName (const std::string& realname_it) {
    this->real_name = realname_it;
}

void    Client::SetHostName(const std::string& hostname_it) {
    this->hostname = hostname_it;
}

void    Client::SetServerName(const std::string& servername_it) {
    this->servername = servername_it;
}

const std::string& Client::getUser() const {
    return this->user;
}

const   std::string& Client::getNickname() const {
    return this->nick;
}

void    Client::addInvitations(const std::string& channel_name) {
    invitations.push_back(channel_name);
}

bool    Client::checkInvite(const std::string& channelName) {
    for (size_t i = 0; i < invitations.size(); i++)
    {
        if (invitations[i] == channelName)
            return true;
    }
    return false;
}

// -------------- YOUSSEF ---------------

Client::Client(const std::string &nick_t, const std::string &user_t, const std::string &host_t, int sock) {
    client_fd = sock;
    nick      = nick_t;
    user      = user_t;
    IPaddress = "NULL";
    real_name = "NULL";
    servername= "NULL";
    hostname  = host_t;
    auth_not  = 0;
    session_duration = 0;
    joinedchannel_not = false;

    socket        = sock; // youssef
    isOperator    = false; // youssef
    nickname        = "NULL"; // youssef
}

void Client::joinChannel(const std::string &channelName)
{
    channels.push_back(channelName);
}

void Client::leaveChannel(const std::string &channelName)
{
    std::vector<std::string>::iterator it = std::find(channels.begin(), channels.end(), channelName);
    if (it != channels.end())
    {
        channels.erase(it);
    }
}

bool Client::isInChannel(const std::string &channelName) const
{
    return std::find(channels.begin(), channels.end(), channelName) != channels.end();
}

void Client::SetFd(int fd) {
    this->client_fd = fd;
}

void Client::setIpAdd(std::string ipaddress) {
    this->IPaddress = ipaddress;
}