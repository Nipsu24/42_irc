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

#pragma once
// #ifndef CHANNEL_HPP
// #define CHANNEL_HPP

#include <string>
#include <vector>
#include "Client.hpp"

class Channel {
public:
	// Constructors
	Channel();
	Channel(std::string name);
	Channel(const Channel& other);
	Channel& operator=(const Channel& other);
	~Channel();

	//Methods
	std::string					getChannelName() const;
	std::string					getChannelPassw() const;
	void						printMode() const;
	void						setMode(const std::vector<std::string>& newModes);
	void						printTopic() const;
	void						setTopic(const std::string& topic);
	// void						printUsers() const;
	std::vector<Client*>&		getUsers();
	// void						setUsers(Client user);
	// std::vector<Client*>			getChOperators() const;
	// void						setChOperators(Client ChOperator);
	void						setUserLimit(int limit);
	void						executeMode();
	void						addClient(Client* client);

private:
	std::string					_channelName;
	std::string					_channelPassw; //empty if no passw for channel
	std::vector<std::string>	_modes; // no default set at channel creation
	std::vector<Client*>		_userList;
	// std::vector<Client*>			_chOperatorList;
	std::string					_topic; //empty if not set
	int							_userLimit; // -1 if not set
	bool						_topicOperatorsOnly; // false by default, needs +t mode
	bool						_inviteOnlyEnabled; // false by default, needs +i mode
};

// #endif