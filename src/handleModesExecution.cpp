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
#include <iostream>
#include "Client.hpp"

/*Helper function of executeModes for modifying modes string so that e.g.
  +i+t-r-l is changed to +it-rl.  Tracks signs and stores -/+ characters
  in groups once signs change. Last if statement appends the last group
  to the string. */
std::string	Server::compressModes(const std::string& setModes)
{
	std::string result;
	std::string currentGroup;
	char currentSign = '\0';
	
	for (char c : setModes) {
		if (c == '+' || c == '-') {
			if (!currentGroup.empty()) {
				result += currentSign + currentGroup;
				currentGroup.clear();
			}
			currentSign = c;
		} 
		else
			currentGroup += c;
	}
	if (!currentGroup.empty())
		result += currentSign + currentGroup;
	return (result);
}

/*Function for setting/unsetting respective modes in the channel. The passed arguments are already
  parsed and expected to be valid. Switch statements are used for the + and - cases and respective
  channel functions are called to set/unset the mode values. Strings 'setModes' and 'setParameters'
  are filled in order to return respective message to client which modes were set for the channel.*/
void	Server::executeModes(Client& client, Channel* channel)
{
	std::string					response;
	char						currentSign;
	int							i = 0;
	std::string					setModes;
	std::string					setParameters;
	Client*						toRemoveOperator;
	Client*						toAddOperator;
	std::vector<std::string>	parameters = channel->getParsedParameters();

	for (char c : channel->getParsedModes()) {
		if (c == '+' || c == '-')
			currentSign = c;
		else {
			if (currentSign == '+') {
				switch (c) {
					case 'i':
						if (!channel->getInviteOnlyState()) {
							channel->setInviteOnlyState(true);
							setModes += "+i";
						}
						break;
					case 'k':
					 	channel->setChannelPassw(parameters[i]);
						setModes += "+k";
						if (setParameters.empty())
							setParameters += parameters[i];
						else
							setParameters += " " + parameters[i];
						i++;
						break;
					case 'l':
						std::cout << "PARAMETER: " << parameters[i] << std::endl;
						channel->setUserLimit(std::stoi(parameters[i]));
						setModes += "+l";
						if (setParameters.empty())
							setParameters += parameters[i];
						else
							setParameters += " " + parameters[i];
						i++;
						break;
					case 'o':
						toAddOperator = getClientByNickname(parameters[i]);
						channel->setChOperator(toAddOperator);
						std::cout << parameters[i] << " set as operator" << std::endl;
						setModes += "+o";
						if (setParameters.empty())
							setParameters += parameters[i];
						else
							setParameters += " " + parameters[i];
						i++;
						break;
					case 't':
						if (!channel->getTopicOperatorsOnlyState()) {
							channel->setTopicOperatorsOnlyState(true);
							setModes += "+t";
						}
						break;
				}		
			}
			else if (currentSign == '-') {
				switch (c) {
					case 'i':
						if (channel->getInviteOnlyState()) {
							channel->setInviteOnlyState(false);
							setModes += "-i";
						}
						break;
					case 'k':
					 	if (!channel->getChannelPassw().empty()) {
							channel->setChannelPassw("");
							setModes += "-k";
						}
						break;
					case 'l':
						if (channel->getUserLimit() != -1) {
							channel->setUserLimit(-1);
							setModes += "-l";
						}
						break;
					case 'o':
						toRemoveOperator = getClientByNickname(parameters[i]);
						channel->unsetChOperator(toRemoveOperator);
						setModes += "-o";
						if (setParameters.empty())
							setParameters += parameters[i];
						else
							setParameters += " " + parameters[i];
						i++;
						break;
					case 't':
						if (channel->getTopicOperatorsOnlyState()) {
							channel->setTopicOperatorsOnlyState(false);
							setModes += "-t";
						}
						break;
				}		
			}
		}
	}
	setModes = compressModes(setModes);
	response = ":" + client.getNick() + " " + "Mode" + " " + channel->getChannelName() + " " + setModes;
	if (!setParameters.empty())
	response += " " + setParameters;
	std::cout << "Message to client: " << response << std::endl;
	MessageServerToClient(client, response);
}
