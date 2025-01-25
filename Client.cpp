/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychihab <ychihab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 17:20:15 by ychihab           #+#    #+#             */
/*   Updated: 2025/01/25 22:32:24 by ychihab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client() : socket(-1), isOperator(false) {}

Client::Client(const std::string &nick, const std::string &user, const std::string &host, int sock)
    : nickname(nick), username(user), hostname(host), socket(sock), isOperator(false) {}

void Client::joinChannel(const std::string &channelName)
{
    channels.push_back(channelName);
}

void Client::leaveChannel(const std::string &channelName)
{
    std::vector<std::string>::iterator it = std::find(channels.begin(), channels.end(), channelName);
    if (it != channels.end())
    {
        channels.erase(it);
    }
}

bool Client::isInChannel(const std::string &channelName) const
{
    return std::find(channels.begin(), channels.end(), channelName) != channels.end();
}