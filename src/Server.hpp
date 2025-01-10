

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
	Server(const Server &other);
	Server &operator=(const Server &other);
	~Server();

	int getPort() const;
	std::string getPassword() const;
	void setPort(int port);
	void setPassword(std::string passwd);

	void runServer();

	void addClient(Client *client);
	void removeClient(int fd);

private:
	int _port = 6667; // Example port
	std::string _passwd;
	std::vector<Client *> _clients;

	void HandleCAPLS(Client &client, const std::string &message);
	void MessageServerToClient(Client client, const std::string &message);
	std::vector<std::string> SplitString(const std::string &str, char delimiter);
	void MessageClientToServer(Client &client, const std::string &message);
	void handleEvents(std::vector<struct pollfd> &fds, std::vector<Client *> &_clients);
	int createServerSocket();
	void bindAndListen(int server_fd);
	void handleNewClient(int server_fd, std::vector<Client *> &_clients);
	void cleanupResources(int server_fd, std::vector<Client *> &_clients);
	void sendTestMessage(int client_fd);
	bool startswith(const std::string &str, const std::string &cmp);
	void handleCapReq(Client &client, const std::string &message);
	void handleUserName(Client &client, const std::string &message);
};