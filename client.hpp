#pragma once
#include "optArgs.hpp"

#include <netdb.h>	   // getaddrinfo
#include <string.h>	   //memset
#include <arpa/inet.h> // inet_ntop
#include <netinet/ip_icmp.h>
#include <openssl/aes.h>

class client
{
private:
	std::string hostName;
	std::string filePath;
	void getAddr(const char* host, struct addrinfo* serverInfo);
public:
	void sendFile();
	client(optArgs* args);
};

