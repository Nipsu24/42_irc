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
#include "response.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

/*Checks if conditions for setting topic are met and if this is the case, sets respective
  topic for channel. First trims topic message string end from by any white space, then checks if 
  channel exists, and in the setTopic function varifies if client is part of the channel
  and if the topic operator only mode is active. Catches respective exceptions thrown by
  setTopic function. Broadcast topic change to all members of the channel*/
void Server::handleTopic(Client &client, const std::string& channelName, std::string message)
{
	std::istringstream iss(message);
	std::getline(iss, message);

	message.erase(0, message.find_first_not_of(' '));
	message.erase(message.find_last_not_of(" \n\r\t")+1);
	
	if (!checkIfChannelExists(channelName))
	{
		MessageServerToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
		return;
	}
	try{
		getChannelByChannelName(channelName)->setTopic(&client, message);
	}
	catch (const Channel::ClientNotOperatorException &e) {
		MessageServerToClient(client, ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
	}
	catch (const Channel::ClientNotInChannelException &e) {
		MessageServerToClient(client, ERR_NOTONCHANNEL(client.getNick(), channelName));
	}
	std::string response = ":" + client.getNick() + " " + "TOPIC " + channelName + " " + message;
	for (Client *member : getChannelByChannelName(channelName)->getUsers())
		MessageServerToClient(*member, RPL_TOPIC(client.getNick(), channelName, message));
}
