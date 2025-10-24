#pragma once
#include "util.h"
#include <string>

class Tram {
private:
  mac_addr_t _dest_mac;
  mac_addr_t _src_mac;
  byte _protocol;
  std::string _payload;

public:
  Tram(bitstream_t *tram);
  Tram(mac_addr_t dest_mac, mac_addr_t src_mac, byte protocol,
       std::string payload);
  bitstream_t *get_tram();
  std::string get_payload();
  std::string to_string();
};
