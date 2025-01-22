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

void Server::handlePrivmsg(Client &client, const std::string channelName, const std::string &message)
{

    
    std::string strippedMessage = message;
    strippedMessage.erase(0, message.find_first_not_of(' '));
    std::string response = ":" + client.getNick() + " PRIVMSG " + channelName + message;
    for (Channel *channel : _channels)
    {
        if (channel->getChannelName() == channelName)
        {
            for (Client *_client : channel->getUsers())
            {
                if (_client != &client)
                {
                    MessageServerToClient(*_client, response);
                    break;
                }
            }
            break;
        }
    }
}
