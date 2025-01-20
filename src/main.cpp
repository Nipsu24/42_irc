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

#include <iostream>
#include <string>
#include "Server.hpp"

int printErrorMessage(int errorFlag)
{
	if (errorFlag == 0)
		std::cout << "Error." << std::endl
				  << "Insufficient arguments, server needs <port> and <password>." << std::endl;
	if (errorFlag == 1)
		std::cout << "Error." << std::endl
				  << "Invalid port number. Recommended ports are 6665 - 6669" << std::endl;
	if (errorFlag == 2)
		std::cout << "Error." << std::endl
				  << "Input number is out of range. Please try again." << std::endl;
	return (1);
}

int checkValidPort(std::string port)
{
	int portNo;
	if (port.length() > 5)
		return 0;
	for (size_t i = 0; i < port.length(); i++)
	{
		if (!isdigit(port[i]))
			return 0;
	}
	portNo = std::stoi(port);
	if (portNo < 1024 || portNo > 49151)
		return 0;
	return portNo;
}

int main(int argc, char **argv)
{
	if (argc != 3)
		return (printErrorMessage(0));
	int port = checkValidPort(argv[1]);
	if (!port)
		return (printErrorMessage(1));
	Server server(port, argv[2]);
	try
	{
		server.runServer();
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << '\n';
		return (1);
	}
	return 0;
}

/*

int	main(int ac, char** av)
{
	int	portNo = -1;

	if (ac != 3)
		return (printErrorMessage(0));
	std::string	arg1 = av[1];
	std::string	arg2 = av[2];
	if (arg1.empty() || arg2.empty())
		return (printErrorMessage(0));
	try
	{
		portNo = stoi(arg1);
	}
	catch (std::invalid_argument &e)
	{
		return (printErrorMessage(1));
	}
	catch (std::out_of_range &e)
	{
		return (printErrorMessage(1));
	}
	std::cout << "Welcome to your IRC / Webserv Server!" << std::endl;
	return (0);
}*/
