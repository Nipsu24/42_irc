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

void Server::handleInvite(Client &client, std::string message)
{
	std::cout << "try to invite" << std::endl;
	std::istringstream iss(message);
	std::string command;
	std::string nick;
	std::string channelName;
	iss >> command;
	iss >> nick;
	iss >> channelName;
	
	if (!checkIfChannelExists(channelName))
	{
		MessageServerToClient(client, ERR_NOSUCHCHANNEL(client.getNick(), channelName));
		return;
	}
	Client* clientToInvite = getClientByNickname(nick);
	if (clientToInvite == nullptr)
	{
		MessageServerToClient(client, ERR_NOSUCHNICK(client.getNick(), nick));
		return; // Exit the function after handling the error
	}
	
	
	//checks if client is in channel and is client operator
	try{
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
	//everything ok, lets invite
	MessageServerToClient(*clientToInvite, RPL_INVITING(client.getNick(), nick, channelName));
	getChannelByChannelName(channelName)->addToInvitationList(clientToInvite);
}


//:iridium.libera.chat 341 timo_ timo___ #testii2

//:platinum.libera.chat 443 timo___ timo_ #testii :is already on channel

//:platinum.libera.chat 341 timo___ timo____ #testii :Inviting timo____ to channel #testii

//:platinum.libera.chat 401 timo___ #testii :No such nick/channel

//:platinum.libera.chat 403 timo___ #testii :No such channel

//:platinum.libera.chat 482 timo___ #testii :You're not a channel operator

//:platinum.libera.chat 441 timo___ timo____ #testii :They aren't on that channel

//:platinum.libera.chat 442 timo___ #toka :You're not on that channel

//:timo___!~timo@mobile-access-b04820-247.dhcp.inet.fi INVITE timo____ :#testii  ->to invited client