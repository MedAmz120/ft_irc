#include "../include/ft_commands.hpp"

bool    CommandHandler::check_nickname(const std::string& nick_name, const std::map<int, Client>&  clients_list) {
    for (std::map<int, Client>::const_iterator it = clients_list.begin(); it != clients_list.end(); ++it) {
        if (it->second.getNickname() == nick_name) {
            return false;  // Nickname exists
        }
    }
    return true;  // Nickname does not exist
}

void    CommandHandler::execute_NICK(Client& client, const Server& server)
{
    if (client.getAuth() == 0) {
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
    }
    else if (command_args.size() != 2) {
        sendMessageToClient(client, "Error: Bad usage NICK <NickName>\n");
    }
    else {
        if (!check_nickname(command_args[1], server.Getclient_list())) {
            sendMessageToClient(client, "Nickname already taken, please choose a different one\n");
        }
        else {
            client.SetNickName(command_args[1]);
            sendMessageToClient(client, "Nickname set successfully !\n");
            std::cout << "Log: " << "User " << client.getClientFd() << " Set Nickname to "  << client.getNickname() << std::endl;
        }

    }
    command_args.clear();
}