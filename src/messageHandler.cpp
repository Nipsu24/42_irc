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
    
    if (client.getState() == REGISTERING)
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
                handleCAPs(client, tokens, i);
            if (token == "PASS")
                handlePass(client, tokens, i);
            if (token == "USER")
                handleUserName(client, tokens, i);
            if (token == "NICK")
            {
                std::cout << "SETNICK FIRST TIME: " + tokens[i + 1] << std::endl;
                handleNick(client, tokens[i + 1]);
            }
            i++;
        }
    }
    else
    {
        std::string command;
		std::string	password;
		std::cout << "RAW MESSAGE: " << message << std::endl;
        std::istringstream iss(message);
        iss >> command;

		if (command == "PING")
        {
            std::string rest;
            std::getline(iss, rest);
            handlePingPong(client, rest);
        }
        else if (command == "JOIN")
        {
            std::string channelName;
            iss >> channelName;
			iss >> password;
            handleJoin(client, channelName, password);
        }
        else if (command == "PRIVMSG")
        {
            std::string channelNameOrNick;
            std::string rest;
            iss >> channelNameOrNick;
            std::getline(iss, rest);
            handlePrivmsg(client, channelNameOrNick, rest);
        }
        else if (command == "NICK")
        {
            std::string nick;
            iss >> nick;
            handleNick(client, nick);
        }
		else if (command == "MODE")
        {
			std::string channelName;
            std::string rest;
            iss >> channelName;
            std::getline(iss, rest);
			handleMode(client, channelName, rest);
        }
        else if (command == "TOPIC")
        {
			std::string channelName;
			iss >> channelName;
			std::string rest;
			std::getline(iss, rest);
			handleTopic(client, channelName, rest);
        }
          else if (command == "KICK")
        {
            handleKick(client, message);
        }
        else if (command == "INVITE")
        {
            handleInvite(client, message);
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
