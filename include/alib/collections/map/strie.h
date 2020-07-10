/**
 * strie.h - thread security trie
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#ifndef __ALIB_STRIE_H__
#define __ALIB_STRIE_H__

#include "../../acom.h"
#include "../../memory/dynapool.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct strie_node {
  int rwlock;  // if rwlock < 0, some one writing; if rwlock > 0, some reading.
  void* data;
  void* children[256];
} strie_node_s, *strie_node_t;

//typedef struct strie {
//} strie_s, *strie_t;
//
//void* strie_insert(strie_t self, uchar* key, uint32_t len, void* data);
//void* strie_delete(strie_t self, uchar* key, uint32_t len);
//void* strie_search(strie_t self, uchar* key, uint32_t len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  // __ALIB_STRIE_H__
