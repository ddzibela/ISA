#include "server.hpp"

server::server()
{
}

void gotPacket(u_char* args, const struct pcap_pkthdr* header,
	const u_char* packet)
{
	if (*args == DLT_LINUX_SLL) {
		auto sllHeader = (sll_header*)packet;
		if (sllHeader->sll_pkttype != LINUX_SLL_HOST) {
			return;
		}
		uint16_t ipv = 0;
		uint16_t ipSize;
		memcpy(&ipv, (packet + sizeof(sllHeader)), 1);
		ipv = ntohs(ipv);
		ipv &= 0xF;
		if (ipv == AF_INET) { //working with ipv4
			iphdr* ipHeader = (iphdr*)(packet + sizeof(sllHeader));
			
		}
		else if (ipv == AF_INET6){ //working with ipv6

		}
	}
	if (*args == DLT_LINUX_SLL2) {

	}
}

void server::run()
{
	const unsigned char* packet;
	char errbuf[PCAP_ERRBUF_SIZE];
	char filterstring[] = "icmp[icmptype] = icmp-echo or icmp6[icmp6type] = icmp6-echo";
	bpf_program captureFilter;
	pcap_pkthdr header;
	this->magic = 0;
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
	pcap_loop(handle, -1, &gotPacket, &linktype);
}
