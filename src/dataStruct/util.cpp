#include "util.h"
#include <iostream>

using namespace std;

void affiche_mac(const mac_addr_t mac) {
  std::cout << "MAC Address: ";
  for (int i = 0; i < 6; ++i) {
    std::cout << std::hex << (int)mac[i];
    if (i < 5) {
      std::cout << ":";
    }
  }
  std::cout << std::dec << std::endl;
}
