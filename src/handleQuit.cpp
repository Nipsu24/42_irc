#include "Server.hpp"
#include "Channel.hpp"
#include <iostream>

void Server::handleQuit(Client &client, std::string message)
{
    std::cout << client.getNick() << " Quitted!!. Message = " << message << std::endl;
    for (Channel *channel : _channels)
    {
        for (Client *_client : channel->getUsers())
        {
            if (_client == &client)
            {
                channel->removeClient(_client);
            }
        }
    }
}