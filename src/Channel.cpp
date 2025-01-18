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

#include <iostream>
#include <algorithm>
#include <sstream>
#include "Channel.hpp"
#include <chrono>
#include <ctime>

Channel::Channel() {}

Channel::Channel(std::string name)
	: _channelName(name), _channelPassw(""), _userLimit(-1), _topicOperatorsOnly(false), _inviteOnlyEnabled(false) {}

Channel::Channel(const Channel &other)
{
	this->_channelName = other._channelName;
}

Channel &Channel::operator=(const Channel &other)
{
	if (this != &other)
	{ // Self-assignment check
		this->_channelName = other._channelName;
	}
	return *this;
}

Channel::~Channel() {}

std::vector<Client *> &Channel::getUsers()
{
	return (_userList);
}

/* function getUserNickname still to be set up in Client Class*/
void Channel::printUsers() const
{
	for (Client *memberOfChannel : _userList)
	{
		std::cout << "[" << memberOfChannel->getNick() << std::endl;
	}
}

/*Returns the channel name of respective channel class*/
std::string Channel::getChannelName() const { return _channelName; }

/*Returns empty string in case no password set, otherwise returns channel password.*/
std::string Channel::getChannelPassw() const
{
	if (_channelPassw.empty())
	{
		std::cout << "No password set for #" << _channelName << std::endl;
		return ("");
	}
	return _channelPassw;
}

void	Channel::setChannelPassw(const std::string& password) {
	_channelPassw = password;
}

/*Returns a copy of a string with the current active modes of a channel. Inserts + sign
  at beginning of string. If no mode active, only returns string with + sign in it.*/
std::string	Channel::getMode() const
{
	std::string	activeModes;
	
	if (_inviteOnlyEnabled)
		activeModes += 'i';
	if (!_channelPassw.empty())
		activeModes += 'k';
	if (_userLimit != -1)
		activeModes += 'l';
	if (_topicOperatorsOnly)
		activeModes += 't';
	activeModes.insert(0, 1, '+');
	return (activeModes);
}

/*Compares elements of _modes vector with passed newModes vector and adds the elements from
  newModes vector to _modes vector in case they are not yet present or removes elements in
  case they have '-' prefix. This is done with erase method which requires index which in turn
  is retrieved with help of find_if and lambda function. In case modes.end is returned this means
  no match has been found. Sorts vector alphabethically. Prints at the end the respective modes
  set to the channel.*/
void Channel::setMode(const std::vector<std::string> &newModes)
{
	std::string result;
	for (std::string toAddMode : newModes)
	{
		bool modeAddExists = false;
		bool modeRemoveExists = false;
		for (const std::string &activeMode : _modes)
		{
			if (toAddMode[0] == '+')
			{
				if (activeMode == toAddMode)
				{
					modeAddExists = true;
					break;
				}
			}
			else if (toAddMode[0] == '-')
			{
				if (activeMode[1] == toAddMode[1])
				{
					modeRemoveExists = true;
					break;
				}
			}
		}
		if (!modeAddExists && !modeRemoveExists)
		{
			_modes.push_back(toAddMode);
			result += toAddMode; // could still be improved -> grouping + and - modes if appearing after each other
		}
		if (!modeAddExists && modeRemoveExists)
		{
			auto it = std::find_if(
				_modes.begin(), _modes.end(),
				[&](const std::string &mode)
				{ return mode[1] == toAddMode[1]; });
			if (it != _modes.end())
			{
				_modes.erase(it);
				result += toAddMode;
			}
		}
	}
	std::sort(_modes.begin(), _modes.end());
	std::cout << "mode/#" << _channelName << " [" << result << "] by INSERT USERNAME" << std::endl;
	executeMode();
}

/*Function for executing set modes of a channel, used in setMode function. First, sets all
  relevant values to initial values and popuates afterwards if applicable (if i, t, k, l are present).
  Then checks if any operator should be added/removed to the _chOperatorList (handles +/- o mode).*/
