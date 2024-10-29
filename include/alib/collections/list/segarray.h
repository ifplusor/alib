/**
 * segarray.h - segment array with dynamic memory
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#ifndef __ALIB_SEGMENT_ARRAY_H__
#define __ALIB_SEGMENT_ARRAY_H__

#include "../../acom.h"

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

struct _alib_segarray_;
typedef struct _alib_segarray_ segarray_s;
typedef segarray_s* segarray_t;

typedef void (*segarray_init_segment_f)(segarray_t self, void* segment, size_t seg_size, size_t start_index, void* arg);

struct _alib_segarray_ {
  size_t seg_blen;
  size_t seg_mask;

  uint8_t** _node_segment;
  size_t node_size;

  size_t region_cur;  // region cursor in _node_segment
  size_t region_size;

  size_t len;

  segarray_init_segment_f init_segment_func;
  void* init_segment_arg;

  uint8_t buf[0];
};

static inline size_t segarray_size(segarray_t self) {
  return self->len;
}

static inline void* segarray_access(segarray_t self, size_t index) {
  register size_t region = index >> self->seg_blen;
  register size_t position = index & self->seg_mask;
  return (void*)&self->_node_segment[region][self->node_size * position];
}

static inline void* segarray_access_s(segarray_t self, size_t index) {
  if (index >= self->len) {
    return NULL;
  }
  return segarray_access(self, index);
}

segarray_t segarray_construct(size_t node_size, segarray_init_segment_f init_segment_func, void* init_segment_arg);
#define segarray_construct_with_type(type) segarray_construct(sizeof(type), NULL, NULL)

segarray_t segarray_construct_ext(size_t node_size,
                                  size_t seg_blen,
                                  size_t region_size,
                                  segarray_init_segment_f init_segment_func,
                                  void* init_segment_arg);
#define segarray_construct_with_type_ext(type, seg_blen, region_size) \
  segarray_construct_ext(sizeof(type), seg_blen, region_size, NULL, NULL)

bool segarray_destruct(segarray_t self);

bool segarray_reset(segarray_t self);

size_t segarray_extend(segarray_t self, size_t size);

#ifdef __cplusplus
};
#endif

#endif  // __ALIB_SEGMENT_ARRAY_H__
