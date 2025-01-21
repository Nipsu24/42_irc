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
#include "Channel.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

void Server::handleKick(Client &client, std::string message)
{
	std::istringstream iss(message);
	std::string command;
	std::string channelName;
	std::string nick;
	iss >> command;
	iss >> channelName;
	iss >> nick;


	if (!checkIfChannelExists(channelName))
	{
		std::string response = ":localhost 403 " + client.getNick() + " " + channelName + " :No such channel";
		MessageServerToClient(client, response);
		return;
	}
	//Channel handles this
	try{
		getChannelByChannelName(channelName)->setKick(&client, channelName, nick);
	}
	catch (const Channel::ClientNotOperatorException &e) {
		std::string response = ":localhost 482 " + client.getNick() + " " + channelName + " :You're not a channel operator";
		MessageServerToClient(client, response);
	}
	catch (const Channel::NickNotExistException &e) {
		std::string response = ":localhost 441 " + client.getNick() + " " + nick = " " + channelName + " :They aren't on that channel";
		MessageServerToClient(client, response);
	}
	catch (const Channel::ClientNotInChannelException &e) {
		std::string response = ":localhost 442 " + client.getNick() + " " + channelName + " :You're not on that channel";
		MessageServerToClient(client, response);
	}

	std::string response = ":" + client.getNick() + "!~localhost KICK" + channelName + nick + ":" + nick;
	MessageServerToClient(client, response);
}