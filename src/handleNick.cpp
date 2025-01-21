#include "Server.hpp"
#include "iostream"

void Server::handleNick(Client &client, std::string nick)
{
	std::string channelName = "";
	std::string oldNick = client.getNick();
	client.setNick(nick);
	if (client.getState() == REGISTERED)
	{ 
		std::string response = ":" + oldNick + "!~" + client.getUsername() + "@localhost NICK :" + client.getNick();
		MessageServerToClient(client, response);
		/*if (client.getLoggedIn() != nullptr) {
			std::string channelName = client.getLoggedIn()->getChannelName();
			for (Channel *channel : _channels)
			{
				if (channel->getChannelName() == channelName)
				{
					for (Client *_client : channel->getUsers())
					{
						if (_client != &client)
							MessageServerToClient(*_client, response);
					}
				}
			}
		} else {
			std::cerr << "Client is not logged into any channel.\n";
		}*/
	}
}