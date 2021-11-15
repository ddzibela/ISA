#include "client.hpp"

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
		//AES_set_encrypt_key((const unsigned char*)xlogin00, 128, &this->key);
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


	srand(time(NULL));
	bool cont = true;
	uint16_t sequence = 0;
	uint16_t magic = rand();
	std::streamoff bytesRead;
	pollfd pollFds = { sock, POLLOUT, 0};
	AES_set_encrypt_key(xlogin, 128, &this->key);

	do {
		sequence++;
		//packet fuckery
		uint8_t packet[MTU];
		std::fill(packet, packet + MTU, 0);//sometimes faster than memset
		//icmp header construction
		icmpEcho* icmpHeader = (icmpEcho*)packet;//cast icmp header onto packet
		if (icmpv == IPPROTO_ICMP) {
			icmpHeader->type = ICMP_ECHO;
		}
		else {
			icmpHeader->type = 128;
		}
		icmpHeader->sequence = sequence;
		icmpHeader->identifier = magic;

		if (sequence == 1) { //we are sending the first packet
			//Fill out secret header
			secretProtoEstablish* secret = (secretProtoEstablish*)(packet + sizeof(icmpEcho));
			secret->magic = icmpHeader->identifier;
			secret->size = fileSize;
			secret->nameLen = this->fileName.length() + 1;
			memcpy(&secret->name, this->fileName.c_str(), this->fileName.length() + 1);

			//encrypt the secret data


			//AES_encrypt((unsigned char*)(void*)secret, (unsigned char*)(void*)secret, &this->key);
		}
		else { //we are sending the file
			secretProtoTransfer* secret = (secretProtoTransfer*)(packet + sizeof(icmpEcho));
			secret->magic = icmpHeader->identifier;
			std::streamoff toSend = MTU - sizeof(icmpEcho) - sizeof(secretProtoTransfer);
			bytesRead = file.tellg();
			//holy mother of typecasting
			file.read((char*)(void*)(&secret->data), toSend);
			//get data length
			if (file.eof()) {
				cont = false; //we do not need to continue anymore
				bytesRead = fileSize - bytesRead;
			}
			else {
				bytesRead = file.tellg() - bytesRead;
			}
			secret->dataLen = bytesRead;
			
			//AES_encrypt(packet, packet, &this->key);
		}

		
		//checksum
		
		//poll if we can write to socket
		if (poll(&pollFds, 1, 30000) == -1) {
			throw std::runtime_error("connection timeout");
		}
		//send
		//determine lenght of data to be sent
		if (sequence == 1) { 
			if (sendto(sock, packet, sizeof(icmpEcho) + sizeof(secretProtoEstablish) + this->fileName.length() - 8, 0, (struct sockaddr*)(serverInfo->ai_addr), serverInfo->ai_addrlen) < 0) {
				file.close();
				throw std::runtime_error("Error sending socket.");
			}
		}
		else {
			if (sendto(sock, packet, sizeof(icmpEcho) + sizeof(secretProtoTransfer) + bytesRead - 8, 0, (struct sockaddr*)(serverInfo->ai_addr), serverInfo->ai_addrlen) < 0) {
				file.close();
				throw std::runtime_error("Error sending socket.");
			}
		}
		//wait for return packet
		//repetitio ad nauseam
	} while (cont);
	
	file.close();
}