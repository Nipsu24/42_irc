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
#include <unordered_set>
#include <sstream>
#include "Client.hpp"

std::vector<Channel *>& Server::getChannels() {
	return (_channels);
}

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

/*Helper function of checkForValidModes, cheks if all strings of vector are all filled, if parameter for l mode only
  consists of digits and if parameter for o contains user which is member of the channel*/
bool	Server::checkValidParameter(int index, std::vector<std::string> parameter, char mode, Channel *channel, Client& client)
{
	std::string	response;

	if ((std::all_of(parameter[index].begin(), parameter[index].end(), [](unsigned char ch) { return std::isspace(ch); }))
		|| parameter[index].empty())
	{
		response = "461 " + client.getNick() + " MODE :Not enough parameters";
		MessageServerToClient(client, response);
		return (false);
	}
	if (mode == 'l' && !std::all_of(parameter[index].begin(), parameter[index].end(), ::isdigit))
	{
		response = "401 " + client.getNick() + " " + parameter[index] + " :No such nick/channel";
		MessageServerToClient(client, response);
		return (false);
	}
	if (mode == 'o') {
		Client* client = getClientByNickname(parameter[index]);
		if (!userIsMemberOfChannel(*client, channel->getChannelName())) {
			return (false);
		}
	}
	return (true);
}

/*First checks if any invalid character is occuring in the modes and returns a respective message if this
  is the case. Then divides + and - modes into separate strings and counts the occurence of k, l and o
  within the +string in order to determine how many parameters are to be expected in the rest of the 
  message. Then iterates further with iss using the parameter amount and stores the remaining content 
  of the message in the parameter vector.*/
bool	Server::checkForValidModes(const std::string& message, Client& client, Channel* channel)
{
	std::string 		modes;
	std::string			response;
	char				currentSign;
	int					neededParameterCount = 0;
	int					index = 0;
	std::unordered_set<char> allowedChars = {'+', '-', 'i', 'k', 'l', 'o', 't'};
	
	std::istringstream	iss(message);
	iss >> modes;
	for (char c : modes) {
		if (allowedChars.find(c) == allowedChars.end()) {
			response = "472 " + client.getNick() + " " + c + " :is an unknown mode char to me";
			MessageServerToClient(client, response);
			return (false);
		}
		if (c == '+' || c == '-')
			currentSign = c;
		else {
			if (currentSign == '+') {
				if (c == 'k' || c == 'l' || c == 'o')
					neededParameterCount++;
			}
			else if (currentSign == '-') {
				if (c == 'o')
					neededParameterCount++;
			}
		}
	}
	if (neededParameterCount > 0) {
		std::vector<std::string> parameter(neededParameterCount);
		for (int i = 0; i < neededParameterCount; i++)
			iss >> parameter[i];
		for (char c : modes) {
			if (c == '+' || c == '-')
				currentSign = c;
			else {
				if (currentSign == '+') {
					if (c == 'k' || c == 'l' || c == 'o') {
						if (!checkValidParameter(index, parameter, c, channel, client))
							return (false);
						index++;
					}
				}
				if (currentSign == '-') {
					if (c == 'o') {
						if (!checkValidParameter(index, parameter, c, channel, client))
							return (false);
						index++;
					}
				}
			}
		}
		channel->setParsedParameters(parameter);
	}
	channel->setParsedModes(modes);
	channel->clearParsedParameters();
	return (true);
}


// bool	Server::checkForValidModes(const std::string& message, Client& client, Channel* channel)
// {
// 	std::string 		modes;
// 	char				currentSign;
// 	std::string			response;
// 	std::string			plusModes;
// 	std::string			minusModes;
// 	int					neededParameterCount = 0;
// 	std::unordered_set<char> allowedChars = {'+', '-', 'i', 'k', 'l', 'o', 't'};
	
