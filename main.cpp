#include "include/Client.hpp"
#include "include/server.hpp"

int main(int ac,char **argv)
{
	if(ac != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return (1);
	}
	Server serverInstance;
	std::cout << "---- SERVER APPLICATION ----" << std::endl;
	try{
		signal(SIGINT, Server::signalHandler); //-> catch the signal (ctrl + c)
		signal(SIGQUIT, Server::signalHandler); //-> catch the signal (ctrl + \)
		int port = atoi(argv[1]);
		serverInstance.initializeServer(port, argv[2]); //-> initialize the server
	}
	catch(const std::exception& e){
		serverInstance.closeAllConnections(); //-> close the file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "Server shutdown completed." << std::endl;
}