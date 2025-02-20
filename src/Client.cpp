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

#include "Client.hpp"

/* ************************************************Constructor Section START*************************************** */
Client::Client() {}

Client::Client(int fd, const sockaddr_in &client_addr)
    : _fd(fd), _addr(client_addr), _nick("*"), _userName(""), _passwdOK(false), _nickOK(false), _userNameOK(false) {}

Client::Client(const Client &other)
{
    this->_fd = other._fd;
	this->_state = other._state;
	this->_addr = other._addr;
	this->_nick = other._nick;
	this->_userName = other._userName;
	this->_passwdOK = other._passwdOK;
	this->_nickOK = other._nickOK;
	this->_userNameOK = other._userNameOK; 
}

Client &Client::operator=(const Client &other)
{
	if (this != &other)
	{
		this->_fd = other._fd;
		this->_state = other._state;
		this->_addr = other._addr;
		this->_nick = other._nick;
		this->_userName = other._userName;
		this->_passwdOK = other._passwdOK;
		this->_nickOK = other._nickOK;
		this->_userNameOK = other._userNameOK; 
	}
	return *this;
}

Client::~Client() {}

/* ************************************************Constructor Section END*************************************** */

void Client::setFd(int value) { _fd = value; }

int Client::getFd() const { return (_fd); }

sockaddr_in Client::getAddr() const { return (_addr); }

void Client::setNick(const std::string &value) { _nick = value; }

std::string Client::getNick() const { return (_nick); }

void Client::setUsername(const std::string &value) { _userName = value; }

std::string Client::getUsername() const { return (_userName); }

void Client::setState(clientState state) { _state = state; }

clientState Client::getState() const { return (static_cast<clientState>(_state)); }

bool Client::getPasswdOK() { return (_passwdOK); }

void Client::setPasswdOK(bool ok) { _passwdOK = ok; }

bool Client::getNickOK() { return (_nickOK); }

void Client::setNickOK(bool ok) { _nickOK = ok; }

bool Client::getUserNameOK() { return (_userNameOK); }

void Client::setUserNameOK(bool ok) { _userNameOK = ok; }
