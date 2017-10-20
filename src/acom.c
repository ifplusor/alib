//
// Created by james on 6/16/17.
//

#include <sys/timeb.h>

#include "acom.h"

const size_t POOL_REGION_SIZE = REGION_SIZE;
const size_t POOL_POSITION_SIZE = POSITION_MASK + 1;

sint64_t current_milliseconds() {
  struct timeb t;
  ftime(&t);
  sint64_t ret = t.time;
  ret *= 1000;
  ret += t.millitm;
  return ret;
}
