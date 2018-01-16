/**
 * dlnk.h - doubly linked list
 */

#ifndef _ALIB_DOUBLY_LINKED_H_
#define _ALIB_DOUBLY_LINKED_H_

#include "acom.h"


//
// doubly linked cycle list

struct doubly_linked_node;
typedef struct doubly_linked_node *dlnk_node_t;

typedef struct doubly_linked_node {
  dlnk_node_t forw, back;
} dlnk_node_s;

static inline void dlnk_init(dlnk_node_t sentinel) {
  sentinel->forw = sentinel->back = sentinel;
}

static inline void dlnk_insert(dlnk_node_t point, dlnk_node_t node) {
  node->forw = point->forw;
  node->forw->back = node;
  node->back = point;
  point->forw = node;
}

static inline dlnk_node_t dlnk_delete(dlnk_node_t node) {
  node->forw->back = node->back;
  node->back->forw = node->forw;
  return node;
}


//
// double-ended queue

typedef dlnk_node_s deque_node_s;
typedef dlnk_node_t deque_node_t;

#define deque_init(sentinel) \
  dlnk_init(sentinel)

#define deque_empty(deque) ((deque)->forw == (deque))

#define deque_push_back(deque, elem, type, member) \
  dlnk_insert((deque)->back, &(((type*) (elem))->member)) \

#define deque_push_front(deque, elem, type, member) \
  dlnk_insert((deque), &(((type*) (elem))->member)) \

#define deque_pop_back(deque, type, member) \
  (deque_empty(deque) ? NULL : container_of(dlnk_delete((deque)->back), type, member))

#define deque_pop_front(deque, type, member) \
  (deque_empty(deque) ? NULL : container_of(dlnk_delete((deque)->forw), type, member))

#define deque_delete(deque, elem, type, member) \
  dlnk_delete(&((type*) (elem))->member)

#define deque_peek_back(deque, type, member) \
  (deque_empty(deque) ? NULL : container_of((deque)->back, type, member))

#define deque_peek_front(deque, type, member) \
  (deque_empty(deque) ? NULL : container_of((deque)->forw, type, member))


#endif //_ALIB_DOUBLY_LINKED_H_
