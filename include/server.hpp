#ifndef SERVER_HPP
#define SERVER_HPP



#include "Channel.hpp"
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h> //-> for inet_ntoa()
#include <poll.h>
#include <csignal>

// ------- Mohamed -------
#include <string>
#include <cstring>
#include <map>
#include "ft_commands.hpp"
#include "Client.hpp"
#define BUFFER_SIZE 1024
// ------- Mohamed -------

#define COLOR_RED "\e[1;31m"
#define COLOR_WHITE "\e[0;37m"
#define COLOR_GREEN "\e[1;32m"
#define COLOR_YELLOW "\e[1;33m"


class Client;

class Server
{
private:
	int serverPort;
	std::string password;
	int ServerSocket; //-> server socket file descriptor
	static bool signalFlag;
	// std::vector<Client> clientList;
	std::vector<struct pollfd> pollDescriptors;
	// ------- Mohamed -------
	char                buffer[BUFFER_SIZE];
	std::map<int, Client>  clients_list;
	int                 new_socket;
	Channel 			channels;
	// ------- Mohamed -------
public:
	Server();

	void initializeServer(int serverPort, std::string password);
	void CreateSocket();
	void AcceptClient();
	void handleIncomingData(int fd, Channel& channel);

	static void signalHandler(int signalNumbre);
	
	void closeAllConnections();
	void removeClient(int fd);
	// ------- Mohamed -------
	
	const std::string&           getPassword() const ;
	void                         reset_buffer();
    const std::map<int, Client>& Getclient_list() const;
    void                         close_client_session(int fd);
	bool                         isClientInServer(const std::string& nickname) const;
	const Client&                getRecipient(const std::string &name) const;
    Client&                      getRecipientNonConst(const std::string &name) ;
    Client&                      getUserToKick(const std::string& nickname);
	// ------- Mohamed -------
};

#endif