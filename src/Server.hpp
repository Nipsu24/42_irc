

#pragma once

#include <string>
#include <netinet/in.h>
#include <vector>
#include "Client.hpp"
#include "Channel.hpp"

const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

class Server
{
public:
	Server(int port, std::string passwd);
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
	std::vector<Channel *> _channels;

	Server();
	Server(const Server &other);
	Server &operator=(const Server &other);

	// runServer.cpp

	void handleEvents(std::vector<struct pollfd> &fds, std::vector<Client *> &_clients);
	int createServerSocket();
	void bindAndListen(int server_fd);
	void handleNewClient(int server_fd, std::vector<Client *> &_clients);
	void cleanupResources(int server_fd, std::vector<Client *> &_clients);
	void sendTestMessage(int client_fd);

	// messageHandler.cpp
	void handleClientMessage(Client &client, const std::string &message);
	std::vector<std::string> SplitString(const std::string &str);
	void MessageServerToClient(Client client, const std::string &message);
	void sendToChannelClients(Client *client, std::string message, std::string channelName);

	// handleCommands.cpp
	void handleCAPLS(Client &client, std::vector<std::string> tokens, int index);
	void handleCapReq(Client &client, std::vector<std::string> tokens, int index);
	void handleUserName(Client &client, std::vector<std::string> tokens, int index);

	void handlePass(Client &client, std::vector<std::string> tokens, int index);
	void handleJoin(Client &client, std::string channelName);
	void handlePart(Client &client, std::vector<std::string> tokens, int index);
	void handlePrivmsg(Client &client, std::string channelName, const std::string &message);
	// void handlePrivmsg(Client &client, std::vector<std::string> tokens, int index, const std::string &message);
	void handleNick(Client &client, std::string nick);
	void handlePingPong(Client &client);

	// channel handles these
	void handleKick(Client &client, std::vector<std::string> tokens, int index);
	void handleTopic(Client &client, std::vector<std::string> tokens, int index);
	void handleInvite(Client &client, std::vector<std::string> tokens, int index);
	// parseChannelModes.cpp
	void handleMode(Client &client, const std::string &channelName, const std::string &message);
	bool channelExists(const std::string &channelName);
	std::vector<Channel *> &getChannels();
	bool userIsMemberOfChannel(Client &client, const std::string &channelName);

	void handleQuit(Client &client, std::string message);
};
