#include "../include/ft_irc.hpp"

void    CommandHandler::execute_KICK(Client& client, ft_irc& server, Channel& Mainchannel) {
    if (client.getAuth() == 0)
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
    else if (client.getUser() == "NULL")
        sendMessageToClient(client, "Error: You must set both a nickname and a username before joining a channel.\n");  
    else if (command_args.size() < 4)
        sendMessageToClient(client, "Error: Bad usage KICK <#channel> <nickname> :<reason>\n");
    else 
    {
        Channel *channel = Mainchannel.getChannel(command_args[1]);
        if (channel == nullptr) 
        {
            sendMessageToClient(client, "Error: The specified channel does not exist.\n");
            command_args.clear();
            return ;
        }
        else 
        {
            if (channel->isOperator(&client)) 
            {
                Client& user_to_kick = server.getUserToKick(command_args[2]);
                std::string   reason;
                for (size_t i = 3; i < command_args.size(); i++) 
                {
                    if (i > 3)
                        reason += " "; // Add a space before appending, except for the first element
                    reason += command_args[i];
                }
                if (channel->isUserInChannel(&user_to_kick)) 
                {
                    channel->kickUser(&client, &user_to_kick, reason);
                    sendMessageToClient(client, "User Has been Kicked from channel\n");
                    std::cout << "Log: "<< client.getUser() << " KICKED " << user_to_kick.getNickname() << " from ";
                    std::cout << command_args[1] << " in reason: " << reason << std::endl;
                }
                else
                {
                    sendMessageToClient(client, "Error: User does not found in channel\n");
                }
            }
            else
            {
                    sendMessageToClient(client, "Error: You Must be the Channel Operator to Kick Users\n");
            }
        }
    }
    command_args.clear();
}
