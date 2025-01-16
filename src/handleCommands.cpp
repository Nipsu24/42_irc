#include "Server.hpp"
#include "Channel.hpp"
#include <iostream>
#include <string>



void Server::handlePingPong(Client &client)
{
	std::string response = ":localhost PONG localhost :localhost";
	MessageServerToClient(client, response);
	int i = 0;
	for (Channel *channel : _channels)
	{
		std::cout << "channel " << i <<  ": " << channel->getChannelName() << std::endl;
		i++;
	}

}



/*
 * Handle the CAP LS message
 */
void Server::handleCAPLS(Client &client, std::vector<std::string> tokens, int index)
{
	std::string response;
	if (tokens[index + 1] == "LS")
	{
		client.setState(REGISTERING);
		std::cout << "Received CAP LS from client" << client.getFd() << ": " << client.getNick() << std::endl; // Debugging
		response = "CAP * LS";
		MessageServerToClient(client, response);
		
		//client.setNick(tokens[index + 3]);
		//client.setUsername(tokens[index + 5]);
		//
	}
	else if (tokens[index + 1] == "END")
	{

		client.setState(REGISTERED);
		response = ":001 " + client.getNick() + ":Welcome to the Internet Relay Network " + client.getNick();
		MessageServerToClient(client, response);
	}
	else
	{
		std::cerr << "Invalid CAP message" << std::endl;
	}
}

/*
 * Handle the USER message
 */
void Server::handleUserName(Client &client, std::vector<std::string> tokens, int index)
{
	
	//bool found = false;
	/*	for (Client *client : _clients)
	{
		
		if (client->getNick() == tokens[index + 3] + "_")
		{
			client->setNick("Guest" + std::to_string(client->getFd()) + "_");
			std::string response = "localhost NOTICE tim :This nickname is registered. Please choose a different nickname.";
			MessageServerToClient(*client, response);
			found = true;
			break;
		}
	}
	if (!found)*/

	client.setNick(tokens[index + 1] + "_");
	client.setUsername(tokens[index + 1]);
	std::cout << "Received USER from client" << client.getFd() << ": " << client.getNick() << std::endl;
	std::string response = "001 " + client.getNick() + " :Welcome to the Internet Relay Network " + client.getNick();
	std::cout << "nick: " + client.getNick() + ", username: " + client.getUsername() << std::endl;
	MessageServerToClient(client, response);
}


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
				client.setLoggedIn(availableChannels);
				channelExists = true;
				break;
			}
		}
		if (channelExists == false)
		{
			Channel *newChannel = new Channel(channelName);
			newChannel->addClient(&client);
			_channels.push_back(newChannel);
			client.setLoggedIn(newChannel);
		}
		std::cout << "Received JOIN from client" << client.getFd() << ": " << client.getNick() << std::endl;
		std::string response = ":" + client.getNick() + " JOIN " + channelName + "\r\n";
		MessageServerToClient(client, response);
	}
}
/*void Server::handleKick(Client &client, std::string message)
{
	(void)client;
	std::string kickedNick = tokens[index + 2];
	std::cout << "KICKED " + kickedNick + "\n";
	client.getLoggedIn()->setKick(&client, kickedNick);
}*/
