#pragma once

#include "ft_irc.hpp"

class Client {
    private:
    std::string     nick;
    std::string     user;
    std::string     real_name; // for identification optional
    std::string     hostname;
    std::string     servername;
    std::string     ip;
    int             client_fd; // The client's file descriptor
    int             auth_not; // Whether the client has successfully authenticated
    long            session_duration; // The timestamp of the client's connection (used for timeout or activity tracking).
    bool            joinedchannel_not; // Whether the client has joined a channel


    public:
    Client(int fd);
    Client(const std::string &nick, const std::string &user, const std::string &host, int sock);
    Client();
    
    int     getClientFd() const;
    bool    getAuth() const ;
    const   std::string& getNickname() const;
    const   std::string& getUser() const;


    void    SetAuth ();
    void    SetUserName (const std::string& username);
    void    SetNickName (const std::string& nickname_it);
    void    SetRealName (const std::string& realname_it);
    void    SetHostName(const std::string& hostname_it);
    void    SetServerName(const std::string& servername_it);

    // -------- Youssef --------
    std::string nickname;
    int socket;
    bool isOperator;
    std::vector<std::string> channels;
    void joinChannel(const std::string &channelName);
    void leaveChannel(const std::string &channelName);
    bool isInChannel(const std::string &channelName) const;

    ~Client();
    Client (const Client& copie);
};