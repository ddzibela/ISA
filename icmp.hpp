#pragma once
#include <cstdint>
#include <fstream>

#define xlogin (unsigned char*)"xdzibe00xdzibe00"

struct icmpEcho
{
	uint8_t type;
	uint8_t code;
	uint16_t checksum;
	uint16_t identifier;
	uint16_t sequence;
};

struct secretProtoEstablish
{
	uint16_t magic;
	uint64_t nameLen;
	uint64_t size;
	char* name;
};

struct secretProtoTransfer
{
	uint16_t magic;
	uint16_t dataLen;
	char* data;
};