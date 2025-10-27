#include "util.h"

class TramARP {
public:
  byte _hardware[2];
  byte _networkLayer[2];
  byte _operation;
  mac_addr_t _mac_src;
  ip_addr_t _ip_src;
  mac_addr_t _mac_dest;
  ip_addr_t _ip_dest;

  TramARP(bitstream_t *tram_arp);
  TramARP(byte operation, mac_addr_t mac_src, ip_addr_t ip_src,
          mac_addr_t mac_dest, ip_addr_t ip_dest);
  bitstream_t *get_tram_arp();

  std::string to_string();
};
