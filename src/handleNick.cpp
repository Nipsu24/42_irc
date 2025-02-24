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

void Server::handleNick(Client &client, std::string nick)
{
	std::string oldNick = client.getNick();

	if (clientExists(nick))
		MessageServerToClient(client, ERR_NICKNAMEINUSE(oldNick, nick));
	else
	{
		client.setNick(nick);
		for (Channel *channel : _channels)
		{
			if (channel->isClientInChannel(&client))
			{
					for (Client *member : channel->getUsers())
					{
						if (member->getNick() != client.getNick())
							MessageServerToClient(*member, RPL_NICK(oldNick, client.getUsername(), client.getNick()));
					}
			}
		}
		MessageServerToClient(client, RPL_NICK(oldNick, client.getUsername(), client.getNick()));
		client.setNickOK(true);
	}
}