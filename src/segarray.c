//
// Created by James on 2018/3/6.
//

#include "segarray.h"


segarray_t segarray_construct(size_t node_size) {
  if (node_size == 0) return NULL;

  node_size = ROUND_UP_8(node_size); // 8 byte aligned

  size_t seg_blen = SEGARRAY_SEGMENT_BLEN;
  size_t seg_mask = ((0x0001ULL << seg_blen) - 1);
  size_t region_size = SEGARRAY_REGION_SIZE;

  // alloc some node
  segarray_t self = amalloc(sizeof(segarray_s) + node_size * (seg_mask + 1));
  if (self == NULL) return NULL;

  char **region = amalloc(sizeof(char*) * region_size);
  if (region == NULL) {
    afree(self);
    return NULL;
  }

  self->_node_segment = region;
  self->_node_segment[0] = self->buf;
  for (size_t i = 1; i < region_size; i++)
    self->_node_segment[i] = NULL;

  self->node_size = node_size;
  self->seg_blen = seg_blen;
  self->seg_mask = seg_mask;
  self->region_size = region_size;

  segarray_reset(self);

  return self;
}

bool segarray_destruct(segarray_t self) {
  if (self != NULL) {
    for (size_t i = 1; i < self->region_size; i++)
      afree(self->_node_segment[i]);
    afree(self->_node_segment);
    afree(self);
  }
  return true;
}

bool segarray_reset(segarray_t self) {
  if (self == NULL) return false;

  // for init memory
  char *segment = self->_node_segment[0];
  memset(segment, 0, self->node_size * (self->seg_mask + 1));

  self->region_cur = 1;
  self->len = 0;

  return true;
}

/*
 * if success, return is the size;
 * else, return is the space we have extend
 */
size_t segarray_extend(segarray_t self, size_t size) {
  size_t seg_size = self->seg_mask + 1;
  size_t capacity = self->region_cur * seg_size;
  size_t free = capacity - self->len;
  if (size > free) {
    // extend memory
    size_t t = (size - free) + seg_size; // must > seg_size
    while (t > seg_size) {
      if (self->region_cur >= self->region_size) {
        // extend region table
        self->_node_segment = arealloc(self->_node_segment, sizeof(char*) * (self->region_size << 1));
        for (size_t i = 0; i < self->region_size; i++) {
          self->_node_segment[self->region_size + i] = NULL;
        }
        self->region_size <<= 1;
      }

      if (self->_node_segment[self->region_cur] == NULL) {
        // alloc memory for segment
        self->_node_segment[self->region_cur] = amalloc(self->node_size * seg_size);
        if (self->_node_segment[self->region_cur] == NULL) {
          // alloc failed!
          break;
        }
      }

      // set memory with 0
      char *segment = self->_node_segment[self->region_cur];
      memset(segment, 0, self->node_size * seg_size);

      self->region_cur++;
      t -= seg_size;
    }

    capacity = self->region_cur * seg_size;
    free = capacity - self->len;

    if (free < size) size = free;
  }

  self->len += size;
  return size;
}