// 	std::istringstream	iss(message);
// 	iss >> modes;
// 	for (char c : modes) {
// 		if (allowedChars.find(c) == allowedChars.end()) {
// 			response = "472 " + client.getNick() + " " + c + " :is an unknown mode char to me";
// 			MessageServerToClient(client, response);
// 			return (false);
// 		}
// 		if (c == '+' || c == '-')
// 			currentSign = c;
// 		else {
// 			if (currentSign == '+') {
// 				plusModes += c;
// 				if (c == 'k' || c == 'l' || c == 'o')
// 					neededParameterCount++;
// 			}
// 			else if (currentSign == '-')
// 				minusModes += c;
// 		}
// 	}
// 	if (neededParameterCount > 0) {
// 		std::vector<std::string> parameter(neededParameterCount);
// 		for (int i = 0; i < neededParameterCount; i++)
// 			iss >> parameter[i];
// 		int	j = 0;
// 		int	result;
// 		for (char c : plusModes) {
// 			if (c == 'k' || c == 'l' || c == 'o') {
// 				result = checkValidParameter(j, parameter, c, channel);
// 				if (result == -1) {
// 					response = "461 " + client.getNick() + " MODE :Not enough parameters";
// 					MessageServerToClient(client, response);
// 					return (false);
// 				}
// 				if (result == 1) {
// 					response = "401 " + client.getNick() + " " + parameter[j] + " :No such nick/channel";
// 					MessageServerToClient(client, response);
// 					return (false);
// 				}
// 				j++;
// 			}
// 		}
// 		channel->setParsedParameters(parameter);
// 	}
// 	channel->setParsedModes(modes);
// 	//set parsedParemeters empty
// 	return (true);
// }

void	Server::executeModes(Client& client, Channel* channel)
{
	std::string	response;
	char		currentSign;
	int			i = 0;
	std::string	setModes;
	std::vector<std::string> parameters = channel->getParsedParameters();

	for (char c : channel->getParsedModes()) {
		if (c == '+' || c == '-')
			currentSign = c;
		else {
			if (currentSign == '+') {
				switch (c) {
					case 'i':
						if (!channel->getInviteOnlyState()) {
							channel->setInviteOnlyState(true);
							setModes += '+';
							setModes += 'i';
						}
						break;
					case 'k':
					 	channel->setChannelPassw(parameters[i]);
						setModes += '+';
						setModes += 'k';
						i++;
						break;
					case 'l':
						channel->setUserLimit(std::stoi(parameters[i]));
						setModes += '+';
						setModes += 'l';
						i++;
						break;
					case 'o':
						//do sth
						break;
					case 't':
						if (!channel->getTopicOperatorsOnlyState()) {
							channel->setTopicOperatorsOnlyState(true);
							setModes += '+';
							setModes += 't';
						}
						break;
				}		
			}
			else if (currentSign == '-') {
				switch (c) {
					case 'i':
						if (channel->getInviteOnlyState()) {
							channel->setInviteOnlyState(false);
							setModes += '-';
							setModes += 'i';
						}
						break;
					case 'k':
					 	if (!channel->getChannelPassw().empty()) {
							channel->setChannelPassw("");
							setModes += '-';
							setModes += 'k';
						}
						break;
					case 'l':
						if (channel->getUserLimit() != -1) {
							channel->setUserLimit(-1);
							setModes += '-';
							setModes += 'l';
						}
						break;
					case 'o':
						//do sth
						break;
					case 't':
						if (channel->getTopicOperatorsOnlyState()) {
							channel->setTopicOperatorsOnlyState(false);
							setModes += '-';
							setModes += 't';
						}
						break;
				}		
			}
		}
	}
}

//iklot

/*Currently covers /mode functionality without mode input, all_of function checks if there is only whitespace
  remaining in the message string. ChannelByChannelName function retrieves channel object with help of channel
  name, as it is required to call the getMode function.*/
void Server::handleMode(Client& client, const std::string& channelName, const std::string& message)
{
	std::string firstResponse;
	std::string	secondResponse;
	Channel* channel = getChannelByChannelName(channelName);
	if ((std::all_of(message.begin(), message.end(), [](unsigned char ch) { return std::isspace(ch); })) || message.empty()) {
		if (channelExists(channelName)) {
			if (userIsMemberOfChannel(client, channelName)) {
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
	else {
		if (checkForValidModes(message, client, channel)) {
				executeModes(client, channel);
			std::cout << "MODES VALID" << std::endl;
		}
		else {
			std::cout << "NOT VALID MODE" << std::endl;
		}
	}
}


// void	Server::handleTopic(Client &client, const std::string& channelName, const std::string& topic) {
// 	if (channelExists(channelName)) {
// 		if (userIsMemberOfChannel(client, channelName)) {

// 		}
// 	}
// }
