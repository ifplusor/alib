//
// Created by james on 10/17/17.
//

#ifndef _ACTRIE_AOBJ_H_
#define _ACTRIE_AOBJ_H_

#include "taggedpointer.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/*
 * memory map:
 *   +-----------+
 *   |  padding  |
 *   +-----------+
 *   |   ameta   |
 *   +-----------+
 *   | aobj_meta |
 *   +-----------+
 *   |  members  |
 *   +-----------+
 */

typedef void *aobj;

typedef aobj (*aobj_init_func)(void *ptr, void *data);
typedef void (*aobj_clean_func)(aobj id);

typedef struct aobj_meta {
  uint32_t isa;
  uint32_t refcnt;
  aobj_clean_func clean;
  char object[0];
} aobj_meta_s, *aobj_meta_t;

#define aobj_meta(ptr) ((aobj_meta_t)((char*)(ptr) - sizeof(aobj_meta_s)))

aobj _aobj_alloc(size_t size, aobj_init_func init, void *data);
#define aobj_alloc(type, init) _aobj_alloc(sizeof(type), init, NULL)
#define aobj_alloc_with(type, init, data) _aobj_alloc(sizeof(type), init, data)
#define aobj_alloc_with_ex(type, init, data, size) \
  _aobj_alloc(sizeof(type) + (size), init, data)

aobj aobj_init(void *ptr, aobj_meta_t meta);

void aobj_retain(aobj id);
void aobj_release(aobj id);

#define _retain(id) aobj_retain(id)
#define _release(id) aobj_release(id)

#define _(type, id, message, args...) __(id, type##_##message, ##args)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_ACTRIE_AOBJ_H_
