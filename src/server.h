#pragma once
#include "dataStruct/util.h"
#include "layers/dataLinkLayer.h"
#include "layers/physicalLayer.h"
#include <string>

class Server : PhysicalLayer, DataLinkLayer {
private:
  int _read_fd;
  int _write_fd;
  mac_addr_t _mac_addr;

public:
  Server(int read_fd, int write_fd, mac_addr_t mac_addr);

  void run_serv();
  void run_cli();

  // // implement the DataLinkLayer methods
  void send_tram(std::string *payload, ip_addr_t ip_next_hop, byte protocol);
  void receve_tram(bitstream_t *bitstream);

  // implement the PhysicalLayer methods
  void send_bytes(bitstream_t *bitstream);
  void receve_bytes();
};
