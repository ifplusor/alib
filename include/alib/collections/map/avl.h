/**
 * avl.h - AVL tree
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#ifndef __ALIB_AVL_H__
#define __ALIB_AVL_H__

#include "../../acom.h"

#ifndef DEBUG_AVL
#define DEBUG_AVL 0
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

struct _avl_node_;
typedef struct _avl_node_ avl_node_s;
typedef avl_node_s* avl_node_t;

struct _avl_node_ {
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
  sptr_t bf;  // balance factor
};

typedef sptr_t (*avl_compare_f)(avl_node_t node, void* key);
typedef avl_node_t (*avl_replace_func)(avl_node_t old, avl_node_t new);

typedef struct _avl_ {
  // fields
  size_t len;
  avl_node_t root;

  // delegate
  avl_compare_f compare;
  avl_replace_func replace;
} avl_s, *avl_t;

avl_t avl_construct(avl_compare_f compare);
bool avl_destruct(avl_t self);

void avl_reset(avl_t self);
avl_node_t avl_insert(avl_t self, void* key, avl_node_t node);
avl_node_t avl_replace(avl_t self, void* key, avl_node_t node);
avl_node_t avl_delete(avl_t self, void* key);
avl_node_t avl_search(avl_t self, void* key);
avl_node_t avl_search_ext(avl_t self, void* key, avl_compare_f compare_func);

typedef sptr_t (*avl_walk_road_f)(avl_node_t node, void* arg);
typedef void (*avl_walk_op_f)(avl_node_t node, void* arg);

void avl_walk_in_order(avl_t self, avl_walk_road_f walk_road, avl_walk_op_f walk_op, void* road_arg, void* op_arg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  // __ALIB_AVL_H__
