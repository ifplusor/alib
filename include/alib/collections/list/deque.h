/**
 * deque.h - double-ended queue
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#ifndef __ALIB_DOUBLEENDED_QUEUE_H__
#define __ALIB_DOUBLEENDED_QUEUE_H__

#include "dlnk.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

//
// double-ended queue

typedef dlnk_node_s deque_node_s;
typedef dlnk_node_t deque_node_t;

#define deque_init(sentinel) dlnk_init(sentinel)

#define deque_empty(deque) ((deque)->forw == (deque))

#define deque_push_back(deque, elem, type, member) dlnk_insert((deque)->back, &(((type*)(elem))->member))

#define deque_push_front(deque, elem, type, member) dlnk_insert((deque), &(((type*)(elem))->member))

#define deque_pop_back(deque, type, member) \
  (deque_empty(deque) ? NULL : container_of(dlnk_delete((deque)->back), type, member))

#define deque_pop_front(deque, type, member) \
  (deque_empty(deque) ? NULL : container_of(dlnk_delete((deque)->forw), type, member))

#define deque_delete(deque, elem, type, member) dlnk_delete(&((type*)(elem))->member)

#define deque_peek_back(deque, type, member) (deque_empty(deque) ? NULL : container_of((deque)->back, type, member))

#define deque_peek_front(deque, type, member) (deque_empty(deque) ? NULL : container_of((deque)->forw, type, member))

#ifdef __cplusplus
};
#endif

#endif  // __ALIB_DOUBLEENDED_QUEUE_H__
