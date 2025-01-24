#include "../include/ft_irc.hpp"
#include "../include/Client.hpp"
#include "../include/ft_commands.hpp"

ft_irc::ft_irc(int port, std::string pass) : port(port) {
    this->password = pass;
    this->opt      = 1;
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(this->port);
    addrlen = sizeof(server_address);
}

ft_irc::~ft_irc() {
    std::cout << "Destroyed" << std::endl;
}

const std::string& ft_irc::getPassword() const {
    return this->password;
}

void    ft_irc::reset_buffer() {
   memset(buffer, 0, BUFFER_SIZE);
}

const std::map<int, Client>& ft_irc::Getclient_list() const {
    return this->clients_list;
}

const Client& ft_irc::getRecipient(const std::string& nickname) const {
    for (std::map<int, Client>::const_iterator it = clients_list.begin(); it != clients_list.end(); ++it) {
        if (it->second.getNickname() == nickname) {
            return it->second;  // Return a reference to the Client object
        }
    }
    static Client defaultClient;  // Static default client
    return defaultClient;  // Return a reference to a default client if not found
}


bool ft_irc::isClientInServer(const std::string& nickname) const {
    std::map<int, Client>::const_iterator it; // Use const_iterator in a const method
    for (it = clients_list.begin(); it != clients_list.end(); ++it) {
        if (it->second.getNickname() == nickname) {
            return true;  // Client with the specified nickname exists
        }
    }
    return false;  // No client with the given nickname found
}


void    ft_irc::close_client_session(int fd) {
    for (std::map<int, Client>::const_iterator it = clients_list.begin(); it != clients_list.end(); ++it) {
        if (it->second.getClientFd() == fd) {
            clients_list.erase(it);
            break ;
        }
    }
}

void    ft_irc::start() {

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror ("socket failed");
        return ;
    }
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror ("Setsockopt failed");
        close (server_fd);
        exit(EXIT_FAILURE);
    }
    if (bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0) {
        perror ("Bind failed");
        close (server_fd);
        close (server_fd);
        exit(EXIT_FAILURE);
    }
    
    if (listen(server_fd, 3) < 0) {
        perror ("Listen failed");
        close (server_fd);
        exit (EXIT_FAILURE);
    }

    std::cout << "The server is listening on " << this->port << std::endl;
    while (1)
    {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&server_address, (socklen_t *)&addrlen)) < 0) {
             std::cerr << "Accept Failed" << std::endl;
            continue;
        }
        std::cout << "Log: New Client connected" << std::endl;
        clients_list[new_socket] = Client(new_socket);
        Channel channels;
        while (true) {
            reset_buffer();
            CommandHandler  command(this->getPassword());
            size_t valread = read(new_socket, buffer, BUFFER_SIZE - 1);
            if (valread > 0) {
                std::string     client_input(buffer);
                
                if (command.check_command(client_input)) {
                    command.handleCommand(clients_list[new_socket], *this, channels);
                if (!clients_list[new_socket].getAuth())
                    continue ;
                }
                else {
                    command.sendMessageToClient(clients_list[new_socket], "Command not exist\n");
                    continue ;
                }
            }
        }
        close (clients_list[new_socket].getClientFd());
    }
}