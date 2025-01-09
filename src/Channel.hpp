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

class Channel {
public:
	// Constructors
	Channel();
	Channel(std::string name);
	Channel(const Channel& other);
	Channel& operator=(const Channel& other);
	~Channel();

	//Methods
	std::string						getChannelName() const;
	std::string						getChannelPassw() const;
	void							printMode() const;
	void							setMode(std::string& mode);
	void							printTopic() const;
	void							setTopic(std::string& topic);
	void							printUsers() const;
	std::vector<Client>				getUsers() const;
	void							setUsers(Client user);
	std::vector<Client>				getChOperators() const;
	void							setChOperators(Client ChOperator);

private:
	std::string					_channelName;
	std::string					_channelPassw;
	std::vector<std::string>	_modes;
	std::vector<Client>			_userList;
	std::vector<Client>			_chOperatorList;
	std::string					_topic;
	int							_userLimit;
};