
#include "Server.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <system_error>



void broadcastMessage(const std::vector<Client>& clients, const char* message, int sender_fd) {
	for (const auto& client : clients) {
		if (client.fd != sender_fd) {
			send(client.fd, message, strlen(message), 0);
		}
	}
}



void Server::runServer()
{
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd == -1) {
		throw std::runtime_error("Socket operation failed: Connection refused");
	}

	/*The setsockopt function is used to set options for a socket. In your code, it sets the SO_REUSEADDR option, 
	which allows a socket to bind to an address that is in the TIME_WAIT state or is otherwise considered unavailable for reuse.
	- &opt: A pointer to the value to set the option (1 typically means enable the option)*/

	int opt = 1;
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
	close(server_fd);
	throw std::system_error(errno, std::generic_category(), "setsockopt failed: Unable to set SO_REUSEADDR");
	}


	// Set up the server address structure
	sockaddr_in server_addr = {};
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(_port);

	/*
	The bind function in socket programming is used to assign a specific address (IP and port) to a socket. 
	This is a critical step when creating a server, as it determines where the server will listen for incoming connections.*/

	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
		perror("Bind failed");
		close(server_fd);
		throw std::system_error(errno, std::generic_category(), "Failed to bind socket");
	}

	// Start listening for incoming connections
	if (listen(server_fd, MAX_CLIENTS) == -1) {
	close(server_fd);
	throw std::system_error(errno, std::generic_category(), "Listen failed");
	}

	std::cout << "Server is listening on port " << getPort() << "..." << std::endl;

	// Set up a list of clients
	std::vector<Client> clients;

	// Use poll to handle multiple connections (non-blocking)
	while (true) {
		// Set up the poll structure
		std::vector<struct pollfd> fds;
		fds.push_back({server_fd, POLLIN, 0}); // Monitor the server socket

		// Add all connected clients to poll list
		for (const auto& client : clients) {
			fds.push_back({client.fd, POLLIN, 0});
		}

		// Wait for events (blocking)
		int poll_result = poll(fds.data(), fds.size(), -1); // Timeout of -1 means blocking indefinitely
		if (poll_result == -1) {
			perror("Poll failed");
			break;
		}

		// Check for events on the server socket (new connection)
		if (fds[0].revents & POLLIN) {
			sockaddr_in client_addr = {};
			socklen_t client_len = sizeof(client_addr);
			int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
			if (client_fd == -1) {
				perror("Accept failed");
				continue;
			}

			std::cout << "New client connected." << std::endl;

			// Add the new client to the list
			clients.push_back(Client{client_fd, client_addr});
		}

		// Check for events on each client socket (messages)
		for (size_t i = 1; i < fds.size(); ++i) {
			if (fds[i].revents & POLLIN) {
				char buffer[BUFFER_SIZE];
				ssize_t bytes_read = recv(fds[i].fd, buffer, BUFFER_SIZE - 1, 0);
				if (bytes_read <= 0) {
					// Client disconnected or error
					std::cout << "Client disconnected." << std::endl;
					close(fds[i].fd);
					clients.erase(clients.begin() + i - 1);
					--i;
				} else {
					// Null-terminate and broadcast the message to other clients
					buffer[bytes_read] = '\0';
					std::cout << "Received: " << buffer << std::endl;
					broadcastMessage(clients, buffer, fds[i].fd);
				}
			}
		}
	}

	// Close all client sockets and the server socket
	for (const auto& client : clients) {
		close(client.fd);
	}
	close(server_fd);
}