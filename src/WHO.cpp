#include "../include/ft_commands.hpp"

void    CommandHandler::display_client_WHO(Client& client, const std::string& nick_name, const std::map<int, Client>& clients_list) {
   for (std::map<int, Client>::const_iterator it = clients_list.begin(); it != clients_list.end(); ++it) {
        if (it->second.getNickname() == nick_name) {
            std::stringstream ss; // Create a stringstream object
            ss << "User ID: " << it->second.getClientFd(); // Use the stringstream to concatenate
            std::string message1 = ss.str(); // Convert stringstream to string
            std::string message2 = " | Username: " + it->second.getUser();
            std::string message3 = " | Nickname: " + it->second.getNickname();
            std::string full = message1 + message2 + message3 + "\n";
            sendMessageToClient(client, full);
        }
    } // User ID: 12345 | Username: johndoe | Nickname: Johnny
}

void    CommandHandler::execute_WHO(Client& client, const Server& server) {
    if (client.getAuth() == 0) {
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
    }
    else if (command_args.size() != 2) {
        sendMessageToClient(client, "Error: Bad usage WHO <NickName>\n");
    }
    else {
        if (!check_nickname(command_args[1], server.Getclient_list()))
            display_client_WHO(client, command_args[1], server.Getclient_list());
        else {
            sendMessageToClient(client, "Nickname does not exist\n");
        }
    }
    command_args.clear();
}