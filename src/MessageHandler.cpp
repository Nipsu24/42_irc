#include "Server.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <poll.h>
#include <unistd.h>
#include <string_view>

// Function to handle the server's response to the client
void Server::MessageServerToClient(Client client, const std::string &message)
{
    std::string formattedMessage = message + "\r\n"; // IRC message format
    if (send(client.getFd(), formattedMessage.c_str(), formattedMessage.size(), 0) == -1)
    {
        perror("Error sending message to client");
    }
}

// A sample MessageClientToServer function that processes the client's message
void Server::MessageClientToServer(Client &client, const std::string &message)
{
    // Split the message by spaces

    // // Check if the message is a CAP LS message
    if (startswith(message, "CAP LS"))
    {
        std::cout << "Handling CAP LS message" << std::endl;
        HandleCAPLS(client, message);
    }
    else if (startswith(message, "USER"))
    {
        std::cout << "Handling USER message" << std::endl;
        handleUserName(client, message);
    }

    else if (startswith(message, "CAP REQ"))
    {
        std::cout << "Handling CAP REQ message" << std::endl;
        handleCapReq(client, message);
    }
    else
    {
        std::cout << "test" << std::endl;
        // Handle other messages here
    }
}

// A simple function to split a string by spaces
std::vector<std::string> Server::SplitString(const std::string &str, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(str);

    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }

    return tokens;
}

bool Server::startswith(const std::string &str, const std::string &cmp)
{
    return str.compare(0, cmp.length(), cmp) == 0;
}
// Function to handle the CAP LS message
void Server::HandleCAPLS(Client &client, const std::string &message)
{
    // Find the position of "NAME" in the message
    size_t namePos = message.find("NICK");
    if (namePos != std::string::npos)
    {
        size_t startPos = namePos + 5; // 5 is the length of "NAME "
        size_t endPos = message.find(' ', startPos);
        std::string clientName = message.substr(startPos, endPos - startPos);

        // Assign the extracted name to the client's name
        client.setNick(clientName);
        std::cout << "Client" << client.getFd() << " name set to: " << client.getNick() << std::endl;
    }

    // client.cap_status = true; // Set the client's CAP status to true

    // Send the CAP LS response to the client
}

void Server::handleUserName(Client &client, const std::string &message)
{

    std::cout << "test" << std::endl;
    size_t namePos = message.find("USER");
    if (namePos != std::string::npos)
    {
        size_t startPos = namePos + 5; // 5 is the length of "NAME "
        size_t endPos = message.find(' ', startPos);
        std::string username = message.substr(startPos, endPos - startPos);

        // Assign the extracted name to the client's name
        client.setUsername(username);
        std::cout << "Client" << client.getFd() << " name set to: " << client.getNick() << std::endl;
    }
    std::string response = ":irc.example.com CAP * LS :account-notify extended-join multi-prefix sasl\r\n";
    MessageServerToClient(client, response);
}

void Server::handleCapReq(Client &client, const std::string &message)
{
    std::cout << "Received CAP REQ from client" << client.getFd() << ": " << client.getNick() << std::endl;
    std::cout << "Message: " << message << std::endl;
}
