#include "client.hpp"
#include <iostream>
#include <exception>

client::client(optArgs* args)
{
	if (args != nullptr) {
		this->hostName = args->r_specifier;
		this->filePath = args->s_specifier;
	}
	else {
		throw std::invalid_argument("*args cannot be nullptr");
	}
}

void client::sendFile()
{
	addrinfo* serverInfo;
	getAddr(this->hostName.c_str(), serverInfo);
}


void client::getAddr(const char* host, addrinfo* serverInfo)
{
	addrinfo hints;
	int result;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_RAW;
	if ((result = getaddrinfo(host, NULL, &hints, &serverInfo)) != 0)
	{
		//std::cerr << gai_strerror(result);
		throw std::runtime_error(gai_strerror(result));
	}
}
