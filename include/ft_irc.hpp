#ifndef FT_IRC_HPP
#define FT_IRC_HPP

#include <string>
#include <cstring>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <map>
#include "ft_commands.hpp"
#include "Client.hpp"
#include "Channel.hpp"

#define BUFFER_SIZE 512

class Client;

class ft_irc {
    private:
    int                 port;
    int                 opt;
    std::string         password;
    int                 server_fd;
    struct sockaddr_in  server_address;
    int                 addrlen;
    int                 new_socket;
    char                buffer[BUFFER_SIZE];
    std::map<int, Client>  clients_list;

    public:
    ft_irc(int port, std::string password); 
    ~ft_irc();
    void                         start();
    const std::string&           getPassword() const ;
    void                         reset_buffer();
    const std::map<int, Client>& Getclient_list() const;
    void                         close_client_session(int fd);
};

#endif