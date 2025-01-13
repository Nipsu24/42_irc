#include "Client.hpp"

Client::Client() {}

Client::Client(int fd, const sockaddr_in& client_addr) 
        : _fd(fd), _addr(client_addr) {}

Client::Client(const Client& other) {
    this->_fd = other._fd;
}

Client& Client::operator=(const Client& other) {
    if (this != &other) {  // Self-assignment check
        this->_fd = other._fd;
    }
    return *this;
}

Client::~Client() {}

void Client::setFd(int value) {
    _fd = value;
}

int Client::getFd() const {
    return _fd;
}

sockaddr_in Client::getAddr() const {
	return _addr;
}

std::string	Client::getNick() const {return _nickname;}
