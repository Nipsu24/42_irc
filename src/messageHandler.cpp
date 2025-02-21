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
#include "response.hpp"
#include <iostream>
#include <sstream>
#include <string>
#include <poll.h>
#include <unistd.h>
#include <string_view>
#include <regex>
#include <vector>
#include <stdexcept>

/*
 * Handle events on the server
 */
void Server::MessageServerToClient(Client client, const std::string &message)
{
    std::cout << ">> " << message << std::endl;
    std::string formattedMessage = message + "\r\n";
    if (send(client.getFd(), formattedMessage.c_str(), formattedMessage.size(), 0) == -1)
    {
        perror("Error sending message to client");
    }
}

/*
    Handle messages from the client
*/
void Server::handleClientMessage(Client &client, const std::string &message)
{
    std::cout << "<< " << message << std::endl;
    if (client.getState() == REGISTERING)
    {
        std::vector<std::string> tokens = SplitString(message);
        if (tokens.size() < 2)
        {
            MessageServerToClient(client, RPL_NICKREQUEST());
            return;
        }
        int i = 0;
        for (auto &token : tokens)
        {
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
        std::string args[3];
        std::istringstream iss(message);
        iss >> args[0];

		if (args[0] == "PING")
        {
            iss >> args[1];
            MessageServerToClient(client, "PONG " + args[1]);
        }
        else if (args[0] == "JOIN")
        {
            iss >> args[1];
			iss >> args[2];
            handleJoin(client, args[1], args[2]);
        }
        else if (args[0] == "PRIVMSG")
        {
            iss >> args[1];
            std::getline(iss, args[2]);
            handlePrivmsg(client, args[1], args[2]);
        }
        else if (args[0] == "NICK")
        {
            iss >> args[1];
            handleNick(client, args[1]);
        }
		else if (args[0] == "MODE")
        {
            iss >> args[1];
            std::getline(iss, args[2]);
			handleMode(client, args[1], args[2]);
        }
        else if (args[0] == "TOPIC")
        {
			iss >> args[1];
			std::getline(iss, args[2]);
			handleTopic(client, args[1], args[2]);
        }
        else if (args[0] == "KICK")
        {
            handleKick(client, message);
        }
        else if (args[0] == "INVITE")
        {
            handleInvite(client, message);
        }
        else if (args[0] == "QUIT")
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
        if (!it->str().empty())
        {
            tokens.push_back(*it);
        }
        ++it;
    }
    return tokens;
}
