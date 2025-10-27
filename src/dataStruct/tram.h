#pragma once
#include "util.h"

class Tram {
public:
  mac_addr_t _dest_mac;
  mac_addr_t _src_mac;
  byte _protocol[2];
  bitstream_t _payload;

  Tram(bitstream_t *tram);
  Tram(mac_addr_t dest_mac, mac_addr_t src_mac, byte protocol[2],
       bitstream_t payload);
  bitstream_t *get_tram();
  std::string to_string();
};
