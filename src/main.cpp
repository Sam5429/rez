#include "dataStruct/util.h"
#include "server.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <thread>
#include <unistd.h>
using namespace std;

int main() {
  int pipe1[2], pipe2[2]; // [0] pour lire et [1] pour écrire

  if (pipe(pipe1) == -1 || pipe(pipe2) == -1) {
    perror("pipe creation\n");
    exit(1);
  }

  cout << "pipe created" << endl;

  // créer le 1er thread
  mac_addr_t mac1 = 0x0123456789a1;
  ip_addr_t ip1 = 0x012341;

  Server server1(pipe1[0], pipe2[1], mac1, ip1);
  std::thread tserver_serv(&Server::run_serv, &server1);

  // attend pour évité les soucis d'ordo
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // créer le 2eme thread
  mac_addr_t mac2 = 0x0123456789a2;
  ip_addr_t ip2 = 0x012342;

  Server server2(pipe2[0], pipe1[1], mac2, ip2);
  std::thread tserver_cli(&Server::run_cli, &server2);

  tserver_cli.join();
  tserver_serv.join();

  cout << "end of main" << endl;
  return 0;
}
