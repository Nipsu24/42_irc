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
#include <arpa/inet.h>



void Server::handlePingPong(Client &client)
{
	std::string response = ":localhost PONG localhost :localhost";
	MessageServerToClient(client, response);
	int i = 0;
	for (Channel *channel : _channels)
	{
		std::cout << "channel " << i <<  ": " << channel->getChannelName() << std::endl;
		i++;
		
		
	}
	std::cout << "number of clients: " << getClients().size() << std::endl;

	for (Client *client : _clients)
	{
		std::cout << "client: " << client->getFd() 
          << " from IP: " << inet_ntoa(client->getAddr().sin_addr) 
          << " username: " << client->getUsername() 
          << ". nick: " << client->getNick() 
          << std::endl;
	}
}

