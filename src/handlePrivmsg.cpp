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
#include "response.hpp"

void Server::handlePrivmsg(Client &client, const std::string channelNameOrNick,  std::string &message)
{    
    message.erase(0, message.find_first_not_of(' '));
	message.erase(message.find_last_not_of(" \n\r\t")+1);
    if (channelNameOrNick[0] == '#')
    {
        for (Channel *channel : _channels)
        {
            if (channel->getChannelName() == channelNameOrNick)
            {
                for (Client *_client : channel->getUsers())
                {
                    if (_client != &client)
                    {
                        MessageServerToClient(*_client, RPL_PRIVMSG(client.getNick(), channelNameOrNick, message));                    
                    }
                }            
            }
        }
    }
    else
    {
        for (Client *_client : _clients)
        {
            if (_client->getNick() == channelNameOrNick)
            {
                MessageServerToClient(*_client, RPL_PRIVMSG(client.getNick(), _client->getNick(), message));                    
            }
        }  
    }
}
