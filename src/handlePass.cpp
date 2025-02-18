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
#include <vector>
#include <arpa/inet.h>

void Server::handlePass(Client &client, const std::vector<std::string>& tokens, int index)
{
	std::cout << "paswwd = " + tokens[index + 1] << std::endl;
	std::string response;
	if (tokens[index + 1] == _passwd)
	{
		MessageServerToClient(client, RPL_PASSWDOK());
		std::cout << "Password ok and ready to register" << std::endl;
		client.setState(REGISTERING);
	}
	else
	{
		std::cout << "not good pass" << std::endl;
		MessageServerToClient(client, ERR_PASSWDMISMATCH(client.getNick()));
		removeClient(client.getFd());
		close(client.getFd());
	}
		
}