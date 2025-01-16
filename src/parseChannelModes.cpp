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

#include "Channel.hpp"
#include "Server.hpp"
#include <algorithm>

std::vector<Channel *>& Server::getChannels() {
	return (_channels);
}

/*Checks if passed channel name exists in vector of channels. Returns true in case channel exists.*/
bool	Server::channelExists(const std::string& channelName) {
	auto it = find_if(getChannels().begin(), getChannels().end(), [&channelName](Channel *channel) {
			return channel->getChannelName() == channelName;
		});
		if (it != getChannels().end())
			return (true);
		else
			return (false);
}

/*Checks if user is member of channel by retrieving channel with help of passed channel name and then
  check within the user list of the channel of occurence for passed user(client).*/
bool	Server::userIsMemberOfChannel(Client &client, const std::string& channelName) {
	auto it = std::find_if(getChannels().begin(), getChannels().end(), [&channelName](Channel* channel) {
		return channel->getChannelName() == channelName;
	});
	if (it != getChannels().end()) {
		Channel* channel = *it;
		// Check if the client exists in the user list
		auto& users = channel->getUsers();
		auto userIt = std::find(users.begin(), users.end(), &client);
		if (userIt != users.end())
			return (true);
	}
	return (false);
}

/*Currently covers /mode functionality without mode input*/
void Server::handleMode(Client &client, const std::string& channelName, const std::string &message) {
	(void)message;
	std::string response;
	if (message.empty() ) {
		if (channelExists(channelName)) {
			if (userIsMemberOfChannel(client, channelName)) {
				response = "324 " + client.getNick() + " " + channelName + " +Cnst"; // modes still to be inserted dynamically
			}
		}
		else {
			response = "403 " + client.getNick() + " " + channelName + ":No such channel"; 
		}
		MessageServerToClient(client, response);
	}
}

// void	Server::handleTopic(Client &client, const std::string& channelName, const std::string& topic) {
// 	if (channelExists(channelName)) {
// 		if (userIsMemberOfChannel(client, channelName)) {

// 		}
// 	}
// }
