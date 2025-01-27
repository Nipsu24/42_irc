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

/*Adds user(client) to a channel or creates new channel in case channel is not yet existing.
  First checks if channel name already exists in vector array of _channels. If this is the case,
  adds client to the channel (if no channel restrictions apply). Otherwise creates pointer to a 
  new channel via 'new' (to ensure that class will exist further on and not go out of scope when 
  function terminates). Respective memory is freed again in destructor of server channel*/
void	Server::handleJoin(Client &client, std::string channelName, std::string password)
{
	if (channelName == "")
	{
		MessageServerToClient(client, "\r\n");
		return;
	}
	else
	{
		std::cout << "Received JOIN from client" << client.getFd() << ": " << client.getNick() << std::endl;
		bool channelExists = false;
		for (Channel *availableChannels : _channels)
		{
			if (channelName == availableChannels->getChannelName())
			{
				if (!availableChannels->checkForModeRestrictions(client, password,
					[&](Client &client, const std::string &response) { MessageServerToClient(client, response); }))
					return ;
				availableChannels->addClient(&client);
				std::string response = ":" + client.getNick() + " JOIN " + channelName;
				MessageServerToClient(client, response);
				channelExists = true;
				break;
			} 
		}
		if (channelExists == false)
		{
			
			Channel *newChannel = new Channel(channelName);
			newChannel->addClient(&client);
			_channels.push_back(newChannel);
			newChannel->setChOperator(&client);
			//sends message to client that client is joined and operator
			std::string response = ":" + client.getNick() + " JOIN " + channelName;
			MessageServerToClient(client, response);
			std::string setOperatorResponse = ":localhost 353 " + client.getNick() + " @ " + channelName + " :@" + client.getNick();
			MessageServerToClient(client, setOperatorResponse);
			std::string setEnd = ":localhost 366 " + client.getNick() + " " + channelName + " :End of /NAMES list.";
			MessageServerToClient(client, setEnd);
		}
	}
}

//>> :zirconium.libera.chat 353 timo_ @ #testi222 :@timo_   
//:zirconium.libera.chat 366 timo_ #testi222 :End of /NAMES list.
