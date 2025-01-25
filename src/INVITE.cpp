#include "../include/ft_irc.hpp"


void CommandHandler::execute_INVITE(Client& client, ft_irc& server, Channel& Mainchannel) {
    if (client.getAuth() == 0) {
        sendMessageToClient(client, "You must authenticate with PASS first.\n");
        command_args.clear();
        return;
    }
    else if (client.getUser() == "NULL")
    {
        sendMessageToClient(client, "Error: You must set both a nickname and a username before joining a channel.\n");
        command_args.clear();
        return ;
    }
    if (command_args.size() != 3) {  // INVITE <nickname> <#channel>
        sendMessageToClient(client, "Error: Bad usage INVITE <nickname> <#channel>\n");
        command_args.clear();
        return;
    }

    std::string nickname = command_args[1];
    std::string channelName = command_args[2];
    Channel* channel = Mainchannel.getChannel(channelName);
    Client& recipient = server.getRecipientNonConst(nickname);
    if (!channel) {
        sendMessageToClient(client, "Error: The specified channel does not exist.\n");
        command_args.clear();
        return;
    }
    if (!recipient.getAuth()) {
        sendMessageToClient(client, "Error: The specified user does not exist or is not authenticated.\n");
        command_args.clear();
        return;
    }
    channel->inviteUser(&client, &recipient);
    command_args.clear();
}