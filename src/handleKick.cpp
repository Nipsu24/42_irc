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

/*Kicks clients from channels, checks first if channel exists, then uses try / catch to account for potential
  errors such as "no operator", "user not in channel" or "user does not exist". Furthermore, checks if reason
  is empty or only consists of whitespace and ':'. If this is the case, reason is treated as empty and the user's
  nickname is passed to message function as "reason". If reaon is not empty, string manipulation is conducted in order
  to remove any whitespace characters at the end of the string and potential ':' at beginning of string.*/
void Server::handleKick(Client &client, std::string message)
{
	std::istringstream	iss(message);
	std::string			command;
	std::string			channelName;
	std::string			nick;
	std::string			reason;
	std::string			kickMessage;
	bool				reasonExist = false;
	iss >> command;
	iss >> channelName;
	iss >> nick;
	std::getline(iss, reason);

	reason.erase(0, reason.find_first_not_of(' '));
	if (!checkIfChannelExists(channelName))
	{
		MessageServerToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
		return;
	}
	if (!reason.empty() && !(std::all_of(reason.begin(), reason.end(), [](unsigned char ch) { return std::isspace(ch) || ch == ':'; }))) {
		reasonExist = true;
		reason.erase(reason.find_last_not_of(" \n\r\t")+1);
		if (reason[0] == ':')
			reason.erase(0, 1);
	}
	//Channel handles this
	try{
		getChannelByChannelName(channelName)->setKick(&client, channelName, nick);
		if (reasonExist == true)
			kickMessage = RPL_KICK(client.getNick(), channelName, nick, reason);
		else
			kickMessage = RPL_KICK(client.getNick(), channelName, nick, nick);
		for (Client *member : getChannelByChannelName(channelName)->getUsers()) {
			MessageServerToClient(*member, kickMessage);
		}
			MessageServerToClient(*getClientByNickname(nick), kickMessage);
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
}
