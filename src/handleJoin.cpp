
#include "Server.hpp"
#include "Channel.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>



void Server::handleJoin(Client &client, std::string channelName)
{
	if (channelName == "")
	{
		MessageServerToClient(client, "\r\n");
		return;
	}
	else
	{
		bool channelExists = false;
		for (Channel *availableChannels : _channels)
		{
			if (channelName == availableChannels->getChannelName())
			{
				availableChannels->addClient(&client);
				channelExists = true;
				break;
			} 
		}
		if (channelExists == false)
		{
			Channel *newChannel = new Channel(channelName);
			newChannel->addClient(&client);
			_channels.push_back(newChannel);
			newChannel->setChOperator(&client);
		}
		std::cout << "Received JOIN from client" << client.getFd() << ": " << client.getNick() << std::endl;
		std::string response = ":" + client.getNick() + " JOIN " + channelName + "\r\n";
		MessageServerToClient(client, response);
	}
}

