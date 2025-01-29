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

#include "Server.hpp"
#include "Channel.hpp"
#include <iostream>

bool isMessagePrivate(const std::string &message)
{
    if(message[0] == ':' && message[message.length()-1] == ':')
        return true;
    else
        return false;
}

void Server::handlePrivmsg(Client &client, const std::string channelName, std::string firstWord, std::string &message)
{    
    removeWhitespace(message);
	std::string response = ":" + client.getNick() + " PRIVMSG " + channelName+ " " + firstWord  + message;
    if(isMessagePrivate(firstWord)) // Check if the message is private, and try to send it
    {
        std::cout << "Trying send private message: "<< response << std::endl;
        std::string nick = firstWord.substr(1, firstWord.length()-2);
        for (Client *_client : _clients)
        {
            if (_client->getNick() == nick)
            {
                MessageServerToClient(*_client, response);
                return; // If client is found, return
            }
        }
    }
    std::cout << "Public message: " << response << std::endl;
    for (Channel *channel : _channels)
    {
        if (channel->getChannelName() == channelName)
        {
            for (Client *_client : channel->getUsers())
            {
                if (_client != &client)
                {
                    MessageServerToClient(*_client, response);                    
                }
            }            
        }
    }
    
}
