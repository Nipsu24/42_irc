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
 * Handle the USER message
 */
void Server::handleUserName(Client &client, std::vector<std::string> tokens, int index)
{

	client.setNick(tokens[index + 1] + "_");
	client.setUsername(tokens[index + 1]);
	std::cout << "Received USER from client" << client.getFd() << ": " << client.getNick() << std::endl;
	std::string response = "001 " + client.getNick() + " :Welcome to the Internet Relay Network " + client.getNick();
	std::cout << "nick: " + client.getNick() + ", username: " + client.getUsername() << std::endl;
	MessageServerToClient(client, response);
}