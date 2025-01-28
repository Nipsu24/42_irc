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

#pragma once

#include "Channel.hpp"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <time.h>

//nick
#define ERR_NOSUCHNICK(nick, nicktofind)                "401 " + nick + " " + nicktofind + " :No such nick/channel"
#define ERR_NICKNAMEINUSE(oldnick, nick)                "433 " + oldnick + " " + nick  + " :Nickname is already in use"

//channel
#define RPL_CREATIONTIME(nickname, channelName, timestamp)          "329 " + nickname + " " + channelName + " " + timestamp
#define ERR_NOSUCHCHANNEL(nick, channelname)                        "403 " + nick + " " + channelname + " :No such channel"
#define ERR_USERNOTINCHANNEL(clientnick, usernickname, channelname) "441 " + nick + " " + usernickname + " " + channelname + " :They aren't on that channel"
#define ERR_NOTONCHANNEL(clientnick, channelname)                   "442 " + clientnick + " " + channelname + " :They aren't on that channel"
#define ERR_USERONCHANNEL(clientnick, usernick, channelname)        "443 " + clientnick + " " + nick + " " + channelName + " :is already on channel"
#define ERR_CHANOPRIVSNEEDED(nick, channelname)                     "482 " + nick + " " + channelname + " :You're not channel operator"


//#define ERR_NOTONCHANNEL(source, channel)               "442 " + source + " " + channel + " :You're not on that channel"
//#define ERR_NOTREGISTERED(source)                       "451 " + source + " :You have not registered"
//#define ERR_ALREADYREGISTERED(source)                   "462 " + source + " :You may not register"
//#define ERR_PASSWDMISMATCH(source)                      "464 " + source + " :Password is incorrect"
//#define ERR_NONICKNAMEGIVEN(source)                     "431 " + source + " :Nickname not given"


//#define ERR_UNKNOWNCOMMAND(source, command)             "421 " + source + " " + command + " :Unknown command"
#define ERR_NEEDMOREPARAMS(nickname)                      "461 " + nickname + " MODE :Not enough parameters"

//#define ERR_TOOMANYCHANNELS(source, channel)            "405 " + source + " " + channel + " :You have joined too many channels"

//modes
#define RPL_CHANNELMODEIS(nickname, channelName, channelModes) "324 " + nickname + " " + channelName + " " + channelModes
#define ERR_CHANNELISFULL(nickname, channelName)               "471 " + nickname + " " + channelName + " :Cannot join channel (+l) - channel is full, try again later"
#define ERR_UNKNOWNMODE(nickname, mode)                        "472 " + nickname + " " + mode + " :is an unknown mode char to me"
#define ERR_INVITEONLYCHAN(nickname, channelName)              "473 " + nickname + " " + channelName + " :Cannot join channel (+i) - you must be invited"
#define ERR_BADCHANNELKEY(nickname, channelName)               "475 " + nickname + " " + channelName + " :Cannot join channel (+k) - bad key"

//#define ERR_CANNOTSENDTOCHAN(source, channel)           "404 " + source + " " + channel + " :Cannot send to channel"


/* Numeric Responses */
#define RPL_WELCOME(nickname)                             "001 " + nickname + " :Welcome " + nickname + " to the ft_irc network"
#define RPL_NAMREPLY(nickname, channelname, users)        "353 " + nickname + " @ " + channelname + " :@" + users
#define RPL_ENDOFNAMES(source, channelname)               "366 " + source + " " + channelname + " :End of /NAMES list."


/* Command Responses */
#define RPL_INVITING(clientnick, nick, channelname)		":" + clientnick + " INVITE " + nick + " " + channelname
#define RPL_NICK(oldnick, username, nick)				":" + oldNick + " NICK :" + nick
#define RPL_TOPIC(clientnick, channelname, newtopic)	":" + clientnick + " TOPIC " + channelName + " " + newtopic
#define RPL_JOIN(source, channel)                       ":" + source + " JOIN :" + channel
#define RPL_KICK(source, channel, target, reason)       ":" + source + " KICK " + channel + " " + target + " :" + reason
//#define RPL_PART(source, channel)                       ":" + source + " PART :" + channel
//#define RPL_PING(source, token)                         ":" + source + " PONG :" + token
//#define RPL_PRIVMSG(source, target, message)            ":" + source + " PRIVMSG " + target + " :" + message
//#define RPL_NOTICE(source, target, message)             ":" + source + " NOTICE " + target + " :" + message
//#define RPL_QUIT(source, message)                       ":" + source + " QUIT :Quit: " + message
