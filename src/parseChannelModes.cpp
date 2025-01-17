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
#include <iostream>

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

Channel*	Server::getChannelByChannelName(const std::string& channelName) {
	const auto& channels = getChannels();
	auto it = std::find_if(channels.begin(), channels.end(), [&channelName](Channel* channel) {
		return channel->getChannelName() == channelName;
	});
	if (it != channels.end())
		return (*it);
	return (nullptr);
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

/*Currently covers /mode functionality without mode input, all_of function checks if there is only whitespace
  remaining in the message string. ChannelByChannelName function retrieves channel object with help of channel
  name, as it is required to call the getMode function.*/
void Server::handleMode(Client &client, const std::string& channelName, const std::string &message)
{
	std::string firstResponse;
	std::string	secondResponse;
	if ((std::all_of(message.begin(), message.end(), [](unsigned char ch) { return std::isspace(ch); })) || message.empty()) {
		if (channelExists(channelName)) {
			if (userIsMemberOfChannel(client, channelName)) {
				Channel* channel = getChannelByChannelName(channelName);
				firstResponse = "324 " + client.getNick() + " " + channelName + " " + channel->getMode();
				secondResponse = "329 " + client.getNick() + " " + channelName + " " + channel->getTimestamp();
				MessageServerToClient(client, firstResponse);
				MessageServerToClient(client, secondResponse);
			}
		}
		else {
			firstResponse = "403 " + client.getNick() + " " + channelName + ":No such channel"; 
			MessageServerToClient(client, firstResponse);
		}
	}
}

//>> @time=2025-01-17T09:08:37.182Z :lithium.libera.chat 324 mariusmeier #new_channel24 +
//@time=2025-01-17T09:08:37.182Z :lithium.libera.chat 329 mariusmeier #new_channel24 1737101855

//iklot
// void	Server::handleTopic(Client &client, const std::string& channelName, const std::string& topic) {
// 	if (channelExists(channelName)) {
// 		if (userIsMemberOfChannel(client, channelName)) {

// 		}
// 	}
// }
