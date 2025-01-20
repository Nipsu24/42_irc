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

/*First checks if any invalid characters are occuring in the modes string and returns a respective message if this
  is the case. Otherwise counts the occurence of k, l and o within the modes string, only counting for the minus part
  the o occurence as for the other modes no parameters are expected for the minus part. This is how the amount of
  needed parameters can be determined in the rest of the message. Then iterates further with iss using the parameter
  amount and stores the remaining content of the message in the parameter vector. Modes and parameters are stored in
  respective data fields of the channel class object for later usage within the exectueModes function.*/
bool	Server::checkForValidModes(const std::string& message, Client& client, Channel* channel)
{
	std::string 		modes;
	std::string			response;
	char				currentSign = '\0';
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
		for (int i = 0; i < neededParameterCount; i++) {
			iss >> parameter[i];
			std::cout << "PARAMETERS " << parameter[i] << std::endl;

		}
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
		channel->clearParsedParameters();
		channel->setParsedParameters(parameter);
		channel->setParsedModes(modes);
		return (true);
	}
	channel->clearParsedParameters();
	channel->setParsedModes(modes);
	return (true);
}

/*Main function for handling parsing and execution of /mode and /mode +/- arguments.
  First checks if message string is empty or only consists of white space characters. If this is the case, it is expected that only the
  /mode command without arguments is passed by client. It is then checked if the channel exists and if the user is member of the channel
  in order to reply with the respective modes set for the channel and the channel creation timestamp (as done in libera chat). If the channel
  does not exist, a respective message is sent to user.
  If the message string is not empty, it is first checked if the channelName is actually containing a '#' at 0 index. If this is not the case,
  the parsed channel name is actually the user name and the /mode command is the initial command sent automatically by client when logging in
  to the server. This case is ignored for the time being. Otherwise, if the channelName is indeed starting with '#' it is furthermore checked
  if the modes given to the command are valid and - if this is the case - the information is passed to the executeModes function.*/
void Server::handleMode(Client& client, const std::string& channelName, const std::string& message)
{
	std::string firstResponse;
	std::string	secondResponse;
	Channel* channel;
	if ((std::all_of(message.begin(), message.end(), [](unsigned char ch) { return std::isspace(ch); })) || message.empty()) {
		if (channelExists(channelName)) {
			if (userIsMemberOfChannel(client, channelName)) {
				channel = getChannelByChannelName(channelName);
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
		if (channelName[0] != '#') //checks if channelName is actually nickname of user (when initially connect and not part of any channel yet)
			return ;
		channel = getChannelByChannelName(channelName);
		if (checkForValidModes(message, client, channel))
				executeModes(client, channel);
	}
}
