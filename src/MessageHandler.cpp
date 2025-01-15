#include "Server.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <poll.h>
#include <unistd.h>
#include <string_view>
#include <regex>
#include <vector>

/*
 * Handle events on the server
 */
void Server::MessageServerToClient(Client client, const std::string &message)
{
    std::string formattedMessage = message + "\r\n"; // IRC message format
    if (send(client.getFd(), formattedMessage.c_str(), formattedMessage.size(), 0) == -1)
    {
        perror("Error sending message to client");
    }
}

/*
 * Handle messages from the client
 */
void Server::MessageClientToServer(Client &client, const std::string &message)
{
    std::vector<std::string> tokens = SplitString(message);
    if (tokens.size() < 1)
    {
        std::cerr << "Invalid message" << std::endl;
        return;
    }
    int i = 0;
    for (auto &token : tokens)
    {
        std::cout << "Client: " << client.getFd() << " Token: " << i << " " << token << std::endl; // Debugging cout to see the tokens
        if (token == "CAP")
            HandleCAPLS(client, tokens, i);
        else if (token == "USER")
            handleUserName(client, tokens, i);
        else if (token == "JOIN")
        {
            std::cout << "here";
            handleJoin(client, tokens, i);
        }
        else if (token == "PRIVMSG")
            handlePrivmsg(client, tokens, i);
        else if (token == "NICK")
            handleNick(client, tokens, i);
        i++;
    }
}

/*
 * Split a string by a delimiter
 */
std::vector<std::string> Server::SplitString(const std::string &str)
{
    std::vector<std::string> tokens;
    std::regex re("\\s+");
    std::sregex_token_iterator it(str.begin(), str.end(), re, -1);
    std::sregex_token_iterator end;

    while (it != end)
    {
        if (!it->str().empty()) // To avoid adding empty strings
        {
            tokens.push_back(*it);
        }
        ++it;
    }
    return tokens;
}

/*
 * Handle the CAP LS message
 */
void Server::HandleCAPLS(Client &client, std::vector<std::string> tokens, int index)
{
    std::string response;
    if (tokens[index + 1] == "LS")
    {
        client.setState(REGISTERING);
        std::cout << "Received CAP LS from client" << client.getFd() << ": " << client.getNick() << std::endl; // Debugging
        response = "CAP * LS";
        MessageServerToClient(client, response);
    }
    else if (tokens[index + 1] == "END")
    {
        response = ":001 " + client.getNick() + ":Welcome to the Internet Relay Network " + client.getNick();
        MessageServerToClient(client, response);
    }
    else
    {
        std::cerr << "Invalid CAP message" << std::endl;
    }
}

/*
 * Handle the USER message
 */
void Server::handleUserName(Client &client, std::vector<std::string> tokens, int index)
{
    client.setNick(tokens[index + 1] + "_");
    client.setUsername(tokens[index + 1]);
    std::cout << "Received USER from client" << client.getFd() << ": " << client.getNick() << std::endl;
    std::string response = "001 " + client.getNick() + " :Welcome to the Internet Relay Network " + client.getNick();
    MessageServerToClient(client, response);
}

void Server::handleNick(Client &client, std::vector<std::string> tokens, int index)
{
    client.setNick(tokens[index + 1]);
}

void Server::handleJoin(Client &client, std::vector<std::string> tokens, int index)
{
    if (tokens[index + 1] == "")
    {
        MessageServerToClient(client, "\r\n");
        return;
    }
    else
    {
        joinChannel(&client, tokens[index+1]);
        std::cout << "Received JOIN from client" << client.getFd() << ": " << client.getNick() << std::endl;
        std::string response = ":" + client.getNick() + " JOIN " + tokens[index + 1] + "\r\n";
        MessageServerToClient(client, response);
    }
}

void Server::handlePrivmsg(Client &client, std::vector<std::string> tokens, int index)
{
    std::string response = ":" + client.getNick() + " PRIVMSG " + tokens[index + 1] + " :" + tokens[index + 2] + "\r\n";
    MessageServerToClient(client, response);
}
