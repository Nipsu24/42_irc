

#include "Server.hpp"
#include "Channel.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>


void Server::handleInvite(Client &client, std::string message)
{
	std::cout << "try to invite" << std::endl;
	std::istringstream iss(message);
	std::string command;
	std::string nick;
	std::string channelName;
	iss >> command;
	iss >> nick;
	iss >> channelName;
	//check if channelname is correct
	if (!checkIfChannelExists(channelName))
	{
		std::cout << "channelexists does not exist" << std::endl;
		std::string response = ":localhost 403 " + client.getNick() + " " + channelName + " :No such channel";
		MessageServerToClient(client, response);
		return;
	}
	//checks if nickname is in use

	std::cout << "try to invite2" << std::endl;
	Client* clientToInvite = getClientByNickname(nick);
	if (clientToInvite == nullptr)
	{
		std::cout << "client exists noot" << std::endl;
		// Send error response to the client
		std::string response = ":localhost 401 " + client.getNick() + " " + channelName + " :No such nick/channel";
		MessageServerToClient(client, response);
		return; // Exit the function after handling the error
	}
	
	
	//checks if client is in channel and is client operator
	try{
		getChannelByChannelName(channelName)->setInvite(&client, channelName, nick);
	}
	catch (const Channel::ClientNotOperatorException &e) {
		std::string response = ":localhost 482 " + client.getNick() + " " + channelName + " :You're not a channel operator";
		MessageServerToClient(client, response);
	}
	catch (const Channel::ClientNotInChannelException &e) {
		std::string response = ":localhost 441 " + client.getNick() + " " + nick = " " + channelName + " :They aren't on that channel";
		MessageServerToClient(client, response);
	}
	catch (const Channel::ClientAlreadyInChannelException &e) {
		std::string response = ":localhost 443 " + client.getNick() + " " + nick = " " + channelName + " :is already on channel";
	}
	std::cout << "everything ok, lets invite" << std::endl;
	std::string response = "INVITE " + nick + " " + channelName;
	MessageServerToClient(client, response);
	
}	


//:iridium.libera.chat 341 timo_ timo___ #testii2

//:platinum.libera.chat 443 timo___ timo_ #testii :is already on channel

//:platinum.libera.chat 341 timo___ timo____ #testii :Inviting timo____ to channel #testii

//:platinum.libera.chat 401 timo___ #testii :No such nick/channel

//:platinum.libera.chat 403 timo___ #testii :No such channel

//:platinum.libera.chat 482 timo___ #testii :You're not a channel operator

//:platinum.libera.chat 441 timo___ timo____ #testii :They aren't on that channel

//:platinum.libera.chat 442 timo___ #toka :You're not on that channel

//:timo___!~timo@mobile-access-b04820-247.dhcp.inet.fi INVITE timo____ :#testii  ->to invited client