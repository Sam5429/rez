#include "../dataStruct/tram.h"
#include "../dataStruct/util.h"
#include <string>

class DataLinkLayer {
public:
  /*
   * create and give the tram to the next layer
   */
  virtual void send_tram(std::string *payload, ip_addr_t ip_next_hop,
                         byte protocol) = 0;

  /*
   * treat the tram and give the payload to the next layer
   */
  virtual void receve_tram(bitstream_t *bitstream) = 0;
};
