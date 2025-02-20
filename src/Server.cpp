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

#include "Server.hpp"
#include "response.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <system_error>


Server::Server() {}

Server::Server(int _port, std::string _passwd) :
	_port(_port),
	_passwd(_passwd)
	{}

Server::Server(const Server& other) {
	this->_port = other._port;
	this->_passwd = other._passwd;
}

Server& Server::operator=(const Server& other) {
	if (this != &other) {  // Self-assignment check
		this->_port = other._port;
		this->_passwd = other._passwd;
	}
	return *this;
}

/*Destructor frees dynamically allocated channels. Uses '.clear'
  at the end to avoid dangling pointers.*/
Server::~Server() {}

void Server::setPort(int value) {
	_port = value;
}

int Server::getPort() const {
	return _port;
}

void Server::setPassword(std::string value) {
	_passwd = value;
}

std::string Server::getPassword() const {
	return _passwd;
}

void Server::addClient(Client* client) {
	_clients.push_back(client);
}

void Server::removeClient(int fd) {
	for (auto it = _clients.begin(); it != _clients.end(); ++it) {
		if ((*it)->getFd() == fd) {
			delete *it;  // Clean up memory
			_clients.erase(it);  // Remove from vector
			break;
		}
	}
}

bool Server::checkIfChannelExists(const std::string& channelName) {
    for (Channel* channel : _channels) {
        if (channel->getChannelName() == channelName) {
            return true; 
        }
    }
    return false;
}


bool Server::clientExists(const std::string& nick){
	for (Client* client : _clients)
	{
		if (client->getNick() == nick)
			return true;
	}
	return false;
}


void Server::handleCAPs(Client &client, const std::vector<std::string>& tokens, int index)
{
	std::string response;

	if (tokens[index + 1] == "LS")
	{
		//std::cout << "Received CAP LS from client" << client.getFd() << ": " << client.getNick() << std::endl; // Debugging
		response = "CAP * LS :multi-prefix sasl";
		MessageServerToClient(client, response);
	}
	else if (tokens[index + 1] == "REQ")
	{
		response = ":localhost CAP " + client.getNick() + " ACK :multi-prefix";
		MessageServerToClient(client, response);
	}
	else if (tokens[index + 1] == "END")
	{
		MessageServerToClient(client, response);
		client.setState(REGISTERED);
	}
	else
	{
		std::cerr << "Invalid CAP message" << std::endl;
	}
}