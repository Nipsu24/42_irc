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

#include "Client.hpp"
#include <vector>
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
		std::string					_channelPassw;
		std::vector<std::string>	_modes;
		std::vector<Client*>		_userList;
		std::vector<Client*>		_chOperatorList;
		std::string					_topic;
		int							_userLimit;
		bool						_topicOperatorsOnly;
		bool						_inviteOnlyEnabled;
		std::string					_timestampOfCreation;
		std::string					_parsedModes;
		std::vector<std::string>	_parsedParameters;
		std::vector<Client*>		_invitationList;
};
