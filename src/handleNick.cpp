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
#include "iostream"

void Server::handleNick(Client &client, std::string nick)
{
	std::string channelName = "";
	std::string oldNick = client.getNick();
	
	std::string uniqueNick = nick;
	while (clientExists(uniqueNick)) {
        uniqueNick += "_";
    }
	client.setNick(uniqueNick);
	if (client.getState() == REGISTERED)
	{
		std::string response = ":" + oldNick + "!~" + client.getUsername() + "@localhost NICK :" + client.getNick();
		MessageServerToClient(client, response);

	}
}