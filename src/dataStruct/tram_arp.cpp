#include "tram_arp.h"
#include "util.h"
#include <cstring>
#include <iostream>
#include <sstream>

TramARP::TramARP(bitstream_t *tram_arp) {
  memcpy(_hardware, tram_arp->data(), sizeof(byte[2]));
  memcpy(_networkLayer, tram_arp->data() + sizeof(byte[2]), sizeof(byte[2]));
  memcpy(&_operation, tram_arp->data() + 2 * sizeof(byte[2]), sizeof(byte));

  _mac_src = *reinterpret_cast<const mac_addr_t *>(
      tram_arp->data() + 2 * sizeof(byte[2]) + sizeof(byte));
  _ip_src = *reinterpret_cast<const ip_addr_t *>(
      tram_arp->data() + 2 * sizeof(byte[2]) + sizeof(byte) +
      sizeof(mac_addr_t));
  _mac_dest = *reinterpret_cast<const mac_addr_t *>(
      tram_arp->data() + 2 * sizeof(byte[2]) + sizeof(byte) +
      sizeof(mac_addr_t) + sizeof(ip_addr_t));
  _ip_dest = *reinterpret_cast<const ip_addr_t *>(
      tram_arp->data() + 2 * sizeof(byte[2]) + sizeof(byte) +
      2 * sizeof(mac_addr_t) + sizeof(ip_addr_t));
}

TramARP::TramARP(byte operation, mac_addr_t mac_src, ip_addr_t ip_src,
                 mac_addr_t mac_dest, ip_addr_t ip_dest) {
  _hardware[0] = 1;
  _hardware[1] = 0;

  _networkLayer[0] = 0;
  _networkLayer[1] = 8;

  _operation = operation;

  _mac_src = mac_src;
  _ip_src = ip_src;
  _mac_dest = mac_dest;
  _ip_dest = ip_dest;
}
bitstream_t *TramARP::get_tram_arp() {
  bitstream_t *tram_arp = new bitstream_t;
  tram_arp->reserve(2 * sizeof(byte[2]) + sizeof(byte) +
                    2 * sizeof(mac_addr_t) + 2 * sizeof(ip_addr_t));

  tram_arp->insert(tram_arp->end(), _hardware, _hardware + sizeof(byte[2]));
  tram_arp->insert(tram_arp->end(), _networkLayer,
                   _networkLayer + sizeof(byte[2]));
  tram_arp->push_back(_operation);

  tram_arp->insert(tram_arp->end(), reinterpret_cast<byte *>(&_mac_src),
                   reinterpret_cast<byte *>(&_mac_src) + sizeof(mac_addr_t));
  tram_arp->insert(tram_arp->end(), reinterpret_cast<byte *>(&_ip_src),
                   reinterpret_cast<byte *>(&_ip_src) + sizeof(ip_addr_t));
  tram_arp->insert(tram_arp->end(), reinterpret_cast<byte *>(&_mac_dest),
                   reinterpret_cast<byte *>(&_mac_dest) + sizeof(mac_addr_t));
  tram_arp->insert(tram_arp->end(), reinterpret_cast<byte *>(&_ip_dest),
                   reinterpret_cast<byte *>(&_ip_dest) + sizeof(ip_addr_t));

  return tram_arp;
}

std::string TramARP::to_string() {
  std::stringstream ss;
  ss << "Hardware Type: " << (int)_hardware[0] << "." << (int)_hardware[1]
     << "\n";
  ss << "Network Layer: " << (int)_networkLayer[0] << "."
     << (int)_networkLayer[1] << "\n";
  ss << "Operation: " << (int)_operation << "\n";
  ss << "Source MAC: " << mac_to_string(_mac_src) << "\n";
  ss << "Source IP: " << ip_to_string(_ip_src) << "\n";
  ss << "Destination MAC: " << mac_to_string(_mac_dest) << "\n";
  ss << "Destination IP: " << ip_to_string(_ip_dest) << "\n";
  return ss.str();
}
