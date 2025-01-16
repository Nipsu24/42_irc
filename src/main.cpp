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




int	printErrorMessage(int errorFlag)
{
	if (errorFlag == 0)
		std::cout << "Error." << std:: endl << "Insufficient arguments, server needs <port> and <password>." << std::endl;
	if (errorFlag == 1)
		std::cout << "Error." << std:: endl << "Invalid input. The port number has to consist of digits only." << std::endl;
	if (errorFlag == 2)
		std::cout << "Error." << std:: endl << "Input number is out of range. Please try again." << std::endl;
	return (1);
}

int main(int argc, char **argv) {

	(void) argv;
	if (argc != 3)
		return (printErrorMessage(0));
	//segfault if argv[1] is not numbers
	Server server(std::stoi(argv[1]), argv[2]);

    try
	{
	    server.runServer(); 
	}
	catch(const std::exception& e)
	{
        std::cerr << e.what() << '\n';
        return(1);
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


