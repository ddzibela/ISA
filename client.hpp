#pragma once
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <exception>
#include <poll.h>
#include <netinet/in.h>
#include <netdb.h>	   // getaddrinfo
#include <string.h>	   //memset
#include <arpa/inet.h> // inet_ntop
#include <netinet/ip_icmp.h> //icmp headers
#include <openssl/aes.h> //AES symmetric encryption
#include "optArgs.hpp"
#include "mtu.hpp" //define MTU
#include "icmp.hpp" //Echo header

class client
{
private:
	std::string hostName;
	std::string filePath;
	std::string fileName;
	int icmpv; //ICMP version
	AES_KEY key;

	void getFileName(const std::string filePath);
public:
	void sendFile();
	client(optArgs* args);
};