void Channel::executeMode()
{
	_inviteOnlyEnabled = false;
	_topicOperatorsOnly = false;
	_channelPassw = "";
	_userLimit = -1;

	bool toAddOperatorFound = false;
	for (const std::string &activeMode : _modes)
	{
		if (activeMode[1] == 'i')
			_inviteOnlyEnabled = true;
		if (activeMode[1] == 't')
			_topicOperatorsOnly = true;
		if (activeMode[1] == 'k')
			_channelPassw = _parsedChannelPassw;
		if (activeMode[1] == 'l')
			_parsedUserLimit = _userLimit;
	}
	if (!_addOperatorsList.empty())
	{
		for (Client *toAddOperator : _addOperatorsList)
		{
			toAddOperatorFound = false;
			for (Client *currentOperator : _chOperatorList)
			{
				if (toAddOperator->getNick() == currentOperator->getNick())
					toAddOperatorFound = true;
			}
			if (toAddOperatorFound == false)
				_chOperatorList.push_back(toAddOperator);
		}
	}
	if (!_removeOperatorsList.empty())
	{
		for (Client *toRemoveOperator : _removeOperatorsList)
		{
			auto it = std::find_if(_chOperatorList.begin(), _chOperatorList.end(), [&](Client *user)
								   { return user == toRemoveOperator; });
			if (it != _chOperatorList.end())
			{
				_chOperatorList.erase(it);
			}
		}
	}
}

/*Prints topic of the channel in case it's set, otherwise prints respective hint.*/
void Channel::printTopic() const
{
	if (_topic.empty())
	{
		std::cout << "No topic set for #" << _channelName << std::endl;
		return;
	}
	std::cout << "Topic for #" << _channelName << ": " << _topic << std::endl;
}

/*Sets topic of the channel and prints respective message*/
void Channel::setTopic(Client *client, const std::string& channelName, std::string& rest)
{
	(void) client;
	(void) channelName;
	std::cout << rest << std::endl;
	if (_topicOperatorsOnly == false)
	{ // code to be added in if statement: "|| client has operator status"
		_topic = rest;
		std::cout << "INSERT USERNAME " << "changed the topic of #" << _channelName << " to: " << _topic << std::endl;
	}
}


void Channel::setKick(Client *client, const std::string& channelName, std::string& rest)
{
	std::cout << client->getNick() + " kicked out " + rest + " from " + channelName << std::endl; 
}

void Channel::setInvite(Client *client, const std::string& channelName, std::string& rest)
{
	std::istringstream iss(rest);
	std::string nick;
    iss >> nick;
	std::cout << client->getNick() + " invited " + nick + " from " + channelName << std::endl;
}


/*Adds new client to the channel by adding it to the vector array of clients.
  Used in joinChannel function in server.cpp*/
void	Channel::addClient(Client *client)
{
	_userList.push_back(client);
}

std::string	Channel::getTimestamp() {return (_timestampOfCreation);}

/*Calculates current time since epoch in seconds and sets respective value to member variable by
  converting value of timestamp into string.*/
void	Channel::setTimestamp() {
	auto now = std::chrono::system_clock::now();
	auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
	_timestampOfCreation = std::to_string(timestamp);
}

std::string	Channel::getParsedModes() { return (_parsedModes); }

std::vector<std::string>	Channel::getParsedParameters() { return (_parsedParameters); }

void	Channel::setParsedModes(std::string modes) { _parsedModes = modes; }

void	Channel::setParsedParameters(std::vector<std::string> parameters) {
	_parsedParameters = parameters; }

bool	Channel::getInviteOnlyState() { return (_inviteOnlyEnabled); }

void	Channel::setInviteOnlyState(bool status) { _inviteOnlyEnabled = status; }

void	Channel::clearParsedParameters() { _parsedParameters.clear(); }

int		Channel::getUserLimit() { return (_userLimit); }

void	Channel::setUserLimit(int limit) {
	_userLimit = limit;
}

bool	Channel::getTopicOperatorsOnlyState() { return (_topicOperatorsOnly); }

void	Channel::setTopicOperatorsOnlyState(bool status) { _topicOperatorsOnly= status; }

// int	main(void)
// {
// 	Channel	channel("firstCh");
// 	std::vector<std::string>	newModes = {"+i", "+c"};
// 	std::vector<std::string>	newModes2 = {"+i", "+n"};
// 	std::vector<std::string>	newModes3 = {"+i", "-n"};
// 	std::cout << channel.getChannelName() << std::endl;
// 	std::cout << channel.getChannelPassw() << std::endl;
// 	channel.printMode();
// 	channel.setMode(newModes);
// 	channel.printMode();
// 	channel.setMode(newModes2);
// 	channel.printMode();
// 	channel.setMode(newModes3);
// 	channel.printMode();
// 	channel.printTopic();
// 	channel.setTopic("Potatoes");
// 	channel.printTopic();
// 	return (0);
// }
