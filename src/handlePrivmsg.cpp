#include "Server.hpp"
#include "Channel.hpp"
#include <iostream>

void Server::handlePrivmsg(Client &client, const std::string channelName, const std::string &message)
{
    std::string response = ":" + client.getNick() + " PRIVMSG " + channelName + message + "\r\n";

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
}
