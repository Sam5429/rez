#pragma once

#include <array>
#include <cstdint>
#include <string>
#include <unistd.h>
#include <vector>

typedef uint8_t byte;
typedef uint64_t mac_addr_t;
typedef uint32_t ip_addr_t;
typedef std::vector<byte> bitstream_t;

void print_mac(const mac_addr_t mac_addr);

std::string mac_to_string(const mac_addr_t mac_addr);
std::string ip_to_string(const ip_addr_t ip_addr);
