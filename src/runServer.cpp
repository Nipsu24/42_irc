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
#include <iostream>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <system_error>

volatile sig_atomic_t		server_running; // For signal handeling

/*
    * Create a server socket
*/
int Server::createServerSocket()
{
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        throw std::runtime_error("Socket operation failed: Connection refused");
    }

    // Set socket options (SO_REUSEADDR)
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
    {
        close(server_fd);
        throw std::system_error(errno, std::generic_category(), "setsockopt failed: Unable to set SO_REUSEADDR");
    }
    return server_fd;
}

/*
    * Bind and listen on the server socket
*/
void Server::bindAndListen(int server_fd)
{
    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(_port);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("Bind failed");
        close(server_fd);
        throw std::system_error(errno, std::generic_category(), "Failed to bind socket");
    }

    if (listen(server_fd, MAX_CLIENTS) == -1)
    {
        close(server_fd);
        throw std::system_error(errno, std::generic_category(), "Listen failed");
    }

    std::cout << "Server is listening on port " << _port << "..." << std::endl;
}

/*
    * Handle a new client connection
*/
void Server::handleNewClient(int server_fd)
{
    
    sockaddr_in client_addr = {};
    socklen_t client_len = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1)
    {
        perror("Accept failed");
        return;
    }
    std::cout << "New client connected." << std::endl;
    _clients.push_back(new Client(client_fd, client_addr));
    std::cout << "clientState:" << _clients.back()->getState() << std::endl; 
 }

/*
    * Handle events on the server socket and client sockets
*/
void Server::handleEvents(std::vector<struct pollfd> &fds)
{
    for (size_t i = 1; i < fds.size(); ++i)
    {
        if (fds[i].revents & POLLIN)
        {
            char buffer[BUFFER_SIZE];
            ssize_t bytes_read = recv(fds[i].fd, buffer, BUFFER_SIZE - 1, 0);
            if (bytes_read <= 0)
            {
                // Client disconnected or error
                std::cout << "Client disconnected." << std::endl;
                close(fds[i].fd);
                _clients.erase(_clients.begin() + i - 1);
                //--i;
            }
            else
            {
                // Null-terminate and process the received message
                buffer[bytes_read] = '\0';
                //std::cout << "Buffer: " << buffer << std::endl;
                std::string message(buffer);
                handleClientMessage(*_clients[i - 1], message);
            }
        }
    }
}

void Server::cleanupResources(int server_fd)
{
    for (auto &client : _clients)
    {
        close(client->getFd());
        delete client; // Don't forget to delete the Client object if you're using raw pointers
    }
    _clients.clear();
    for (auto &channel : _channels)
    {
        delete channel; // Don't forget to delete the Client object if you're using raw pointers
    }
    _channels.clear(); 
    close(server_fd);
}

void handle_sig(int sig) 
{
	if(sig == SIGSEGV)
        std::cerr << "Segmentation fault occurred! Exiting." << std::endl;
    else if(sig == SIGINT || sig == SIGTERM ||sig == SIGQUIT)
        std::cout << std::endl << "Shutting down server..." << std::endl;
    server_running = 0;
}

void HandleSignals()
{
    server_running = 1;
	signal(SIGINT, handle_sig);   // Handle Ctrl+C
    signal(SIGTERM, handle_sig); // Handle termination request (kill)
    signal(SIGQUIT, handle_sig); /* Handle Ctrl + \ */
    signal(SIGSEGV, handle_sig); // Handle segmentation fault
}

void Server::runServer()
{
    // Create and configure server socket
    int server_fd = createServerSocket();
    bindAndListen(server_fd);
    HandleSignals();

    std::vector<struct pollfd> fds;
    // Main event loop
    while (server_running)
    {
        // Set up the poll structure
        fds.clear();
        fds.push_back({server_fd, POLLIN, 0}); // Monitor the server socket

        // Add all client sockets to the poll list
        for (const auto &client : _clients)
        {
            fds.push_back({client->getFd(), POLLIN, 0});
        }

        int poll_result = poll(fds.data(), fds.size(), -1);
        if (poll_result == -1)
        {
            if (errno == EINTR)  // If poll is interrupted by signal.
                continue;
            perror("Poll failed");
            break;
        }
        // Check for events on the server socket (new connection)
        if (fds[0].revents & POLLIN )
        {
            handleNewClient(server_fd);
        }
        handleEvents(fds);
    }
    // Clean up and close all resources
    cleanupResources(server_fd);
}
