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
#include <sstream>
#include <string>
#include <vector>


struct Parse
{
	std::string command = "";
	std::string channelName = "";
	std::string nickName = "";
	std::string userName = "";
	std::string message = "";
	bool toAll = false;
	int receiverFD = -1;
};

Parse parseMessage(const std::string& input) {

	Parse newRequest;
	std::istringstream iss(input);
	std::string temp;

	// Extract command
	iss >> newRequest.command;

	// Extract channel namejoinChannel(client, currentRequest.channelName);
	if (iss >> temp) {
		if (!temp.empty() && temp[0] == '#') {
			newRequest.channelName = temp;
		} else {
			newRequest.message = temp;
		}
	}

	std::string remainingMessage;
	std::getline(iss, remainingMessage);

	if (!remainingMessage.empty()) {

		if (remainingMessage[0] == ' ') {
			remainingMessage = remainingMessage.substr(1);
		}

		if (!newRequest.message.empty()) {
			newRequest.message += " ";
		}
		newRequest.message += remainingMessage;
	}

	// Output the results

	return newRequest;
}


