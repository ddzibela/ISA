#pragma once
#include "optArgs.hpp"

#include <netdb.h>	   // getaddrinfo
#include <string.h>	   //memset
#include <arpa/inet.h> // inet_ntop
#include <netinet/ip_icmp.h> //icmp headers
#include <openssl/aes.h> //AES symmetric encryption

class client
{
private:
	std::string hostName;
	std::string filePath;
	std::string fileName;
	int icmpv; //ICMP version

	void getAddr(const char* host, struct addrinfo* serverInfo);
	void getFileName(const std::string filePath);
public:
	void sendFile();
	client(optArgs* args);
};

