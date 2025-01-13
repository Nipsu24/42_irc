#pragma once

#include <string>
#include <netinet/in.h>

class Client {
public:
    // Constructors
    Client();
    Client(int fd, const sockaddr_in& client_addr);
    Client(const Client& other);
    Client& operator=(const Client& other);
    ~Client();

    void		setFd(int value);
    int			getFd() const;
	sockaddr_in	getAddr() const;
	std::string	getNick() const;

private:
    int			_fd;
	sockaddr_in	_addr;
	std::string	_nickname;

};


