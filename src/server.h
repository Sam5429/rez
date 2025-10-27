#pragma once
#include "dataStruct/util.h"
#include "layers/dataLinkLayer.h"
#include "layers/physicalLayer.h"
#include "protocols/ARP.h"
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

struct arp_line {
  ip_addr_t ip_addr;
  mac_addr_t mac_addr;
};

class Server : PhysicalLayer, DataLinkLayer {
private:
  int _read_fd;
  int _write_fd;
  mac_addr_t _mac_addr;
  ip_addr_t _ip_addr;
  std::unordered_map<ip_addr_t, mac_addr_t> _arp_table;
  mac_addr_t search_arp_table(ip_addr_t ip_addr);

public:
  Server(int read_fd, int write_fd, mac_addr_t mac_addr, ip_addr_t _ip_addr);

  void run_serv();
  void run_cli();

  // implement the PhysicalLayer methods

  void send_bytes(bitstream_t *bitstream);
  void receve_bytes();

  // implement the DataLinkLayer methods

  void send_tram(bitstream_t *payload, ip_addr_t ip_next_hop, byte protocol[2]);
  void receve_tram(bitstream_t *bitstream);

  // implement the Address Resolution Protocol

  void send_arp_response(ip_addr_t ip_dest, mac_addr_t mac_dest);
  void send_arp_request(ip_addr_t ip_dest);
  void receve_arp(bitstream_t *arp_body);
};
