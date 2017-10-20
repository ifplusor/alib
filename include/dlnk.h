//
// Created by james on 10/13/17.
//

#ifndef _ALIB_DOUBLY_LINKED_H_
#define _ALIB_DOUBLY_LINKED_H_

#include "acom.h"

struct doubly_linked_node;
typedef struct doubly_linked_node *dlnk_node_t;

typedef struct doubly_linked_node {
  dlnk_node_t forw, back;
} dlnk_node_s;

void dlnk_init(dlnk_node_t dentinel);
void dlnk_insert(dlnk_node_t point, dlnk_node_t node);
void dlnk_delete(dlnk_node_t node);

typedef dlnk_node_s deque_node_s;
typedef dlnk_node_t deque_node_t;

#define deque_init(sentinel) ({ \
  dlnk_init(sentinel); \
})

#define deque_empty(deque) ((deque)->forw == (deque))

#define deque_push_back(deque, elem, type, member) ({ \
  dlnk_insert((deque)->back, &((type*) (elem))->member); \
})

#define deque_push_front(deque, elem, type, member) ({ \
  dlnk_insert((deque), &((type*) (elem))->member); \
})

#define deque_pop_back(deque, type, member) ({ \
  deque_empty(deque) ? NULL : ({ \
    dlnk_node_t node = (deque)->back; \
    dlnk_delete(node); \
    container_of(node, type, member); \
  }); \
})

#define deque_pop_front(deque, type, member) ({ \
  deque_empty(deque) ? NULL : ({ \
    dlnk_node_t node = (deque)->forw; \
    dlnk_delete(node); \
    container_of(node, type, member); \
  }); \
})

#define deque_delete(deque, elem, type, member) ({ \
  dlnk_delete(&((type*) (elem))->member); \
})

#define deque_peek_back(deque, type, member) ({ \
  deque_empty(deque) ? NULL : container_of((deque)->back, type, member); \
})

#define deque_peek_front(deque, type, member) ({ \
  deque_empty(deque) ? NULL : container_of((deque)->forw, type, member); \
})

#endif //_ALIB_DOUBLY_LINKED_H_
