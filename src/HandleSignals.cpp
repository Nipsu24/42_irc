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
#include <signal.h>
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include "Server.hpp"
#include "HandleSignals.hpp"

volatile sig_atomic_t server_running = 1;

// Signal handler for SIGINT (Ctrl+C)
void handle_sigint(int sig) 
{
	(void)sig;
    std::cout << "Received SIGINT, shutting down gracefully..." << std::endl;
    server_running = 0; // Set flag to stop the server
}

// Signal handler for SIGTERM (terminate signal)
void handle_sigterm(int sig) 
{
	(void)sig;
    std::cout << "Received SIGTERM, shutting down gracefully..." << std::endl;
    server_running = 0; // Set flag to stop the server
}

// Signal handler for SIGHUP (reload configuration)
void handle_sighup(int sig) 
{
	(void)sig;
    std::cout << "Received SIGHUP, reloading configuration..." << std::endl;
}

// Signal handler for SIGSEGV (segmentation fault)
void handle_sigsegv(int sig) 
{
	(void)sig;
    std::cerr << "Received SIGSEGV, segmentation fault occurred! Exiting." << std::endl;
    exit(1); 
}

void HandleSignals()
{
	signal(SIGINT, handle_sigint);   // Handle Ctrl+C
    signal(SIGTERM, handle_sigterm); // Handle termination request
    signal(SIGHUP, handle_sighup);   // Handle hang-up (e.g., to reload config)
    signal(SIGSEGV, handle_sigsegv); // Handle segmentation fault (just an example)
}







