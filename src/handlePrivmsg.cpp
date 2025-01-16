#include "Server.hpp"
#include "Channel.hpp"
#include <iostream>
/*std::string parsePrivMessage(std::string message, std::string token)
{
    std::string parsedMessage;
    std::string::size_type pos = message.find(token);
    if (pos != std::string::npos)
    {
        parsedMessage = message.substr(pos);
    }
    return parsedMessage;
}*/


void Server::handlePrivmsg(Client &client, const std::string channelName, const std::string &message)
{
    std::string response = ":" + client.getNick() + " PRIVMSG " + channelName + " " + message + "\r\n";

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

/*std::string parsePrivMessage(std::string message, std::string token)
{
    std::string parsedMessage;
    std::string::size_type pos = message.find(token);
    if (pos != std::string::npos)
    {
        parsedMessage = message.substr(pos);
    }
    return parsedMessage;
}

void Server::handlePrivmsg(Client &client, std::vector<std::string> tokens, int index, const std::string &message)
{
    std::string channelName = tokens[index + 1];
    std::string response = ":" + client.getNick() + " PRIVMSG " + channelName + " " + parsePrivMessage(message, tokens[index + 2]) + "\r\n";

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
}*/
