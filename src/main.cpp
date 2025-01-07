/* ******************************************************************************************************************************************** */
/*                                                                                                                                              */
/*         ::::::::::: :::::::::   ::::::::                 :::                                                                                 */
/*            :+:     :+:    :+: :+:    :+:               :+:                                                                                   */
/*           +:+     +:+    +:+ +:+                     +:+                                                                                     */
/*          +#+     +#++:++#:  +#+                    +#+                                                                                       */
/*         +#+     +#+    +#+ +#+                   +#+                                                                                         */
/*        #+#     #+#    #+# #+#    #+#           #+#                                                                                           */
/*   ########### ###    ###  ########           ###                                                                                             */
/*                                                          :::       ::: :::::::::: :::::::::   ::::::::  :::::::::: :::::::::  :::     :::    */
/*                                                         :+:       :+: :+:        :+:    :+: :+:    :+: :+:        :+:    :+: :+:     :+:  	*/
/*                                                        +:+       +:+ +:+        +:+    +:+ +:+        +:+        +:+    +:+ +:+     +:+   	*/
/*                                                       +#+  +:+  +#+ +#++:++#   +#++:++#+  +#++:++#++ +#++:++#   +#++:++#:  +#+     +:+    	*/
/*   By: Timo Saari<tsaari@student.hive.fi>,	        +#+ +#+#+ +#+ +#+        +#+    +#+        +#+ +#+        +#+    +#+  +#+   +#+         */
/*   	 Matti Rinkinen<mrinkine@student.hive.fi>,      #+#+# #+#+#  #+#        #+#    #+# #+#    #+# #+#        #+#    #+#   #+#+#+#           */
/*       Marius Meier<mmeier@student.hive.fi>           ###   ###   ########## #########   ########  ########## ###    ###     ###              */
/*                                                                                                                                              */
/* ******************************************************************************************************************************************** */



//TESTING BASIC SERVER FROM CHATGPT



#include <iostream>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <poll.h>
#include <string>


const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

// Struct to hold information about each connected client
struct Client {
    int fd;
    sockaddr_in addr;
};

// Function to broadcast messages to all clients
void broadcastMessage(const std::vector<Client>& clients, const char* message, int sender_fd) {
    for (const auto& client : clients) {
        if (client.fd != sender_fd) {
            send(client.fd, message, strlen(message), 0);
        }
    }
}

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



	if (argc != 2)
		return (printErrorMessage(0));
    // Create a socket
	int PORT = std::stoi(argv[1]);
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Socket creation failed");
        return 1;
    }

    // Set SO_REUSEADDR to allow binding to the port even if it's in TIME_WAIT
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1) {
        perror("setsockopt failed");
        close(server_fd);
        return 1;
    }




    // Set up the server address structure
    sockaddr_in server_addr = {};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Bind the socket to the port
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        close(server_fd);
        return 1;
    }

    // Start listening for incoming connections
    if (listen(server_fd, MAX_CLIENTS) == -1) {
        perror("Listen failed");
        close(server_fd);
        return 1;
    }

    std::cout << "Server is listening on port " << PORT << "..." << std::endl;

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


