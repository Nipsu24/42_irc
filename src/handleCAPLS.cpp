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

/*
 * Handle the CAP LS message
 */
void Server::handleCAPLS(Client &client, std::vector<std::string> tokens, int index)
{
	std::string response;
	if (tokens[index + 1] == "LS")
	{
		client.setState(REGISTERING);
		std::cout << "Received CAP LS from client" << client.getFd() << ": " << client.getNick() << std::endl; // Debugging
		response = "CAP * LS";
		MessageServerToClient(client, response);
		
	}
	else if (tokens[index + 1] == "END")
	{

		client.setState(REGISTERED);
		response = ":001 " + client.getNick() + ":Welcome to the Internet Relay Network " + client.getNick();
		MessageServerToClient(client, response);
	}
	else
	{
		std::cerr << "Invalid CAP message" << std::endl;
	}
}