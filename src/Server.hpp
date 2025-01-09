

#pragma once 


#include <string>
#include <netinet/in.h>
#include <vector> 
#include "Client.hpp"

const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;



class Server
{
	public:
	
		Server();
		Server(int port, std::string passwd);
		Server(const Server& other);
    	Server& operator=(const Server& other);
		~Server();

		int getPort() const;
		std::string getPassword() const;
		void setPort(int port);
		void setPassword(std::string passwd);

		void runServer();

		void addClient(Client* client);
    	void removeClient(int fd);



	private:
	
		int _port;
		std::string _passwd;
		std::vector <Client*> _clients;

		

};