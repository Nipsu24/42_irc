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

void Server::handleTopic(Client &client, std::string message)
{
	std::istringstream iss(message);
	std::string command;
	std::string channelName;
	std::string topic;
	iss >> channelName;
	std::getline(iss, topic);
	topic.erase(0, topic.find_first_not_of(' '));
	

	if (!checkIfChannelExists(channelName))
	{
		std::string response = ":localhost 403 " + client.getNick() + " " + channelName + " :No such channel";
		MessageServerToClient(client, response);
		return;
	}
	try{
		getChannelByChannelName(channelName)->setTopic(&client, channelName, topic);
	}
	catch (const Channel::ClientNotOperatorException &e) {
		std::string response = ":localhost 482 " + client.getNick() + " " + channelName + " :You're not a channel operator";
		MessageServerToClient(client, response);
	}
	catch (const Channel::ChannelNotFoundException &e) {
		std::string response = ":localhost 442 " + client.getNick() + " " + channelName + " :You're not on that channel";
		MessageServerToClient(client, response);
	}
	//:copper.libera.chat 482 timppa #testchannel24 :You're not a channel operator
	//:copper.libera.chat 403 timppa #jeecahnnel :No such channel
	//:copper.libera.chat 442 timppa #testchannel24 :You're not on that channel
}