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
  adds client to the channel (if no channel restrictions apply). Otherwise creates pointer to a 
  new channel via 'new' (to ensure that class will exist further on and not go out of scope when 
  function terminates). Respective memory is freed again in destructor of server channel*/

/*ADDs now user to several channels in if /join #cahnnel1,channel2.   Password still needs to be done*/
void	Server::handleJoin(Client &client, std::string channels, std::string password)
{
	if (channels == "")
	{
		MessageServerToClient(client, "\r\n");
		return;
	}
	else
	{
		std::stringstream ss(channels);
		std::string channelName;

		while (std::getline(ss, channelName, ',')) 
		{
			std::cout << "channel; " << channelName << std::endl;

			std::string namesList = "";
			std::cout << "Received JOIN from client" << client.getFd() << ": " << client.getNick() << std::endl;
			bool channelExists = false;
			for (Channel *availableChannels : _channels)
			{
				if (channelName == availableChannels->getChannelName())
				{
					std::string namesList = "";
					
					for (Client *clientsIn : availableChannels->getUsers())
					{
						namesList.append(clientsIn->getNick() + " ");
					}
					if (!availableChannels->checkForModeRestrictions(client, password,
						[&](Client &client, const std::string &response) { MessageServerToClient(client, response); }))
						return ;
					availableChannels->addClient(&client);
					for (Client *member : getChannelByChannelName(channelName)->getUsers()) {
						MessageServerToClient(*member, RPL_JOIN(client.getNick(), channelName));
						MessageServerToClient(*member, RPL_NAMREPLY(client.getNick(), channelName, namesList));
						MessageServerToClient(*member, RPL_ENDOFNAMES(client.getNick(), channelName));
					}
					channelExists = true;
					break;
				} 
			}
			if (channelExists == false)
			{
				
				Channel *newChannel = new Channel(channelName);
				newChannel->setTimestamp();
				newChannel->addClient(&client);
				_channels.push_back(newChannel);
				newChannel->setChOperator(&client);
				//sends message to client that client is joined and operator
				MessageServerToClient(client, RPL_JOIN(client.getNick(), channelName));
				MessageServerToClient(client, RPL_NAMREPLY(client.getNick(), channelName, client.getNick()));
				MessageServerToClient(client, RPL_ENDOFNAMES(client.getNick(), channelName));
			}
		}
		
	}
}

//>> :zirconium.libera.chat 353 timo_ @ #testi222 :@timo_   
//:zirconium.libera.chat 366 timo_ #testi222 :End of /NAMES list.
