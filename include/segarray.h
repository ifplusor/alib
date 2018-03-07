/**
 * segarray.h - segment dynamic array
 *
 * @author James Yin <ywhjames@hotmail.com>
 */

#ifndef _ALIB_SEGARRAY_H_
#define _ALIB_SEGARRAY_H_

#include "acom.h"

// init segment size, it will be alloc when segarray construct, and 0-region point it.
#if !SEGARRAY_SEGMENT_BLEN
#define SEGARRAY_SEGMENT_BLEN 18
#endif

// init region size
#if !SEGARRAY_REGION_SIZE
#define SEGARRAY_REGION_SIZE 8
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct alib_segarray {
  size_t seg_blen;
  size_t seg_mask;

  char **_node_segment;
  size_t node_size;

  size_t region_cur; // region cursor in _node_segment
  size_t region_size;

  size_t len;

  char buf[0];
} segarray_s, *segarray_t;

static inline size_t segarray_size(segarray_t self) {
  return self->len;
}

static inline void *segarray_access(segarray_t self, size_t index) {
  register size_t region = index >> self->seg_blen;
  register size_t position = index & self->seg_mask;
  return (void *) &self->_node_segment[region][self->node_size * position];
}

static inline void *segarray_access_s(segarray_t self, size_t index) {
  if (index >= self->len) {
    return NULL;
  }
  register size_t region = index >> self->seg_blen;
  register size_t position = index & self->seg_mask;
  return (void *) &self->_node_segment[region][self->node_size * position];
}

segarray_t segarray_construct(size_t node_size);
#define segarray_construct_with_type(type) segarray_construct(sizeof(type))

bool segarray_destruct(segarray_t self);

bool segarray_reset(segarray_t self);

size_t segarray_extend(segarray_t self, size_t size);

#ifdef __cplusplus
};
#endif

#endif //_ALIB_SEGARRAY_H_
