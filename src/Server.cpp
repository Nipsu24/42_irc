

#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <system_error>



#include "Server.hpp"

Server::Server() {}

Server::Server(int _port, std::string _passwd) :
	_port(_port),
	_passwd(_passwd) {}

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