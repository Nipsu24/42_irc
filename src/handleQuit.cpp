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