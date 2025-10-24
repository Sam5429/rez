#include "server.h"
#include "dataStruct/tram.h"
#include "dataStruct/util.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <unistd.h>

Server::Server(int read_fd, int write_fd, mac_addr_t mac_addr)
    : _read_fd(read_fd), _write_fd(write_fd) {
  memcpy(_mac_addr, mac_addr, sizeof(mac_addr_t));
}

void Server::run_serv() {
  // Lire la réponse
  this->receve_bytes();

  // Envoyer une réponse
  std::string message = "coucou";

  this->send_tram(&message, 0, 0);
}

void Server::run_cli() {
  // Envoyer une requet
  std::string message = "coucou";

  this->send_tram(&message, 0, 0);

  // Lire la réponse
  this->receve_bytes();
}

void Server::send_tram(std::string *payload, ip_addr_t ip_next_hop,
                       byte protocol) {
  // read the ip in the paquet and search in the ARP table for the next link
  mac_addr_t mac_dest;
  mac_dest[0] = 0x01;
  mac_dest[1] = 0x23;
  mac_dest[2] = 0x45;
  mac_dest[3] = 0x67;
  mac_dest[4] = 0x89;
  mac_dest[5] = 0xa1;

  Tram *tram = new Tram(mac_dest, _mac_addr, 0, *payload);
  this->send_bytes(tram->get_tram());
}

void Server::receve_tram(bitstream_t *bitstream) {
  Tram *new_tram = new Tram(bitstream);
  std::cout << new_tram->to_string() << std::endl;

  // treat the receved tram and return the datas
  std::string payload = new_tram->get_payload();
  std::cout << payload << std::endl;
}

void Server::send_bytes(bitstream_t *bitstream) {
  int bytes_written = write(_write_fd, bitstream->data(), bitstream->size());
  int eof = '\0';
  write(_write_fd, &eof, 1);
  if (bytes_written <= 0) {
    std::perror("Error writing to the pipe");
    exit(-4);
  }
}

void Server::receve_bytes() {
  bitstream_t *buffer = new bitstream_t;
  const size_t BUFFER_SIZE = 256; // toute taille de buff supérieur à 1
  char temp_buffer[BUFFER_SIZE];
  int bytes_read;

  while ((bytes_read = read(_read_fd, temp_buffer, BUFFER_SIZE - 1)) > 0) {
    buffer->insert(buffer->end(), temp_buffer, temp_buffer + bytes_read);
    if (temp_buffer[(bytes_read) % BUFFER_SIZE - 1] == '\0') {
      break;
    }
  }

  if (bytes_read < 0) {
    std::perror("Error reading from the pipe");
    exit(-4);
  }

  receve_tram(buffer);
}
