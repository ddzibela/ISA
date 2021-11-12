#include "client.hpp"
#include <iostream>
#include <exception>
#include <algorithm>

void* get_in_addr(struct sockaddr* sa)
{
	if (sa->sa_family == AF_INET)
	{
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

client::client(optArgs* args)
{
	if (args != nullptr) {
		this->hostName = args->s_specifier;
		this->filePath = args->r_specifier;
		std::size_t found = filePath.find_last_of("/\\");
		this->fileName = filePath.substr(found + 1);
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

void client::sendFile()
{
	//get IP address
	addrinfo* serverInfo;
	addrinfo hints;
	int result;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_RAW;

	if ((result = getaddrinfo(this->hostName.c_str(), NULL, &hints, &serverInfo)) != 0)
	{
		std::cerr << gai_strerror(result);
		throw std::runtime_error(gai_strerror(result));
	}

	//get human readable ip, test only
	//char ip[100];
	//inet_ntop(serverInfo->ai_family, get_in_addr(serverInfo->ai_addr), ip, 100);
	//std::cerr << ip << std::endl;

	//determine protocol version from ip version
	this->icmpv = ((serverInfo->ai_family == AF_INET) ? IPPROTO_ICMP : IPPROTO_ICMPV6);

	//open raw socket
	int sock = socket(serverInfo->ai_family, serverInfo->ai_socktype, this->icmpv);
	if (sock == -1) {
		throw std::runtime_error("Error opening socket.");
	}

	//file operations
	std::ifstream file;
	file.open(this->filePath.c_str(), std::ios::in | std::ios::binary | std::ios::ate); //open file in binary mode, start at the end
	if (!file.is_open()) {
		throw std::runtime_error("Error opening file.");
	}
	std::streampos fileEndPos = file.tellg(); // get filesize in B
	file.seekg(0, std::ios::beg); //return to beginning of file

	std::streamoff fileSize = fileEndPos.operator std::streamoff();
	std::cerr << fileSize <<  std::endl;
	uint16_t sequence = 0;
	uint64_t sent = 1;
	do {
		sequence++;
		//packet fuckery
		uint8_t packet[MTU];
		std::fill(packet, packet + MTU, 0);//sometimes faster than memset
		//icmp header construction
		icmpEcho* icmpHeader = (icmpEcho*)packet;//cast icmp header onto packet
		icmpHeader->code = ICMP_ECHO;
		icmpHeader->sequence = sequence;

		if (sequence == 1) { //we are sending the first packet
			secretProtoEstablish* secret = (secretProtoEstablish*)(packet + sizeof(icmpEcho));
			secret->magic = icmpHeader->identifier;
			secret->size = fileSize;
			secret->nameLen = this->fileName.length() + 1;
			memcpy(&secret->name, this->fileName.c_str(), this->fileName.length() + 1);
			//char kokot[50];
			//memcpy(kokot, &secret->name, this->fileName.length() + 1);
			//std::cerr << kokot;
		}
		else {
			secretProtoTransfer* secret = (secretProtoTransfer*)(packet + sizeof(icmpEcho));
			secret->magic = icmpHeader->identifier;
			std::streamoff toSend = MTU - fileSize;
			toSend = (toSend < 0) ? 0 : toSend;
			char data[MTU];
			//std::fill(secret->data, data + MTU, 0);
			//holy mother of typecasting
			file.read((char*)(void*)(&secret->data), toSend);
			std::cerr << &secret->data;
		}
		
		//encrypt the packet
		//checksum
		//poll if we can write to socket
		//send
		//wait for return packet
		//repetitio ad nauseam
	} while (true);

}