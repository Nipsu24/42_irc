#include "Server.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <poll.h>
#include <unistd.h>
#include <string_view>
#include <regex>
#include <vector>

/*void Server::sendToChannelClients(Client* client, std::string message, std::string channelName)
{

    for (Channel *channel : _channels)
    {
        std::cout << channel->getChannelName() << std::endl;
        if (channel->getChannelName() == channelName)
        {
            for (Client *_client : channel->getUsers())
            {
                if (&_client != &client)
                    MessageServerToClient(*_client, message);
            }
        }
    }

}*/

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
    if (client.getState() != REGISTERED)
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
                handleCAPLS(client, tokens, i);
            else if (token == "USER")
                handleUserName(client, tokens, i);
            i++;
        }
    }
    else
    {
        std::string command;
		std::cout << "RAW MESSAGE: " << message << std::endl;
        std::istringstream iss(message);
        iss >> command;

		if (command == "PING")
        {
            handlePingPong(client);
        }
        else if (command == "JOIN")
        {
            std::string channelName;
            iss >> channelName;
            handleJoin(client, channelName);
        }
        else if (command == "PRIVMSG")
        {
            std::string channelName;
            std::string rest;
            iss >> channelName;
            std::getline(iss, rest);
            handlePrivmsg(client, channelName, rest);
        }
        else if (command == "NICK")
        {
            std::string nick;
            iss >> nick;
            handleNick(client, nick);
        }
        else if (command == "TOPIC")
        {
            std::string channelName;
            std::string rest;
            iss >> channelName;
            std::getline(iss, rest);
            client.getLoggedIn()->setTopic(&client, channelName, rest);
        }
        else if (command == "KICK")
        {
            std::string channelName;
            std::string nick;
            iss >> channelName;
            iss >> nick;
            std::cout << channelName << "in" << nick << std::endl;
            client.getLoggedIn()->setKick(&client, channelName, nick);
        }
        else if (command == "MODE")
        {
			std::string channelName;
            std::string rest;
            iss >> channelName;
            std::getline(iss, rest);
			handleMode(client, channelName, rest);
        }
        else if (command == "INVITE")
        {
            std::string channelName;
            std::string nick;
            iss >> nick;
            iss >> channelName;
            client.getLoggedIn()->setInvite(&client, channelName, nick);
        }
        else if (command == "QUIT")
        {
            handleQuit(client, message);
        }
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
