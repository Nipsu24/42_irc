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

template <typename T> 
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) 
{ 
    // os << "["; 
    for (int i = 0; i < v.size(); ++i) { 
        os << v[i]; 
        if (i != v.size() - 1) 
            os << ", "; 
    } 
    // os << "]\n"; 
    return os; 
} 


Channel::Channel() {}

Channel::Channel(std::string name) 
		: _channelName(name), _userLimit(-1), _channelPassw("") {}

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

std::string	Channel::getChannelName() const {return _channelName;}

/*Returns empty string in case no password set, otherwise returns channel password.*/
std::string	Channel::getChannelPassw() const {
	if (_channelPassw.empty()) {
		std::cout << "No password set for #" << _channelName << std::endl;
		return ("");
	}
	return _channelPassw;
}

/*Checks if there are any modes set for the channel. If this is the case,
  prints respective modes.*/
void	Channel::printMode() const {
	if (_modes.empty())
		std::cout << "No mode set for " << _channelName << std::endl;
	else {
		std::string result;
		for (int i = 0; i < _modes.size(); ++i)
			result += _modes[i];
		std::cout << "mode/#" << _channelName << " [" << result << "]" << std::endl;
	}
}

/*Compares elements of _modes vector with passed newModes vector and adds the elements from 
  newModes vector to _modes vector in case they are not yet present. Sorts vector alphabethically.
  Prints at the end the respective modes set to the channel.*/
void	Channel::setMode(const std::vector<std::string> newModes) {
	for (std::string toAddMode : newModes) {
		bool modeExists = false;
		for (const std::string& activeMode : _modes) {
			if (activeMode == toAddMode){
				modeExists = true;
				break;
			}
		}
		if (!modeExists)
			_modes.push_back(toAddMode);
	}
	std::sort(_modes.begin(), _modes.end());
	std::cout << "mode/#" << _channelName << " [" << _modes << "] by INSERT USERNAME" << std::endl;
}

/*Prints topic of the channel in case it's set, otherwise prints respective hint.*/
void	Channel::printTopic() const {
	if (_topic.empty()) {
		std::cout << "No topic set for #" << _channelName << std::endl;
		return ;
	}
	std::cout << "Topic for " << _channelName << ": " << _topic << std::endl;
}

/*Sets topic of the channel and prints respective message*/
void	Channel::setTopic(const std::string& topic) {
	_topic = topic;
	std::cout << "INSERT USERNAME " << "changed the topic of #" << _channelName << " to: " << _topic << std::endl;
}

/*Sets a limit to the number of users in a group and adds '+l' to _modes.*/
void	Channel::setUserLimit(int limit){
	_userLimit = limit;
	std::cout << "mode/#" << _channelName << " [+l" << limit << "]" << " by INSERT USERNAME" << std::endl;
	_modes.push_back("+l");
	std::sort(_modes.begin(), _modes.end());
}

// void	Channel::unsetMode(const std::string& mode) {
// 	for (std::string activeMode : _modes){
// 		if (activeMode[1] == mode[1]) {
// 		//delete mode from vector here
// 		_modes.erase(activeMode);
// 		std::cout << "mode/#" << _channelName << " [" << mode << "] by INSERT USERNAME" << std::endl;
// 		if (mode == "-l")
// 			_userLimit = -1;
// 		if (mode == "-k")
// 			_channelPassw = "";
// 		break ;
// 		}
// 	}
// }


//Further functions TBD//

int	main(void)
{
	Channel	channel("firstCh");
	std::vector<std::string>	newModes = {"+i", "+c"};
	std::vector<std::string>	newModes2 = {"+i", "+n"};
	std::cout << channel.getChannelName() << std::endl;
	std::cout << channel.getChannelPassw() << std::endl;
	channel.printMode();
	channel.setMode(newModes);
	channel.printMode();
	channel.setMode(newModes2);
	channel.printMode();
	channel.printTopic();
	channel.setTopic("Potatoes");
	channel.printTopic();
	return (0);
}