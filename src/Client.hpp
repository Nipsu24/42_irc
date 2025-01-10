#pragma once

#include <string>
#include <netinet/in.h>

class Client
{
public:
    // Constructors
    Client();
    Client(int fd, const sockaddr_in &client_addr);
    Client(const Client &other);
    Client &operator=(const Client &other);
    ~Client();

    void setFd(int value);
    int getFd() const;
    void setNick(const std::string &value);
    std::string getNick() const;
    void setUsername(const std::string &value);
    std::string getUsername() const;
    sockaddr_in getAddr() const;

    bool cap_status;

private:
    int _fd;
    sockaddr_in _addr;
    std::string _nick;
    std::string _username;
};
