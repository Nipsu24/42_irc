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

void Server::handleKick(Client &client, std::string message)
{
	std::istringstream iss(message);
	std::string command;
	std::string channelName;
	std::string nick;
	iss >> command;
	iss >> channelName;
	iss >> nick;
	std::string reason;
	std::getline(iss, reason);

	reason.erase(0, reason.find_first_not_of(' '));

	if (!checkIfChannelExists(channelName))
	{
		MessageServerToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
		return;
	}
	//Channel handles this
	try{
		getChannelByChannelName(channelName)->setKick(&client, channelName, nick);
	}
	catch (const Channel::ClientNotOperatorException &e) {
		MessageServerToClient(client, ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
	}
	catch (const Channel::NickNotExistException &e) {
		MessageServerToClient(client, ERR_USERNOTINCHANNEL(client.getNick(), nick, channelName));
	}
	catch (const Channel::ClientNotInChannelException &e) {
		MessageServerToClient(client, ERR_NOTONCHANNEL(client.getNick(), channelName));
	}
	MessageServerToClient(client, RPL_KICK(client.getNick(), channelName, nick, reason));
}