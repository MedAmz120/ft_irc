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
    valid_commands.insert("KICK");
    valid_commands.insert("INVITE");
    valid_commands.insert("MODE");
    valid_commands.insert("TOPIC");
    valid_commands.insert("ADMIN");
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
    else if (command_args[0] == "QUIT")// DONE
        execute_QUIT(client, server);
    else if (command_args[0] == "ADMIN")// DONE
        execute_ADMIN(client);
    else if (command_args[0] == "WHO") // Done
        execute_WHO(client, server);
    else if (command_args[0] == "JOIN") // Done
        execute_JOIN(client, channel);
    else if (command_args[0] == "PART") // DONE
        execute_PART(client, channel);
    else if (command_args[0] == "PRIVMSG") // DONE
        execute_PRIVMSG(client, server, channel);
    else if (command_args[0] == "KICK") // DONE
        execute_KICK(client, server, channel);
    else if (command_args[0] == "MODE") // DONE
        execute_MODE(client, channel);
    else if (command_args[0] == "INVITE") // DONE but add a log message on channel::inviteUser() method
        execute_INVITE(client, server, channel);
    else if (command_args[0] == "TOPIC") // DONE but Channel::setTopic() doesn't check properly if the channel is topic restricted and must add log message also
        execute_TOPIC(client, channel);
}