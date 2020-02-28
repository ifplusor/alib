/**
 * prique.h - priority queue
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#ifndef __ALIB_PRIORITY_QUEUE_H__
#define __ALIB_PRIORITY_QUEUE_H__

#include "../../acom.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef sptr_t (*prique_compare_f)(void* node1, void* node2);

typedef struct _alib_priority_queue_ {
  void** data;
  size_t len, capacity;
  prique_compare_f compare_func;
} prique_s, *prique_t;

static inline size_t prique_size(prique_t self) {
  return self->len;
}

prique_t prique_construct(prique_compare_f compare_func);
void prique_destruct(prique_t self);

void* prique_peek(prique_t self);
void prique_push(prique_t self, void* value);
void* prique_pop(prique_t self);

#ifdef __cplusplus
};
#endif

#endif  // __ALIB_PRIORITY_QUEUE_H__
