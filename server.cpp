#include "server.hpp"

server::server()
{
	this->captureFilter = "inbound and (icmp[icmptype] == icmp-echo of icmp6[icmp6type] == icmp6-echo)";
}

void server::run()
{
	char errbuf[PCAP_ERRBUF_SIZE];
	bpf_program captureFilter;
	//setup pcap
	pcap_t* handle = pcap_open_live("any", BUFSIZ, 1, 1000, errbuf);
	if (handle == nullptr) {
		throw std::runtime_error(errbuf);
	}

	if (pcap_compile(handle, &captureFilter, this->captureFilter.c_str(), 0, PCAP_NETMASK_UNKNOWN) == 0) {
		pcap_perror(handle, "");
		throw;
	}
	if (pcap_setfilter(handle, &captureFilter) == -1) {
		pcap_perror(handle, "");
		throw;
	}
	//capture packets
}
