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

void client::getFileName(const std::string filePath)
{
	std::size_t found = filePath.find_last_of("/\\");
	this->fileName = filePath.substr(found + 1);
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

void client::sendFile()
{
	addrinfo* serverInfo;
	getAddr(this->hostName.c_str(), serverInfo);
	this->icmpv = ((serverInfo->ai_family == AF_INET) ? IPPROTO_ICMP : IPPROTO_ICMPV6);//determine protocol version from ip version

	//open raw socket
	int sock = socket(serverInfo->ai_family, serverInfo->ai_socktype, this->icmpv);
	if (sock == -1) {
		throw std::runtime_error("Error opening socket.");
	}

	//icmp header construction
	//file operations
	//encryption
	//icmp checksum calculation
	//send packet
	//listen for return packet 
	//poll to check we can write again
	//repetitio ad absurdum
}