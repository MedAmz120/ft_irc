/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ychihab <ychihab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/15 22:57:54 by ychihab           #+#    #+#             */
/*   Updated: 2025/01/27 00:59:46 by ychihab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <vector>

class Client {
public:
    std::string nickname;
    std::string username;
    std::string hostname;
    int socket;
    std::vector<std::string> channels;
    std::string writeBuffer;

    Client();
    Client(const std::string &nick, const std::string &user, const std::string &host, int sock);
    void joinChannel(const std::string &channelName);
    void leaveChannel(const std::string &channelName);
    bool isInChannel(const std::string &channelName) const;
};

#endif