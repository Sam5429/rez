#include "../dataStruct/util.h"

enum { ARP_REQUEST = 1, ARP_RESPONSE = 2 };

/*
 * Address Resolution Protocol
 */
class ARP {
public:
  virtual void send_arp_response(ip_addr_t ip_dest, mac_addr_t mac_dest) = 0;
  virtual void send_arp_request(ip_addr_t ip_dest) = 0;
  virtual void receve_arp(bitstream_t *arp_body) = 0;
};
