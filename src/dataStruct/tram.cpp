#include "tram.h"
#include "util.h"
#include <cstddef>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>

Tram::Tram(mac_addr_t dest_mac, mac_addr_t src_mac, byte protocol[2],
           bitstream_t payload) {
  _dest_mac = dest_mac;
  _src_mac = src_mac;

  memcpy(_protocol, protocol, sizeof(byte[2]));

  _payload = payload;
}

Tram::Tram(bitstream_t *tram) {
  std::cout << "bitstream_t contents: ";
  for (size_t i = 0; i < tram->size(); ++i) {
    std::cout << std::hex << std::uppercase << (int)(*tram)[i] << " ";
  }
  std::cout << std::dec << std::endl;
  _dest_mac = *reinterpret_cast<const mac_addr_t *>(tram->data());
  _src_mac =
      *reinterpret_cast<const mac_addr_t *>(tram->data() + sizeof(mac_addr_t));

  memcpy(_protocol, tram->data() + 2 * sizeof(mac_addr_t), sizeof(byte[2]));

  size_t payload_start = sizeof(mac_addr_t) * 2 + sizeof(byte[2]);
  _payload.insert(_payload.end(), tram->data() + payload_start,
                  tram->data() + tram->size());
}

bitstream_t *Tram::get_tram() {
  bitstream_t *tram = new bitstream_t;
  tram->reserve(sizeof(mac_addr_t) * 2 + sizeof(byte[2]) + _payload.size());

  tram->insert(tram->end(), reinterpret_cast<byte *>(&_dest_mac),
               reinterpret_cast<byte *>(&_dest_mac) + sizeof(mac_addr_t));
  tram->insert(tram->end(), reinterpret_cast<byte *>(&_src_mac),
               reinterpret_cast<byte *>(&_src_mac) + sizeof(mac_addr_t));

  tram->insert(tram->end(), _protocol, _protocol + sizeof(byte[2]));

  tram->insert(tram->end(), _payload.begin(), _payload.end());
  return tram;
}

std::string Tram::to_string() {
  std::ostringstream oss;
  oss << "Destination MAC: " << mac_to_string(_dest_mac) << "\n";
  oss << "Source MAC: " << mac_to_string(_src_mac) << "\n";
  oss << "Protocol: " << std::hex << std::uppercase << (int)_protocol[0] << " "
      << (int)_protocol[1] << "\n";
  oss << "Payload: ";
  for (size_t i = 0; i < _payload.size(); ++i) {
    oss << std::hex << std::uppercase << (int)_payload[i];
    if (i != _payload.size() - 1) {
      oss << " ";
    }
  };
  oss << "\n";
  // for (size_t i = 0; i < _payload_size; ++i) {
  //   oss << std::hex << (int)_payload[i] << " ";
  // }
  return oss.str();
}
