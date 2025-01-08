

#pragma once 


#include <string>
#include <netinet/in.h> 

const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

struct Client {
    int fd;
    sockaddr_in addr;
};


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



	private:
	
		int _port;
		std::string _passwd;
		

};