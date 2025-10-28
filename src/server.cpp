#include "server.h"
#include "dataStruct/tram.h"
#include "dataStruct/tram_arp.h"
#include "dataStruct/util.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>
#include <unordered_map>

Server::Server(int read_fd, int write_fd, mac_addr_t mac_addr,
               ip_addr_t ip_addr)
    : _read_fd(read_fd), _write_fd(write_fd) {
  _mac_addr = mac_addr;
  _ip_addr = ip_addr;
  _arp_table = std::unordered_map<ip_addr_t, mac_addr_t>();
  std::cout << "Server created :" << std::endl;
  std::cout << "  MAC address: " << mac_to_string(_mac_addr) << std::endl;
  std::cout << "  IP address: " << ip_to_string(_ip_addr) << std::endl;
}

mac_addr_t Server::search_arp_table(ip_addr_t ip_addr) {
  if (_arp_table.find(ip_addr) != _arp_table.end()) {
    return _arp_table[ip_addr];
  } else {
    return 0;
  }
}

void Server::run_serv() {
  this->receve_bytes();
  this->receve_bytes();
}

void Server::run_cli() {
  // Envoyer une requet
  bitstream_t message;
  const char *str = "coucou";
  message.insert(message.end(), str, str + strlen(str));

  byte protocol[2];
  protocol[0] = 0x08;
  protocol[1] = 0x00;

  ip_addr_t ip1 = 0x012341;
  send_tram(&message, ip1, protocol);
}

void Server::send_bytes(bitstream_t *bitstream) {
  size_t size = bitstream->size();
  // send the size of the bitstream first
  if (write(_write_fd, &size, sizeof(size_t)) <= 0) {
    std::perror("Error writing size to the pipe");
    exit(-4);
  };

  int bytes_written = write(_write_fd, bitstream->data(), bitstream->size());
  if (bytes_written <= 0) {
    std::perror("Error writing to the pipe");
    exit(-4);
  }
  delete bitstream;
}

void Server::receve_bytes() {
  // Lire la taille des données avant de lire les données elles-mêmes
  size_t data_size = 0;
  ssize_t size_read = read(_read_fd, &data_size, sizeof(size_t));
  if (size_read != sizeof(size_t)) {
    std::perror("Error reading size from the pipe");
    exit(-4);
  }

  bitstream_t *buffer = new bitstream_t;
  const size_t BUFFER_SIZE = 256; // toute taille de buff supérieur à 1
  char temp_buffer[BUFFER_SIZE];
  ssize_t bytes_read;
  buffer->reserve(data_size);

  size_t total_read = 0;
  while (total_read < data_size) {
    bytes_read = read(_read_fd, temp_buffer,
                      std::min(BUFFER_SIZE, data_size - total_read));
    if (bytes_read <= 0) {
      std::perror("Error reading from the pipe");
      exit(-4);
    }
    buffer->insert(buffer->end(), temp_buffer, temp_buffer + bytes_read);
    total_read += bytes_read;
  }

  std::cout << "Buffer contents (hex)(" << bytes_read << "): ";
  for (auto c : *buffer) {
    printf("%02x ", static_cast<unsigned char>(c));
  }
  std::cout << std::endl;

  receve_tram(buffer);
}

void Server::send_tram(bitstream_t *payload, ip_addr_t ip_next_hop,
                       byte protocol[2]) {
  mac_addr_t broadcast_mac = -1;
  if (protocol[0] == 0x08 && protocol[1] == 0x06) {

    Tram *tram = new Tram(broadcast_mac, _mac_addr, protocol, *payload);
    this->send_bytes(tram->get_tram());
    delete payload;
    delete tram;
  } else if (protocol[0] == 0x08 && protocol[1] == 0x00) {

    mac_addr_t mac_dest = search_arp_table(ip_next_hop);
    if (mac_dest == 0) {
      std::cout << "MAC address not found in ARP table. Sending ARP request."
                << std::endl;
      send_arp_request(ip_next_hop);
      this->receve_bytes();
    }

    mac_dest = search_arp_table(ip_next_hop);
    if (mac_dest == 0) {
      std::cerr << "Error: MAC address still not found in ARP table after ARP "
                   "request."
                << std::endl;
      exit(-3);
    }

    Tram *tram = new Tram(mac_dest, _mac_addr, protocol, *payload);
    this->send_bytes(tram->get_tram());
    delete tram;
  } else {
    std::cout << "Unknown protocol." << std::endl;
    exit(-2);
  }
}

void Server::receve_tram(bitstream_t *bitstream) {
  Tram *new_tram = new Tram(bitstream);
  delete bitstream;

  if (new_tram->_dest_mac != _mac_addr &&
      new_tram->_dest_mac != (mac_addr_t)-1) {
    // not for us
    delete new_tram;
    return;
  }

  if (new_tram->_protocol[0] == 0x08 && new_tram->_protocol[1] == 0x06) {
    // ARP protocol
    bitstream_t *arp_payload =
        new bitstream_t(new_tram->_payload.begin(), new_tram->_payload.end());
    receve_arp(arp_payload);
  } else {
    std::string payload_str(new_tram->_payload.begin(),
                            new_tram->_payload.end());
    std::cout << "Contenu (string): " << payload_str << std::endl;
  }
  delete new_tram;
}

void Server::send_arp_request(ip_addr_t ip_dest) {
  mac_addr_t unknown_mac = 0;

  byte protocol[2]; // ARP protocol
  protocol[0] = 0x08;
  protocol[1] = 0x06;

  // create arp request tram
  TramARP *tram_arp =
      new TramARP(ARP_REQUEST, _mac_addr, _ip_addr, unknown_mac, ip_dest);

  // send the tram
  send_tram(tram_arp->get_tram_arp(), 0, protocol);
  delete tram_arp;
}

void Server::receve_arp(bitstream_t *arp_body) {
  TramARP arp = TramARP(arp_body);
  delete arp_body;
  if (arp._operation == ARP_REQUEST) {
    if (arp._ip_dest == _ip_addr) {
      send_arp_response(arp._ip_dest, arp._mac_dest);
    }
  } else if (arp._operation == ARP_RESPONSE) {
    _arp_table[arp._ip_src] = arp._mac_src;
  } else {
    std::cerr << "Unknown ARP operation: " << (int)arp._operation << std::endl;
    exit(-2);
  }
}

void Server::send_arp_response(ip_addr_t ip_dest, mac_addr_t mac_dest) {
  TramARP tram_arp =
      TramARP(ARP_RESPONSE, _mac_addr, _ip_addr, mac_dest, ip_dest);

  byte protocol[2]; // ARP protocol
  protocol[0] = 0x08;
  protocol[1] = 0x06;

  send_tram(tram_arp.get_tram_arp(), 0, protocol);
}
