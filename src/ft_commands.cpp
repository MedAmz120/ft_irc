#include "../include/ft_irc.hpp"

CommandHandler::CommandHandler()
{}

CommandHandler::~CommandHandler ()
{}

CommandHandler::CommandHandler(const std::string& password_it) {
    valid_commands.insert("PASS");
    valid_commands.insert("NICK");
    valid_commands.insert("USER");
    valid_commands.insert("JOIN");
    valid_commands.insert("PART");
    valid_commands.insert("PRIVMSG");
    valid_commands.insert("QUIT");
    valid_commands.insert("WHO");
    password = password_it;
}

void    CommandHandler::sendMessageToClient(const Client& client, const std::string& message)
{
        send (client.getClientFd(), message.c_str(), message.size(), 0);
}

bool    CommandHandler::check_command(const std::string& client_input) {
    if (client_input.empty())
        return false;
    std::istringstream  iss(client_input);
    std::string         command;

    command_args.clear();

    if (!(iss >> command))// Check if there's a first word in the input
        return false;


    if (valid_commands.find(command) == valid_commands.end())
        return false;

    command_args.push_back(command);
    while (iss >> command)
        if (!command.empty())
            command_args.push_back(command);
    return true;
}

void    CommandHandler::handleCommand(Client& client, ft_irc& server, Channel& channel) {
    if (command_args[0] == "PASS")
        execute_PASS(client);
    else if (command_args[0] == "USER") // Done
        execute_USER(client);
    else if (command_args[0] == "NICK") // Done
        execute_NICK(client, server);
    else if (command_args[0] == "QUIT")
        execute_QUIT(client, server); // still need work
    else if (command_args[0] == "ADMIN")
        execute_ADMIN(client);
    else if (command_args[0] == "WHO") // Done
        execute_WHO(client, server);
    else if (command_args[0] == "JOIN") // Done
        execute_JOIN(client, channel);
    else if (command_args[0] == "PART")
        execute_PART(client, channel);
    else if (command_args[0] == "PRIVMSG")
        execute_PRIVMSG(client, server, channel);
}