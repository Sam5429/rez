#include "../dataStruct/util.h"

/* 
 * Address Resolution Protocol
 */
class ARP {
public:
  /*
   * send the data on the link
   */
  virtual void send_bytes(bitstream_t *bitstream) = 0;

  /*
   * read the data from the link
   */
  virtual void receve_bytes() = 0;
};
