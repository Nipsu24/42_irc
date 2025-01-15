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

#include "Channel.hpp"
#include "Server.hpp"
#include <algorithm>

std::vector<Channel *> Server::getChannels() {
	return (_channels);
}

void Server::handleMode(Client &client, std::vector<std::string> tokens, int index, const std::string &message) {
	 if (tokens.size() == 2) {
		auto it = find_if(getChannels().begin(), getChannels().end(), [&tokens](Channel *channel) {
			return channel->getChannelName() == tokens[1];
		});
		if (it != getChannels().end()) {
			//check if client is in channel and then reply to client
		}
	 }
}