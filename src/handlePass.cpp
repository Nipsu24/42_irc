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
#include <algorithm>

void Server::handlePass(Client &client, const std::vector<std::string>& tokens, int index)
{
	std::string response;
	if (tokens[index + 1] == _passwd)
	{
		MessageServerToClient(client, RPL_PASSWDOK());
		client.setPasswdOK(true);
	}
	else
	{
		std::cout << "wrong password" << std::endl;
		MessageServerToClient(client, ERR_PASSWDMISMATCH(client.getNick()));
		close(client.getFd());
		_clients.erase(std::remove_if(_clients.begin(), _clients.end(),
		[&](Client* c) { return c->getFd() == client.getFd(); }), _clients.end());
	}
}