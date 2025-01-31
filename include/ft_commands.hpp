#ifndef FT_COMMANDS_HPP
#define FT_COMMANDS_HPP

#include "Client.hpp"
#include "server.hpp"
#include "Channel.hpp"
#include <string>
#include <vector>
#include <set>
#include <sstream>

class Client;
class Server;
class Channel;

class CommandHandler {
    private:
    std::vector<std::string>    command_args;
    std::set<std::string>       valid_commands;
    std::string                 password;
    

    public:
    CommandHandler();
    CommandHandler(const std::string& password);
    ~CommandHandler();
    void handleCommand(Client& client, Server& server, Channel& channel);
    int check_command(const std::string& client_input);

    void    execute_PASS(Client& client);
    void    execute_USER(Client& client);
    void    execute_NICK(Client& client, const Server& server);
    void    execute_QUIT(Client& client, Server& server);
    void    execute_ADMIN(Client& client);
    void    execute_WHO(Client& client, const Server& server);
    void    execute_JOIN(Client& client, Channel& Mainchannel);
    void    execute_PART(Client& client, Channel& Mainchannel);
    void    execute_PRIVMSG(Client& client, const Server& server, Channel& Mainchannel);
    void    execute_KICK(Client& client, Server& server, Channel& Mainchannel);
    void    execute_INVITE(Client& client, Server& server, Channel& Mainchannel);
    void    execute_MODE(Client& client, Channel& Mainchannel);
    void    execute_TOPIC(Client& client, Channel& Mainchannel);
    void    execute_TIME(Client& client);
    void    execute_NOTICE(Client& sender, Server& server);

    void    sendMessageToClient(const Client& client, const std::string& mesage);
    bool    check_nickname(const std::string& nick_name, const std::map<int, Client>&  clients_list);
    void    display_client_WHO(Client& client, const std::string& nick_name, const std::map<int, Client>& clients_list);
    bool    check_channel_name(const std::string& name, Client& client);
    bool    check_mode(const std::string& mode, Client& client, const std::string& command_args);
    void    handleLSCAP(const std::string& client_input, Client& client, Server& server, Channel& channel);
    bool    check_username(const std::string& str);
    bool    isValidCharacter(unsigned char c);
};



#endif