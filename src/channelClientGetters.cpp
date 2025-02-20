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
#include <sstream>
#include "Client.hpp"

/*THESE FUNCTIONS ARE USED IN THE CONTEXT OF CHANNEL MODES HANDLING*/

/*Returns all channel objects from the server*/
std::vector<Channel *>& Server::getChannels() {
	return (_channels);
}

/*Returns all client objects from the server*/
std::vector<Client *>& Server::getClients() {
	return (_clients);
}

/*Checks if passed channel name exists in vector of channels. Returns true in case channel exists.*/
bool	Server::channelExists(const std::string& channelName)
{
	auto it = find_if(getChannels().begin(), getChannels().end(), [&channelName](Channel *channel)
	{ return channel->getChannelName() == channelName; });
	if (it != getChannels().end())
		return (true);
	else
		return (false);
}

/*Returns Channel object by passing it's name to the function. If object cannot be found by channel
  name, returns nullptr.*/
Channel*	Server::getChannelByChannelName(const std::string& channelName)
{
	const auto& channels = getChannels();
	auto it = std::find_if(channels.begin(), channels.end(), [&channelName](Channel* channel)
	{ return channel->getChannelName() == channelName; });
	if (it != channels.end())
		return (*it);
	return (nullptr);
}

/*Returns Client object by passing it's name to the function. If object cannot be found by client
  name, returns nullptr.*/
Client*	Server::getClientByNickname(const std::string& nickname)
{
	const auto& client = getClients();
	auto it = std::find_if(client.begin(), client.end(), [&nickname](Client* client)
	{ return client->getNick() == nickname; });
	if (it != client.end())
		return (*it);
	return (nullptr);
}

/*Returns client object by passing a client nickname to the method.*/
Client*	Channel::getClientByNickname(const std::string& nickname)
{
	const auto& client = getUsers();
	auto it = std::find_if(client.begin(), client.end(), [&nickname](Client* client)
	{ return client->getNick() == nickname; });
	if (it != client.end())
		return (*it);
	return (nullptr);
}

/*Checks if user is member of channel by retrieving channel with help of passed channel name and then
  check within the user list of the channel of occurence for passed user(client).*/
bool	Server::userIsMemberOfChannel(Client &client, const std::string& channelName)
{
	auto it = std::find_if(getChannels().begin(), getChannels().end(), [&channelName](Channel* channel)
	{ return channel->getChannelName() == channelName; });
	if (it != getChannels().end()) {
		Channel* channel = *it;
		auto& users = channel->getUsers();
		auto userIt = std::find(users.begin(), users.end(), &client);
		if (userIt != users.end())
			return (true);
	}
	return (false);
}
