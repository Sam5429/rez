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
  mac_addr_t mac1;
  mac1[0] = 0x01;
  mac1[1] = 0x23;
  mac1[2] = 0x45;
  mac1[3] = 0x67;
  mac1[4] = 0x89;
  mac1[5] = 0xa1;
  Server server1(pipe1[0], pipe2[1], mac1);
  std::thread tserver_serv(&Server::run_serv, &server1);

  cout << "first serv created" << endl;

  // attend pour évité les soucis d'ordo
  std::this_thread::sleep_for(std::chrono::seconds(1));

  // créer le 2eme thread
  mac_addr_t mac2;
  mac2[0] = 0x01;
  mac2[1] = 0x23;
  mac2[2] = 0x45;
  mac2[3] = 0x67;
  mac2[4] = 0x89;
  mac2[5] = 0xa2;
  Server server2(pipe2[0], pipe1[1], mac2);
  std::thread tserver_cli(&Server::run_cli, &server2);

  cout << "second serv created" << endl;

  tserver_cli.join();
  tserver_serv.join();
}
