//
// Created by james on 10/16/17.
//

#ifndef _ALIB_TAGGED_POINTER_H_
#define _ALIB_TAGGED_POINTER_H_

#include "amem.h"

#define POINTER_TAG(x) ((uptr_t)(x) & 0x07U)

#define TAGGED_POINTER(ptr) (POINTER_TAG(ptr) == 0)
#define TAGGED_INTEGER(x) (POINTER_TAG(x) == 0x01)
#define TAGGED_AOBJECT(id) (POINTER_TAG(id) == 0x02)

#define GET_INTEGER(x)  ((uptr_t)(x) >> AMEM_ALIGN)
#define TAG_INTEGER(x) ((void*)(((uptr_t)(x) << AMEM_ALIGN) | 0x01))

#define GET_AOBJECT(id) ((void*)((uptr_t)(id) & ALIGN_MASK(AMEM_ALIGN)))
#define TAG_AOBJECT(ptr) ((void*)((uptr_t)(ptr) | 0x02))

#define aobj_getptr(x) (TAGGED_AOBJECT(x) ? GET_AOBJECT(x) : (x))

#endif //_ALIB_TAGGED_POINTER_H_
