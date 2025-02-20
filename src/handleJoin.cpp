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

/*Adds user(client) to a channel or creates new channel in case channel is not yet existing.
  First checks if channel name already exists in vector array of _channels. If this is the case,
  adds client to the channel (if no channel restrictions apply) and sends message about new member
  to all members in channel. Otherwise creates pointer to a new channel via 'new' (to ensure that
  class will exist further on and not go out of scope when function terminates).*/
void	Server::handleJoin(Client &client, std::string channels, std::string password)
{
	if (channels == "")
		MessageServerToClient(client, "\r\n");
	else
	{
		std::stringstream ss(channels);
		std::string channelName;

		while (std::getline(ss, channelName, ',')) 
		{
			std::string namesList = "";
			bool channelExists = false;
			for (Channel *availableChannels : _channels) {
				if (channelName == availableChannels->getChannelName()) {
					std::string namesList = "";
					if (!availableChannels->checkForModeRestrictions(client, password,
						[&](Client &client, const std::string &response) { MessageServerToClient(client, response); }))
						return ;
					availableChannels->addClient(&client);
					for (Client *clientsIn : availableChannels->getUsers())
						namesList.append(clientsIn->getNick() + " ");
					for (Client *member : getChannelByChannelName(channelName)->getUsers()) {
						MessageServerToClient(*member, RPL_JOIN(client.getNick(), channelName));
						MessageServerToClient(*member, RPL_NAMREPLY(client.getNick(), channelName, namesList));
						MessageServerToClient(*member, RPL_ENDOFNAMES(client.getNick(), channelName));
					}
					channelExists = true;
					return;
				} 
			}
			if (channelExists == false) {
				Channel *newChannel = new Channel(channelName);
				newChannel->setTimestamp();
				newChannel->addClient(&client);
				_channels.push_back(newChannel);
				newChannel->setChOperator(&client);
				//sends message to client that client is joined and operator
				MessageServerToClient(client, RPL_JOIN(client.getNick(), channelName));
				MessageServerToClient(client, RPL_NAMREPLY(client.getNick(), channelName, client.getNick()));
				MessageServerToClient(client, RPL_ENDOFNAMES(client.getNick(), channelName));
				return ;
			}
		}
		
	}
}
