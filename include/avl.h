//
// Created by james on 9/28/17.
//

#ifndef _ALIB_AVL_H_
#define _ALIB_AVL_H_

#include "acom.h"

#define DEBUG_AVL 0

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct avl_node;
typedef struct avl_node *avl_node_t;

typedef struct avl_node {
#if DEBUG_AVL
  avl_node_t left, right, parent;
#define avl_left left
#define avl_right right
#define avl_parent parent
#else
  avl_node_t link[3];
#define avl_left link[0]
#define avl_right link[1]
#define avl_parent link[2]
#endif
  sptr_t bf; // balance factor
} avl_node_s;

typedef sptr_t (*avl_compare_func)(avl_node_t node, void *key);
typedef avl_node_t (*avl_replace_func)(avl_node_t old, avl_node_t new);

typedef struct avl {
  size_t len;
  avl_node_t root;
  avl_compare_func compare;
  avl_replace_func replace;
} avl_s, *avl_t;

avl_t avl_construct(avl_compare_func compare);
bool avl_destruct(avl_t self);

void avl_reset(avl_t self);
avl_node_t avl_insert(avl_t self, void *key, avl_node_t node);
avl_node_t avl_replace(avl_t self, void *key, avl_node_t node);
avl_node_t avl_delete(avl_t self, void *key);
avl_node_t avl_search(avl_t self, void *key);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_ALIB_AVL_H_
