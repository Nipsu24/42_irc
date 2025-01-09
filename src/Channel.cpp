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
#include "Channel.hpp"

Channel::Channel() {}

Channel::Channel(std::string name) 
		: _channelName(name) {}

Channel::Channel(const Channel& other) {
    this->_channelName = other._channelName;
}

Channel& Channel::operator=(const Channel& other) {
    if (this != &other) {  // Self-assignment check
        this->_channelName = other._channelName;
    }
    return *this;
}

Channel::~Channel() {}

std::string	Channel::getChannelName() const{return _channelName;}

std::string	Channel::getChannelPassw() const{return _channelPassw;}

/*Checks if there are any modes set for the channel. If this is the case,
  prints respective modes.*/
void	Channel::printMode() const {
	if (_modes.empty())
		std::cout << "No mode set for " << _channelName << std::endl;
	else {
		std::string result;
		for (int i = 0; i < _modes.size(); ++i)
			result += _modes[i];
		std::cout << "mode/" << _channelName << " [" << result << "]" << std::endl;
	}
}

/*Checks first if mode to be set already exists for the channel. If so, returns respective
  message and returns. Otherwise adds mode to vector array and sorts array alphabethically.
  Prints at the end the mode set to the channel.*/
void Channel::setMode(std::string& mode) {
	for (std::string activeMode : _modes){
		if (activeMode == mode){
			std::cout << "Mode: <" << mode << "> already set." << std::endl;
			return ;
		}
	}
	_modes.push_back(mode);
	std::sort(_modes.begin(), _modes.end());
	std::cout << "mode/" << _channelName << " [" << mode << "]" << std::endl;
}

//Further functions TBD//
