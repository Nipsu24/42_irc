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

#include <string>
#include <vector>
#include "Client.hpp"

class Client;

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
	void						setKick(Client* client, std::string kickedNick);
	void						printUsers() const;
	std::vector<Client*>&		getUsers();
	void						executeMode();
	void						addClient(Client* client);

private:
	std::string					_channelName;
	std::string					_channelPassw; //empty if no passw for channel
	std::string					_parsedChannelPassw; //to be filled with characters found after /mode +k
	std::vector<std::string>	_modes; // no default set at channel creation
	std::vector<Client*>		_userList;
	std::vector<Client*>		_chOperatorList;
	std::string					_topic; //empty if not set
	int							_userLimit; // -1 if not set
	int							_parsedUserLimit; // to be filled with number found after /mode +l
	bool						_topicOperatorsOnly; // false by default, needs +t mode
	bool						_inviteOnlyEnabled; // false by default, needs +i mode
	std::vector<Client*>		_removeOperatorsList; // needs to be populated by parsing, set empty if not applicable
	std::vector<Client*>		_addOperatorsList; //  needs to be populated by parsing, set empty if not applicable
};

// #endif