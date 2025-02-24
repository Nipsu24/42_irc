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
#include <sstream>

/*Handles invite of a user to a channel. First checks whether channel and client exists, then
  sends invite in case respective parameters such as inviter is operator, in the channel and
  invitee is not yet in the channel. Finally adds invitee to invitation list. */
void Server::handleInvite(Client &client, std::string message)
{
	std::cout << "try to invite" << std::endl;
	std::istringstream	iss(message);
	std::string			command;
	std::string			nick;
	std::string			channelName;
	iss >> command;
	iss >> nick;
	iss >> channelName;
	
	if (!checkIfChannelExists(channelName)) {
		MessageServerToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
		return;
	}
	Client* clientToInvite = getClientByNickname(nick);
	if (clientToInvite == nullptr) {
		MessageServerToClient(client, ERR_NOSUCHNICK(client.getNick(), nick));
		return;
	}
	try {
		getChannelByChannelName(channelName)->setInvite(&client, channelName, nick);
	}
	catch (const Channel::ClientNotOperatorException &e) {
		MessageServerToClient(client, ERR_CHANOPRIVSNEEDED(client.getNick(), channelName));
	}
	catch (const Channel::ClientNotInChannelException &e) {
		MessageServerToClient(client, ERR_USERNOTINCHANNEL(client.getNick(), nick, channelName));
	}
	catch (const Channel::ClientAlreadyInChannelException &e) {
		MessageServerToClient(client, ERR_USERONCHANNEL(client.getNick(), nick, channelName));
	}
	MessageServerToClient(*clientToInvite, RPL_INVITING(client.getNick(), nick, channelName));
	getChannelByChannelName(channelName)->addToInvitationList(clientToInvite);
}
