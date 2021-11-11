#include "client.hpp"
#include <iostream>
#include <exception>

client::client(optArgs* args)
{
	if (args != nullptr) {
		this->hostName = args->s_specifier;
		this->filePath = args->r_specifier;
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
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_RAW;
	
	if ((result = getaddrinfo(host, NULL, &hints, &serverInfo)) != 0)
	{
		std::cerr << gai_strerror(result);
		throw std::runtime_error(gai_strerror(result));
	}
	std::cout << "got here";
}

void client::sendFile()
{
	addrinfo* serverInfo;
	getAddr(this->hostName.c_str(), serverInfo);
	this->icmpv = ((serverInfo->ai_family == AF_INET) ? IPPROTO_ICMP : IPPROTO_ICMPV6);//determine protocol version from ip version

	std::cout << "AF_INET:" << AF_INET << "ICMPV4" << IPPROTO_ICMP << std::endl;

	//open raw socket
	int sock = socket(serverInfo->ai_family, serverInfo->ai_socktype, this->icmpv);
	if (sock == -1) {
		throw std::runtime_error("Error opening socket.");
	}
	//packet fuckery
	uint8_t packet[MTU];
	std::fill(packet, packet + MTU, 0);//sometimes faster than memset
	//icmp header construction
	icmphdr* icmpHeader = (icmphdr*)packet;//cast icmp header onto packet
	icmpHeader->code = ICMP_ECHO;

	//file operations
	std::ifstream file;
	file.open(this->filePath, std::ios::in | std::ios::binary | std::ios::ate); //open file in binary mode, start at the end
	if (!file.is_open()) {
		throw std::runtime_error("Error opening file.");
	}
	std::streampos fileSize = file.tellg(); // get filesize in B
	file.seekg(0, std::ios::beg); //return to beginning of file

	//do while loop
		//poll if we can send packet
		//if sequence = 1
			//send metadata = magic number, file name, file len
		//if sequence != 1
			//send packet
		
	//encryption
	//icmp checksum calculation
	//send packet
	//listen for return paccket
		//packet arrived safely
			//repetitio ad nauseam
		//packet arrived scuffed
			//resend
		//packet did not arrive in 30s
			//shut it down
}