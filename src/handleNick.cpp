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
	
	if (client.getState() != REGISTERED)
	{
		std::string uniqueNick = nick;
		while (clientExists(uniqueNick)) {
			uniqueNick += "_";
		}
		client.setNick(uniqueNick);
	}
	else
	{
		if (clientExists(nick))
		{
			std::string response = ":localhost 433 " + oldNick + " " + nick + ":Nickname is already in use.";
			MessageServerToClient(client, response);
		}
		else
		{
			client.setNick(nick);
			std::string response = ":" + oldNick + "!~" + client.getUsername() + "@localhost NICK :" + client.getNick();
			MessageServerToClient(client, response);
		}
	}
}