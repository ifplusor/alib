/**
 * dynapool.c
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#include "alib/memory/dynapool.h"

dynapool_t dynapool_construct(size_t node_size) {
  if (node_size == 0) {
    return NULL;
  }

  // adjust size for store linked meta
  node_size = node_size < sizeof(deque_node_s) ? sizeof(deque_node_s) : node_size;
  node_size = ROUND_UP_8(node_size);  // 8 byte aligned

  // alloc some node
  dynapool_t self = amalloc(sizeof(dynapool_s) + node_size * DYNAPOOL_INIT_SIZE);
  if (self == NULL) {
    return NULL;
  }

  self->_node_pool[0] = self->buf;
  for (size_t i = 1; i < DYNAPOOL_REGION_SIZE; i++) {
    self->_node_pool[i] = NULL;
  }

  self->node_size = node_size;
  self->alloc_size = DYNAPOOL_INIT_SIZE;

  dynapool_reset(self);

  return self;
}

bool dynapool_destruct(dynapool_t self) {
  if (self != NULL) {
    for (size_t i = 1; i < DYNAPOOL_REGION_SIZE; i++) {
      afree(self->_node_pool[i]);
    }
    afree(self);
  }
  return true;
}

void* dynapool_alloc_node(dynapool_t self) {
  deque_node_t deque = &self->_free_list;
  if (deque_empty(deque)) {
    if (self->alloc_cur >= DYNAPOOL_REGION_SIZE) {
      ALOG_FATAL("overflow in dynapool!");
    }

    size_t new_size = self->alloc_size << (self->alloc_cur - 1);

    // extend memory
    if (self->_node_pool[self->alloc_cur] == NULL) {
      // alloc memory to double
      self->_node_pool[self->alloc_cur] = amalloc(self->node_size * new_size);
      if (self->_node_pool[self->alloc_cur] == NULL) {
        return NULL;
      }
    }

    // init memory
    char* pool = self->_node_pool[self->alloc_cur];
    for (size_t j = 0; j < new_size; j++) {
      void* node = pool + self->node_size * j;
      deque_push_back(deque, node, deque_node_s, forw);
    }

    self->alloc_cur++;
  }
  return deque_pop_front(deque, deque_node_s, forw);
}

bool dynapool_free_node(dynapool_t self, void* node) {
  if (self == NULL || node == NULL) {
    return false;
  }
  deque_push_front(&self->_free_list, node, deque_node_s, forw);
  return true;
}

// reset nodepool state, but not free memory
bool dynapool_reset(dynapool_t self) {
  if (self == NULL) {
    return false;
  }
  deque_node_t deque = &self->_free_list;
  deque_init(deque);

  // for init memory
  char* pool = self->_node_pool[0];
  for (size_t j = 0; j < DYNAPOOL_INIT_SIZE; j++) {
    void* node = pool + self->node_size * j;
    deque_push_back(deque, node, deque_node_s, forw);
  }
  self->alloc_cur = 1;

  return true;
}
