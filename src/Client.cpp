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

Client::Client() {}

Client::Client(int fd, const sockaddr_in &client_addr)
    : _fd(fd), _addr(client_addr), _nick(""), _userName("") {}

Client::Client(const Client &other)
{
    this->_fd = other._fd;
}

Client &Client::operator=(const Client &other)
{
    if (this != &other)
    { // Self-assignment check
        this->_fd = other._fd;
    }
    return *this;
}

Client::~Client() {}

void Client::setFd(int value)
{
    _fd = value;
}

int Client::getFd() const
{
    return _fd;
}

sockaddr_in Client::getAddr() const
{
    return _addr;
}

void Client::setNick(const std::string &value)
{
    _nick = value;
}

std::string Client::getNick() const
{
    return _nick;
}

void Client::setUsername(const std::string &value)
{
    _userName = value;
}

std::string Client::getUsername() const
{
    return _userName;
}

void Client::setState(clientState state)
{
    _state = state;
}

clientState Client::getState() const
{
    return static_cast<clientState>(_state);
}


