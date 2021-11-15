#include "server.hpp"

server::server()
{
	this->magic = 0;
	this->candidateMagic = 0;
	this->sequence = 1;
}

void server::run()
{
	const unsigned char* packet;
	char errbuf[PCAP_ERRBUF_SIZE];
	char filterstring[] = "icmp[icmptype] = icmp-echo or icmp6[icmp6type] = icmp6-echo";
	bpf_program captureFilter;
	pcap_pkthdr header;
	//setup pcap
	pcap_t* handle = pcap_open_live("any", BUFSIZ, 1, 1000, errbuf);
	if (handle == nullptr) {
		throw std::runtime_error(errbuf);
	}

	if (pcap_compile(handle, &captureFilter, filterstring, 0, PCAP_NETMASK_UNKNOWN) == -1) {
		pcap_perror(handle, "");
		throw;
	}
	if (pcap_setfilter(handle, &captureFilter) == -1) {
		pcap_perror(handle, "");
		throw;
	}
	//capture packets
	unsigned char linktype = pcap_datalink(handle);
	while (true) {
		packet = pcap_next(handle, &header);
		if (linktype == DLT_LINUX_SLL) {
			auto sllHeader = (sll_header*)packet;
			if (sllHeader->sll_pkttype != LINUX_SLL_HOST) {
				continue;
			}
			uint16_t ipSize;
			iphdr* ipHeader = (iphdr*)(packet + sizeof(sll_header));
			if (ipHeader->version == 4) {
				ipSize = sizeof(iphdr);
			}
			else {
				//ipSize = sizeof()
			}

			icmpEcho* icmpHeader = (icmpEcho*)(packet + sizeof(sll_header) + ipSize);
			//decypher the packet here
			if (icmpHeader->sequence == 1 && this->sequence == 1) {
				//nadvazujeme spojenie
				secretProtoEstablish* secret = (secretProtoEstablish*)(packet + sizeof(sll_header) + ipSize + sizeof(icmpEcho));
				if (icmpHeader->identifier != secret->magic) continue;
				this->magic = secret->magic;
				this->size = secret->size;
				char* name = new char[secret->nameLen];
				memcpy(name, &secret->name, secret->nameLen);
				this->file.open(name, std::ios::binary | std::ios::out);
				delete name;
				this->sequence++;
			}
			else if (icmpHeader->sequence == 1 && this->sequence != 1) {
				//ignorujeme
				continue;
			}
			else if (icmpHeader->sequence != 1 && this->sequence == 1) {
				//ignorujeme
				continue;
			}
			else if (icmpHeader->sequence != 1 && this->sequence != 1) {
				//ukladame subor
				secretProtoTransfer* secret = (secretProtoTransfer*)(packet + sizeof(sll_header) + ipSize + sizeof(icmpEcho));
				if ((icmpHeader->identifier != secret->magic) && (this->magic != secret->magic)) continue;
				this->file.write((char*)(void*)&secret->data, secret->dataLen);
				this->size -= secret->dataLen;
			}
			if (this->size == 0) {
				//we have received the file, start with clean slate
				this->file.close();
				this->magic = 0;
				this->candidateMagic = 0;
				this->sequence = 1;
			}
		}
		else if (linktype == DLT_LINUX_SLL2) {

		}


	}
}
