#include "../include/ft_commands.hpp"

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
    valid_commands.insert("TIME");
    valid_commands.insert("NOTICE");
    password = password_it;
}

void    CommandHandler::sendMessageToClient(const Client& client, const std::string& message)
{
        send (client.getClientFd(), message.c_str(), message.size(), 0);
}

int    CommandHandler::check_command(const std::string& client_input) {

    // Check for the presence of the substring "CAP LS"
    if (client_input.find("CAP LS") != std::string::npos) {
        return -1;
    }
    
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


void    CommandHandler::handleLSCAP(const std::string& client_input, Client& client, Server& server) {
    // Split the input by new lines
    std::istringstream iss(client_input);
    std::string line;
    std::vector<std::string> lines;

    // Read all lines into a vector
    while (std::getline(iss, line)) {
        if (!line.empty()) {
            lines.push_back(line);
        }
    }

    // Assuming the first line is always "CAP LS" and should be ignored
    for (size_t i = 1; i < lines.size(); i++) {
        // Clear previous commands
        command_args.clear();

        // Split the line into words and push into command_args
        std::istringstream cmdStream(lines[i]);
        std::string word;
        while (cmdStream >> word) {
            command_args.push_back(word);
        }

        // Process commands based on the first word
        if (!command_args.empty()) {
            if (command_args[0] == "PASS")
                execute_PASS(client);
            else if (command_args[0] == "USER")
                execute_USER(client);
            else if (command_args[0] == "NICK")
                execute_NICK(client, server);
            else {
                sendMessageToClient(client, "Command Not found\n");
                return;
            }
        }
    }
    // Clear command arguments after processing
    command_args.clear();
};

void    CommandHandler::handleCommand(Client& client, Server& server, Channel& channel) {
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
    else if (command_args[0] == "INVITE") // DONE 
        execute_INVITE(client, server, channel);
    else if (command_args[0] == "TOPIC") // DONE 
        execute_TOPIC(client, channel);
    else if (command_args[0] == "TIME") // DONE
        execute_TIME(client);
    else if (command_args[0] == "NOTICE") // DONE
        execute_NOTICE(client, server);
}