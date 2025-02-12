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
#include <functional>

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
	void						setChannelPassw(const std::string& password);
	std::string					getMode() const;
	void						setTopic(Client *client, const std::string& topic);
	void						setKick(Client *client, const std::string& channelName, std::string& rest);
	void						setInvite(Client *client, const std::string& channelName, std::string& rest);
	std::vector<Client*>&		getUsers();
	void						addClient(Client* client);
	void						removeClient(Client* client);
	std::string					getTimestamp();
	void						setTimestamp();
	std::string					getParsedModes();
	void						setParsedModes(std::string modes);
	std::vector<std::string>	getParsedParameters();
	void						setParsedParameters(std::vector<std::string> parameters);
	void						clearParsedParameters();
	bool						getInviteOnlyState();
	void						setInviteOnlyState(bool status);
	int							getUserLimit();
	void						setUserLimit(int limit);
	bool						getTopicOperatorsOnlyState();
	void						setTopicOperatorsOnlyState(bool status);
	std::vector<Client *>&		getChOperatorList();
	void						setChOperator(Client* client);
	void						unsetChOperator(Client* client);
	bool						isClientOperator(Client* client);
	bool 						isClientInChannel(Client* client);
	Client*						getClientByNickname(const std::string& nickname);
	std::size_t					getNumberOfUsersInCh() const;
	bool						checkForModeRestrictions(Client &client, std::string password,
										std::function<void(Client&, const std::string&)> messageFunc);
	bool						isChannelOperator(Client* client);
	bool						isOnInvitationList(Client* client); 
	void						addToInvitationList(Client* client);
	std::vector<Client *>&		getInvitationList();
	

		class ClientNotOperatorException : public std::exception
	{
			public:
					const char  *what() const noexcept override;
	};
	class NickNotExistException : public std::exception
	{
			public:
					const char  *what() const noexcept override;
	};
	class ChannelNotFoundException : public std::exception
	{
			public:
					const char  *what() const noexcept override;
	};
	class ClientNotInChannelException : public std::exception
	{
			public:
					const char  *what() const noexcept override;
	};
	class ClientAlreadyInChannelException : public std::exception
	{
			public:
					const char  *what() const noexcept override;
	};

private:
	std::string					_channelName;
	std::string					_channelPassw; //empty if no passw for channel
	std::vector<std::string>	_modes; // no default set at channel creation
	std::vector<Client*>		_userList;
	std::vector<Client*>		_chOperatorList;
	std::string					_topic; //empty if not set
	int							_userLimit; // -1 if not set
	bool						_topicOperatorsOnly; // false by default, needs +t mode
	bool						_inviteOnlyEnabled; // false by default, needs +i mode
	std::string					_timestampOfCreation;
	std::string					_parsedModes;
	std::vector<std::string>	_parsedParameters;
	std::vector<Client*>		_invitationList;
};

//user limit;
//invite Only;
//channel password;

//operator? to set modes

//for lkit:
//>> @time=2025-01-21T08:37:56.847Z :osmium.libera.chat 482 NoOperator #new_channel24 :You're not a channel operator

//assign channelOperator and self deassign operator role:
// >> @time=2025-01-21T08:41:51.281Z :UserOperator1!~mariusmei@dsl-hkibng21-54f864-67.dhcp.inet.fi MODE #new_channel24 +o NoOperator
// << MODE #new_channel24 -o NoOperator
// >> @time=2025-01-21T08:41:56.660Z :NoOperator!~mariusmei@dsl-hkibng21-54f864-67.dhcp.inet.fi MODE #new_channel24 -o NoOperator