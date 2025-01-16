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
void Server::handleClientMessage(Client &client, const std::string &message)
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
            handleJoin(client, tokens, i);
        else if (token == "PRIVMSG")
            handlePrivmsg(client, tokens, i, message);
        else if (token == "NICK")
            handleNick(client, tokens, i);
        else if (token == "KICK")
            handleKick(client, tokens, i);
        else if (token == "PING")
            MessageServerToClient(client, "PONG");
		else if (token == "MODE")
           handleMode(client, tokens, i, message);

        // else if (token == "TOPIC")
        //     handleTopic(client, tokens, i);
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
