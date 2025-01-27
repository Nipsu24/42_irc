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
#include "response.hpp"
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
			MessageServerToClient(client, ERR_NICKNAMEINUSE(oldNick, nick));
		else
		{
			client.setNick(nick);
			MessageServerToClient(client, RPL_NICK(oldNick, client.getUsername(), client.getNick()));
		}
	}
}