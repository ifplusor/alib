/**
 * dynapool.h - dynamic node pool
 *
 * @author James Yin <ywhjames@hotmail.com>
 */

#ifndef _ALIB_DYNAPOOL_H_
#define _ALIB_DYNAPOOL_H_

#include "dlnk.h"

// init size, it will be alloc when dynapool construct, and 0-region point it.
#if !DYNAPOOL_INIT_SIZE
#define DYNAPOOL_INIT_SIZE 32
#endif

// region size, which limit max size.
#if !DYNAPOOL_REGION_SIZE
#define DYNAPOOL_REGION_SIZE 60
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef struct alib_dynapool {
  deque_node_s _free_list; // free node list
  size_t node_size;
  size_t alloc_size;
  size_t alloc_cur; // region cursor in _node_pool
  void* _node_pool[DYNAPOOL_REGION_SIZE];
  char buf[0];
} dynapool_s, *dynapool_t;

dynapool_t dynapool_construct(size_t node_size);
#define dynapool_construct_with_type(type) dynapool_construct(sizeof(type))

bool dynapool_destruct(dynapool_t self);

void *dynapool_alloc_node(dynapool_t self);
bool dynapool_free_node(dynapool_t self, void *node);

bool dynapool_reset(dynapool_t self);


#ifdef __cplusplus
};
#endif

#endif //_ALIB_DYNAPOOL_H_