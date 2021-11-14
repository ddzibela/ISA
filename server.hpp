#pragma once
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <netdb.h>	   // getaddrinfo
#include <string.h>	   //memset
#include <arpa/inet.h> // inet_ntop
#include <netinet/ip_icmp.h> //icmp headers
#include <openssl/aes.h> //AES symmetric encryption
#include <pcap.h>
#include "optArgs.hpp"
#include "mtu.hpp" //define MTU
#include "icmp.hpp" //Echo header

class server
{
private:
	std::string captureFilter;
	uint16_t magic;
public:
	server();
	void run();
};

