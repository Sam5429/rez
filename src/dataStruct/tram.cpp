#include "tram.h"
#include "util.h"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

Tram::Tram(mac_addr_t dest_mac, mac_addr_t src_mac, byte protocol,
           std::string payload) {
  memcpy(_dest_mac, dest_mac, sizeof(mac_addr_t));
  memcpy(_src_mac, src_mac, sizeof(mac_addr_t));
  _protocol = protocol;
  _payload = payload;
}

Tram::Tram(bitstream_t *tram) {
  memcpy(_dest_mac, tram->data(), sizeof(mac_addr_t));
  memcpy(_src_mac, tram->data() + sizeof(mac_addr_t), sizeof(mac_addr_t));
  _protocol = tram->data()[sizeof(mac_addr_t) * 2];
  // Calculate the start of the payload
  size_t payload_start = sizeof(mac_addr_t) * 2 + sizeof(byte);

  // Assuming _payload is a std::string
  _payload = std::string((const char *)tram->data() + payload_start,
                         tram->size() - payload_start);
}

bitstream_t *Tram::get_tram() {
  bitstream_t *tram = new bitstream_t;
  tram->reserve(sizeof(mac_addr_t) * 2 + sizeof(byte) + _payload.size());
  tram->insert(tram->end(), _dest_mac, _dest_mac + sizeof(mac_addr_t));
  tram->insert(tram->end(), _src_mac, _src_mac + sizeof(mac_addr_t));
  tram->push_back(_protocol);
  tram->insert(tram->end(), _payload.begin(), _payload.end());
  return tram;
}

std::string Tram::get_payload() { return _payload; }

std::string Tram::to_string() {
  std::ostringstream oss;
  oss << "Destination MAC: ";
  for (size_t i = 0; i < sizeof(mac_addr_t); ++i) {
    oss << std::hex << (int)_dest_mac[i]
        << (i < sizeof(mac_addr_t) - 1 ? ":" : "");
  }
  oss << "\nSource MAC: ";
  for (size_t i = 0; i < sizeof(mac_addr_t); ++i) {
    oss << std::hex << (int)_src_mac[i]
        << (i < sizeof(mac_addr_t) - 1 ? ":" : "");
  }
  oss << "\nProtocol: " << std::dec << (int)_protocol;
  oss << "\nPayload: ";
  oss << _payload;
  // for (size_t i = 0; i < _payload_size; ++i) {
  //   oss << std::hex << (int)_payload[i] << " ";
  // }
  return oss.str();
}
