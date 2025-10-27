#include "util.h"
#include <iostream>
#include <sstream>

using namespace std;

std::string mac_to_string(const mac_addr_t mac_addr) {
  std::ostringstream oss = std::ostringstream();
  for (int i = 5; i >= 0; --i) {
    oss << std::hex << ((mac_addr >> (i * 8)) & 0xFF);
    if (i > 0) {
      oss << ":";
    }
  }
  oss << std::dec;
  return oss.str();
}

std::string ip_to_string(const ip_addr_t ip_addr) {
  std::ostringstream oss;
  for (int i = 3; i >= 0; --i) {
    oss << ((ip_addr >> (i * 8)) & 0xFF);
    if (i > 0) {
      oss << ".";
    }
  }
  return oss.str();
}
