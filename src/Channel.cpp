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

/* ************************************************Constructor Section START*************************************** */
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

/* ************************************************Constructor Section END*************************************** */

/*Returns the channel name of respective channel class*/
std::string Channel::getChannelName() const { return (_channelName); }

/*Returns channelPassw or empty string in case no password set*/
std::string Channel::getChannelPassw() const
{
	if (_channelPassw.empty())
	{
		std::cout << "No password set for #" << _channelName << std::endl;
		return ("");
	}
	return (_channelPassw);
}

bool Channel::isClientOperator(Client* client) {
    
    auto it = std::find(_chOperatorList.begin(), _chOperatorList.end(), client);
    return it != _chOperatorList.end();
}

bool Channel::isClientInChannel(Client* client) {
	auto it = std::find(_userList.begin(), _userList.end(), client);
	return it != _userList.end();
}

/*Function for setting the password of a channel*/
void	Channel::setChannelPassw(const std::string& password) { _channelPassw = password; }

/*Returns a copy of a string with the current active modes of a channel and the respective
  parameters if applicable. Inserts + sign at beginning of string. If no mode active, only
  returns string with + sign in it.*/
std::string	Channel::getMode() const
{
	std::string	activeModes;
	std::string parameters;
	
	if (_inviteOnlyEnabled)
		activeModes += 'i';
	if (!_channelPassw.empty()) {
		activeModes += 'k';
		if (parameters.empty())
			parameters = _channelPassw;
		else
			parameters += " " + _channelPassw;
	}
	if (_userLimit != -1) {
		activeModes += 'l';
		if (parameters.empty())
			parameters = std::to_string(_userLimit);
		else
			parameters += " " + std::to_string(_userLimit);
	}
	if (_topicOperatorsOnly)
		activeModes += 't';
	activeModes.insert(0, 1, '+');
	if (!parameters.empty())
		activeModes += " " + parameters;
	return (activeModes);
}

void Channel::setTopic(Client *client, const std::string& channelName, std::string& rest)
{
	(void)channelName;
	if (!isClientOperator(client))
		throw ClientNotOperatorException();
	std::cout << rest << std::endl;
	if (_topicOperatorsOnly == false)
	{ // code to be added in if statement: "|| client has operator status"
		_topic = rest;
		std::cout << "INSERT USERNAME " << "changed the topic of #" << _channelName << " to: " << _topic << std::endl;
	}
}


void Channel::setKick(Client *client, const std::string& channelName, std::string& nick)
{
	if (!isClientInChannel(client))
		throw ClientNotInChannelException();
	if (!isClientOperator(client))
		throw ClientNotOperatorException();
	auto it = std::find_if(_userList.begin(), _userList.end(), [&](Client* client) {
        return client->getNick() == nick;
    });
    // If the client is found, remove it using removeClient
    if (it != _userList.end()) {
        removeClient(*it); // Call removeClient with the found client pointer
    }
	else {
		throw NickNotExistException();
	}
	std::cout << client->getNick() + " kicked out " + nick + " from " + channelName << std::endl; 
}

void Channel::setInvite(Client *client, const std::string& channelName, std::string& nick)
{
	(void)channelName;
	(void) nick;
	if (!isClientInChannel(client))
		throw ClientNotInChannelException();
	if (!isClientOperator(client))
		throw ClientNotOperatorException();
	if (isClientInChannel(getClientByNickname(nick)))
		throw ClientAlreadyInChannelException();
}



/*Retrieves all client objects from a channel*/
std::vector<Client *> &Channel::getUsers() { return (_userList); }

/*Adds new client to the channel by adding it to the vector array of clients.
  Used in joinChannel function in server.cpp*/
void	Channel::addClient(Client *client)
{
	_userList.push_back(client);
}

void Channel::removeClient(Client *client)
{
	auto it = std::find(_userList.begin(), _userList.end(), client);
	if (it != _userList.end())
	{
		_userList.erase(it);
	}
}

/*Retrieves timestamp of channel creation.*/
std::string	Channel::getTimestamp() {return (_timestampOfCreation);}

/*Calculates current time since epoch in seconds and sets respective value to member variable by
  converting value of timestamp into string.*/
void	Channel::setTimestamp() {
	auto now = std::chrono::system_clock::now();
	auto timestamp = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
	_timestampOfCreation = std::to_string(timestamp);
}

/*Getters and setters for different channel modes and parsing part*/
std::string	Channel::getParsedModes() { return (_parsedModes); }

void	Channel::setParsedModes(std::string modes) { _parsedModes = modes; }

std::vector<std::string>	Channel::getParsedParameters() { return (_parsedParameters); }

void	Channel::setParsedParameters(std::vector<std::string> parameters) { _parsedParameters = parameters; }

void	Channel::clearParsedParameters() { _parsedParameters.clear(); }

bool	Channel::getInviteOnlyState() { return (_inviteOnlyEnabled); }

void	Channel::setInviteOnlyState(bool status) { _inviteOnlyEnabled = status; }

int		Channel::getUserLimit() { return (_userLimit); }

void	Channel::setUserLimit(int limit) { _userLimit = limit; }

bool	Channel::getTopicOperatorsOnlyState() { return (_topicOperatorsOnly); }

void	Channel::setTopicOperatorsOnlyState(bool status) { _topicOperatorsOnly= status; }

std::vector<Client *>&	Channel::getChOperatorList() { return (_chOperatorList); }

/*Check is client is in _chOperatorList and adds it, if this is not the case. Client to be passed
  to push_back function, as class object expected (in contrast to erase function which expects an
  iterator)*/
void	Channel::setChOperator(Client* client) {
	auto it = std::find(_chOperatorList.begin(), _chOperatorList.end(), client);
	if (it == _chOperatorList.end())
		_chOperatorList.push_back(client);
}

/*Checks if client is in _chOperatorList and erases client from list if this is the case.
  'it' needs to be passed to erase function, as an iterator is expected here (in contrast to
  push_back function, which expects a class object)*/
void	Channel::unsetChOperator(Client* client) {
	auto it = std::find(_chOperatorList.begin(), _chOperatorList.end(), client);
	if (it != _chOperatorList.end())
		_chOperatorList.erase(it);
}




const char* Channel::ClientNotOperatorException::what() const noexcept {
    return "Client is not an operator in this channel";
}

const char* Channel::NickNotExistException::what() const noexcept {
    return "Nick does not exist in this channel";
}

const char* Channel::ChannelNotFoundException::what() const noexcept {
    return "Channel not found";
}

const char* Channel::ClientNotInChannelException::what() const noexcept {
    return "Client is not in this channel";
}

const char* Channel::ClientAlreadyInChannelException::what() const noexcept {
    return "Client already in this channel";
}