#include "../include/ft_commands.hpp"

void    CommandHandler::execute_TIME(Client& client) {
    // Get current server time
    std::time_t now = std::time(nullptr);
    std::string timeString = std::ctime(&now); // Convert time to string format

    // Trim the newline character that ctime adds at the end of the time string
    timeString.erase(timeString.length() - 1);

    std::string message = ":ircserv 391 " + client.getNickname() + " :Local time on the server is " + timeString;

    sendMessageToClient(client, message + '\n');
    command_args.clear();
}