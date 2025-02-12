/* **************************************************************************************** */
/*                                                                                          */
/*                                                        ::::::::::: :::::::::   ::::::::  */
/*                                                           :+:     :+:    :+: :+:    :+:  */
/*                                                          +:+     +:+    +:+ +:+          */
/*                                                         +#+     +#++:++#:  +#+           */
/*  By: Timo Saari<tsaari@student.hive.fi>,               +#+     +#+    +#+ +#+            */
/*      Matti Rinkinen<mrinkine@student.hive.fi>,        #+#     #+#    #+# #+#    #+#      */
/*      Marius Meier<mmeier@student.hive.fi>        ########### ###    ###  ########        */
/*                                                                                          */
/* **************************************************************************************** */

#pragma once

#include <string>
#include <netinet/in.h>
#include <vector>
#include "Client.hpp"
#include "Channel.hpp"

#include <signal.h>
#include <unistd.h>
#include <cstdlib>

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

	int							getPort() const;
	std::string 				getPassword() const;
	void						setPort(int port);
	void						setPassword(std::string passwd);
	void						runServer();
	void						addClient(Client *client);
	void						removeClient(int fd);

	// runServer.cpp
	void						handleEvents(std::vector<struct pollfd> &fds);
	int							createServerSocket();
	void						bindAndListen(int server_fd);
	void						handleNewClient(int server_fd);
	void						cleanupResources(int server_fd);
	void						sendTestMessage(int client_fd);

	// messageHandler.cpp
	void						handleClientMessage(Client &client, const std::string &message);
	std::vector<std::string>	SplitString(const std::string &str);
	void						MessageServerToClient(Client client, const std::string &message);
	void						sendToChannelClients(Client *client, std::string message, std::string channelName);

	// handleCommands.cpp
	void						handleCAPs(Client &client, const std::vector<std::string>& tokens, int index);
	//void						handleCapReq(Client &client, std::vector<std::string> tokens, int index);
	void						handleUserName(Client &client, std::vector<std::string> tokens, int index);

	void						handlePass(Client &client, std::vector<std::string> tokens, int index);
	void						handleJoin(Client &client, std::string channelName, std::string password);
	void						handlePart(Client &client, std::vector<std::string> tokens, int index);
	void						handlePrivmsg(Client &client, std::string channelNameOrNick, std::string &message);
	
	// void handlePrivmsg(Client &client, std::vector<std::string> tokens, int index, const std::string &message);
	void						handleNick(Client &client, std::string nick);
	void						handlePingPong(Client &client, std::string rest);

	// channel handles these
	void						handleKick(Client &client, std::string message);
	void						handleTopic(Client &client, const std::string& channelName, std::string message);
	void						handleInvite(Client &client, std::string message);
	// parseChannelModes.cpp
	void						handleQuit(Client &client, std::string message);
	
	//handleModesParsing.cpp
	bool						checkValidParameter(int index, std::vector<std::string> parameter, char mode, Channel *channel, Client& client);
	bool						checkForValidModes(const std::string& message, Client& client, Channel* channel);
	void						handleMode(Client& client, const std::string& channelName, const std::string& message);
	
	//handleModesExecution.cpp
	std::string					compressModes(const std::string& setModes);
	void						executeModes(Client& client, Channel* channel);
	
	//channelClientGetters.cpp
	std::vector<Channel *>&		getChannels();
	std::vector<Client *>&		getClients();
	bool						channelExists(const std::string& channelName);
	Channel*					getChannelByChannelName(const std::string& channelName);
	Client*						getClientByNickname(const std::string& nickname);
	bool						userIsMemberOfChannel(Client &client, const std::string& channelName);

	bool						checkIfChannelExists(const std::string& channelName);
	bool						clientExists(const std::string& nick);

private:

	int							_port = 6667; // Example port
	std::string					_passwd;
	std::vector<Client *>		_clients;
	std::vector<Channel *>		_channels;
};

void removeWhitespace(std::string &str);
