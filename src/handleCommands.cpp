#include "Server.hpp"
#include "Channel.hpp"
#include <iostream>


/*
 * Handle the CAP LS message
 */
void Server::HandleCAPLS(Client &client, std::vector<std::string> tokens, int index)
{
    std::string response;
    if (tokens[index + 1] == "LS")
    {
        client.setState(REGISTERING);
        std::cout << "Received CAP LS from client" << client.getFd() << ": " << client.getNick() << std::endl; // Debugging
        response = "CAP * LS";
        MessageServerToClient(client, response);
    }
    else if (tokens[index + 1] == "END")
    {
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
    client.setNick(tokens[index + 1] + "_");
    client.setUsername(tokens[index + 1]);
    std::cout << "Received USER from client" << client.getFd() << ": " << client.getNick() << std::endl;
    std::string response = "001 " + client.getNick() + " :Welcome to the Internet Relay Network " + client.getNick();
    MessageServerToClient(client, response);
}


void Server::handleNick(Client &client, std::vector<std::string> tokens, int index)
{
    client.setNick(tokens[index + 1]);
}


void Server::handleJoin(Client &client, std::vector<std::string> tokens, int index)
{
	std::string& channelName = tokens[index + 1];
    if (channelName == "")
	{
			MessageServerToClient(client, "\r\n");
			return;
	}
	else
	{
		bool	channelExists = false;
		for (Channel* availableChannels : _channels) {
			if (channelName == availableChannels->getChannelName()) {
				availableChannels->addClient(&client);
				client.setLoggedin(availableChannels);
				channelExists = true;
				break;
			}
		}
		if (channelExists == false) {
			Channel* newChannel = new Channel(channelName);
			newChannel->addClient(&client);
			_channels.push_back(newChannel);
			client.setLoggedin(newChannel);
		}
		std::cout << "Received JOIN from client" << client.getFd() << ": " << client.getNick() << std::endl;
		std::string response = ":" + client.getNick() + " JOIN " + channelName + "\r\n";
		MessageServerToClient(client, response);
    }
}

void Server::handlePrivmsg(Client &client, std::vector<std::string> tokens, int index)
{
    std::string response = ":" + client.getNick() + " PRIVMSG " + tokens[index + 1] + " :" + tokens[index + 2] + "\r\n";
    MessageServerToClient(client, response);
}

void Server::handleKick(Client &client, std::vector<std::string> tokens, int index)
{
	(void) client;
	std::string kickedNick = tokens[index + 2];
	std::cout << "KICKED " + kickedNick + "\n";
	//client.getLoggedin()->setKick(&client, kickedNick);
}