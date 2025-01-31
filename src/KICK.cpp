#include "../include/ft_commands.hpp"

void    CommandHandler::execute_KICK(Client& client, Server& server, Channel& Mainchannel) {
    if (client.getAuth() == 0)
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
    else if (client.getUser() == "NULL")
        sendMessageToClient(client, "Error: You must set both a nickname and a username before joining a channel.\n");  
    else if (command_args.size() < 4)
        sendMessageToClient(client, "Error: Bad usage KICK <#channel> <nickname> :<reason>\n");
    else 
    {
        int channel_argpos = 1;
        int usertokick_argpos = 2;
        int reason_argpos = 3;

        if (command_args[1][0] != '#') {
            channel_argpos = 2;
            usertokick_argpos = 3;
            reason_argpos = 4;
        }

        Channel *channel = Mainchannel.getChannel(command_args[channel_argpos]);
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
                if (!command_args[usertokick_argpos].empty() && command_args[usertokick_argpos][0] == ':') {
                    std::string newStr = command_args[usertokick_argpos].substr(1);  // Removes the first character
                    command_args[usertokick_argpos] = newStr;
                }
                Client& user_to_kick = server.getUserToKick(command_args[usertokick_argpos]);
                std::string   reason;
                for (size_t i = reason_argpos; i < command_args.size(); i++) 
                {
                    if (i > reason_argpos)
                        reason += " "; // Add a space before appending, except for the first element
                    reason += command_args[i];
                }
                if (reason.empty())
                    reason = "no reason";
                if (channel->isUserInChannel(&user_to_kick)) 
                {
                    channel->kickUser(&client, &user_to_kick, reason);
                    sendMessageToClient(client, "User Has been Kicked from channel\n");
                    std::cout << "Log: "<< client.getNickname() << " KICKED " << user_to_kick.getNickname() << " from ";
                    std::cout << command_args[channel_argpos] << " in reason: " << reason << std::endl;
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
