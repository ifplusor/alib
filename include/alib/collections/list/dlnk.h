/**
 * dlnk.h - doubly linked list
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#ifndef __ALIB_DOUBLY_LINKED_H__
#define __ALIB_DOUBLY_LINKED_H__

#include "../../acom.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//
// doubly linked cycle list

struct _alib_doubly_linked_node_;
typedef struct _alib_doubly_linked_node_ dlnk_node_s;
typedef dlnk_node_s* dlnk_node_t;

// design for embedded in custom struct
struct _alib_doubly_linked_node_ {
  dlnk_node_t forw, back;
};

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

#ifdef __cplusplus
};
#endif

#endif  // __ALIB_DOUBLY_LINKED_H__
