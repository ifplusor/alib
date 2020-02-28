/**
 * acom.c
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#include "alib/acom.h"

#include <sys/timeb.h>

const size_t POOL_REGION_SIZE = REGION_SIZE;
const size_t POOL_POSITION_SIZE = POSITION_MASK + 1;

sint64_t current_milliseconds() {
  struct timeb t;
  ftime(&t);
  sint64_t ret = (sint64_t)t.time;
  ret *= 1000;
  ret += t.millitm;
  return ret;
}
