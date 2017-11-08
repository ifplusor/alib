//
// Created by james on 10/17/17.
//

#ifndef _ALIB_OBJECT_H_
#define _ALIB_OBJECT_H_

#include "taggedpointer.h"

typedef void *aobj;

typedef aobj (*aobj_init_func)(void *ptr, void *data);
typedef void (*aobj_clean_func)(aobj id);

typedef struct aobj_meta {
  uint32_t isa;
  aobj_clean_func clean;
} aobj_meta_s, *aobj_meta_t;

ALIB_EXP_DECL aobj _aobj_alloc(size_t size, aobj_init_func init, void *data);
ALIB_EXP_DECL aobj _aobj_init(void *ptr, aobj_meta_t meta);
ALIB_EXP_DECL void _aobj_retain(aobj id);
ALIB_EXP_DECL void _aobj_release(aobj id);


// alib object system api
// ===============================

#define aobj_alloc(type, init) \
  _aobj_alloc(sizeof(type), init, NULL)
#define aobj_alloc_with(type, init, data) \
  _aobj_alloc(sizeof(type), init, data)
#define aobj_alloc_with_ex(type, init, data, size) \
  _aobj_alloc(sizeof(type) + (size), init, data)

#define aobj_init _aobj_init

#define _retain(id) _aobj_retain(id)
#define _release(id) _aobj_release(id)

// Note: we need a filed 'uint32_t magic' for mark object
#define aclass(type, ...) \
struct type; \
typedef struct type *type##_t; \
typedef struct type { \
  uint32_t magic; \
  __VA_ARGS__ \
} type##_s;

#define afunc_delc(type, message, rtype, ...) \
  ALIB_EXP_DECL rtype type##_##message ( __VA_ARGS__ )

#define afunc_defn(type, message, rtype, ...) \
  ALIB_EXP_DEFN rtype type##_##message ( __VA_ARGS__ )

#define _(type, id, message, ...) \
  __(id, type##_##message, ##__VA_ARGS__)

#endif //_ALIB_OBJECT_H_
