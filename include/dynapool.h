#ifndef _ALIB_NODE_POOL_H_
#define _ALIB_NODE_POOL_H_

#include "dlnk.h"

#if !DYNAPOOL_INIT_SIZE
#define DYNAPOOL_INIT_SIZE 32
#endif

#if !DYNAPOOL_REGION_SIZE
#define DYNAPOOL_REGION_SIZE 60
#endif

typedef struct dyna_nodepool {
  deque_node_s _sentinel;
  size_t node_size;
  size_t alloc_size;
  size_t alloc_cur;
  void* _nodepool[DYNAPOOL_REGION_SIZE];
  char buf[0];
} dynapool_s, *dynapool_t;

dynapool_t dynapool_construct(size_t node_size);
#define dynapool_construct_with_type(type) dynapool_construct(sizeof(type))

bool dynapool_destruct(dynapool_t pool);

void *dynapool_alloc_node(dynapool_t pool);
bool dynapool_free_node(dynapool_t pool, void *node);

bool dynapool_reset(dynapool_t pool);

#endif //_ALIB_NODE_POOL_H_