#pragma once

#include <unistd.h>
#include <vector>

typedef unsigned char byte;
typedef byte mac_addr_t[6];
typedef byte ip_addr_t[2];
typedef std::vector<byte> bitstream_t;

void print_mac(const mac_addr_t mac_addr);
