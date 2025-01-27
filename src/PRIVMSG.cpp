#include "../include/ft_commands.hpp"

void    CommandHandler::execute_PRIVMSG(Client& client, const Server& server, Channel& Mainchannel) {
    if (client.getAuth() == 0) {
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
        command_args.clear();
        return ;
    }
    else if (client.getUser() == "NULL")
    {
        sendMessageToClient(client, "Error: You must set both a nickname and a username before joining a channel.\n");
        command_args.clear();
        return ;
    }
    else if (command_args.size() < 3) {
        sendMessageToClient(client, "Error: Bad usage PRIVMSG <recipient> <message>\n");
        command_args.clear();
        return ;
    }
    else {
        std::string full_message;
        for (size_t i = 2; i < command_args.size(); i++) {
            if (i > 2)
                full_message += " "; // Add a space before appending, except for the first element
            full_message += command_args[i];
        }
        full_message += '\n';
        if (command_args[1][0] == '#') {
            Channel *channel = Mainchannel.getChannel(command_args[1]);
            if (channel == nullptr) {
                sendMessageToClient(client, "Error: The specified channel does not exist.\n");
                command_args.clear();
                return ;
            }
            else {
                if (channel->isUserInChannel(&client)) {
                channel->broadcastMessage(full_message);
                std::cout << "Log: "<< client.getUser() << " send \"" << full_message << "\" To " << command_args[1] << std::endl;           
                command_args.clear();
                return ;
                }
                else {
                    sendMessageToClient(client, "Error: You must be a member of the channel to send messages to it\n");
                    command_args.clear();
                }
            }
        }
        else {
            if (server.isClientInServer(command_args[1])) {
                const Client& recipient = server.getRecipient(command_args[1]);
                sendMessageToClient(recipient, full_message);
                std::cout << "Log: "<< client.getUser() << " send " << full_message << " To " << recipient.getNickname() << std::endl;           
                command_args.clear();
            }
            else
                sendMessageToClient(client, "Error: The specified recipient does not exist.\n");  
        }
    }
    command_args.clear();
}
