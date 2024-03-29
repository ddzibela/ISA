#pragma once
#include <cstdint>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <netinet/in.h>
#include <utility>
#include <netdb.h>	   // getaddrinfo
#include <string.h>	   //memset
#include <arpa/inet.h> // inet_ntop
#include <netinet/ip_icmp.h> //icmp headers
#include <openssl/aes.h> //AES symmetric encryption
#include <pcap.h>
#include "optArgs.hpp"
#include "mtu.hpp" //define MTU
#include "icmp.hpp" //Echo header
#include "sll.hpp"

class server
{
private:
	uint16_t magic;
	uint16_t candidateMagic;
	uint16_t sequence;
	uint64_t size;
	std::ofstream file;
	AES_KEY key;
public:
	server();
	void run();
};

