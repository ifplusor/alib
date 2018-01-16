#include "dynapool.h"

dynapool_t dynapool_construct(size_t node_size) {
  if (node_size == 0) return NULL;

  // adjust size for store linked meta
  node_size = node_size < sizeof(deque_node_s) ? sizeof(deque_node_s) : node_size;
  node_size = ROUND_UP_8(node_size); // 8 byte aligned

  // alloc some node
  dynapool_t pool = amalloc(sizeof(dynapool_s) + node_size * DYNAPOOL_INIT_SIZE);
  if (pool == NULL) return NULL;

  pool->_nodepool[0] = pool->buf;
  for (size_t i = 1; i < DYNAPOOL_REGION_SIZE; i++)
    pool->_nodepool[i] = NULL;

  pool->node_size = node_size;
  pool->alloc_size = DYNAPOOL_INIT_SIZE;

  dynapool_reset(pool);

  return pool;
}

bool dynapool_destruct(dynapool_t pool) {
  if (pool != NULL) {
    for (size_t i = 1; i < DYNAPOOL_REGION_SIZE; i++)
      afree(pool->_nodepool[i]);
    afree(pool);
  }
  return true;
}

void *dynapool_alloc_node(dynapool_t pool) {
  deque_node_t deque = &pool->_sentinel;
  if (deque_empty(deque)) {
    if (pool->alloc_cur >= DYNAPOOL_REGION_SIZE) {
      fprintf(stderr, "%s(%d) - fatal: overflow in dynapool!", __FILE__, __LINE__);
      exit(-1);
    }

    // extend memory
    if (pool->_nodepool[pool->alloc_cur] == NULL) {
      // alloc double memory
      pool->_nodepool[pool->alloc_cur] = amalloc(pool->node_size * (pool->alloc_size << (pool->alloc_cur - 1)));
      if (pool->_nodepool[pool->alloc_cur] == NULL) return NULL;
    }

    // init memory
    char *region = pool->_nodepool[pool->alloc_cur];
    for (size_t j = 0; j < (pool->alloc_size << (pool->alloc_cur - 1)); j++) {
      void *node = region + pool->node_size * j;
      deque_push_back(deque, node, deque_node_s, forw);
    }

    pool->alloc_cur++;
  }
  return deque_pop_front(deque, deque_node_s, forw);
}

bool dynapool_free_node(dynapool_t pool, void *node) {
  if (pool == NULL || node == NULL) return false;
  deque_push_front(&pool->_sentinel, node, deque_node_s, forw);
  return true;
}

bool dynapool_reset(dynapool_t pool) {
  if (pool == NULL) return false;
  deque_node_t deque = &pool->_sentinel;
  deque_init(deque);

  // for init memory
  char *region = pool->_nodepool[0];
  for (size_t j = 0; j < DYNAPOOL_INIT_SIZE; j++) {
    void *node = region + pool->node_size * j;
    deque_push_back(deque, node, deque_node_s, forw);
  }
  pool->alloc_cur = 1;
}
