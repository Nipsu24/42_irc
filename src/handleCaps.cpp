
#include "Server.hpp"
#include "response.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>
#include <system_error>


void Server::handleCAPs(Client &client, const std::vector<std::string>& tokens, int index)
{
	std::string response;
	if (tokens[index + 1] == "LS")
	{
		std::cout << "Received CAP LS from client" << client.getFd() << ": " << client.getNick() << std::endl; // Debugging
		response = "CAP * LS :multi-prefix sasl";
		MessageServerToClient(client, response);
	}
	else if (tokens[index + 1] == "REQ")
	{
		response = ":localhost CAP " + client.getNick() + " ACK :multi-prefix";
		MessageServerToClient(client, response);
	}
	else if (tokens[index + 1] == "END")
	{

		std::cout << "END sended" << std::endl;
		if (!client.getPasswdOK())
		{
			MessageServerToClient(client, RPL_PASSWDREQUEST());
			MessageServerToClient(client, ERR_PASSWDMISMATCH(client.getNick()));
			close(client.getFd());
			removeClient(client.getFd());
			return;
		}
		else
		{
			client.setState(REGISTERED);
			MessageServerToClient(client, RPL_WELCOME(client.getNick()));
		}
		
	}
	else
	{
		std::cerr << "Invalid CAP message" << std::endl;
	}
}