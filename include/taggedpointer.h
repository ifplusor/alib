//
// Created by james on 10/16/17.
//

#ifndef _ALIB_TAGGED_POINTER_H_
#define _ALIB_TAGGED_POINTER_H_

#include "amem.h"

#define POINTER_TAG 0x00
#define INTEGER_TAG 0x01

#define GET_TAG(x) ((uptr_t)(x) & 0x07U)

#define TAGGED_POINTER(ptr) (GET_TAG(ptr) == POINTER_TAG)
#define TAGGED_INTEGER(x) (GET_TAG(x) == INTEGER_TAG)
#define TAGGED_AOBJECT(id) (TAGGED_POINTER(id) && *((uint32_t*)(id)) == AOBJ_MAGIC)

#define GET_INTEGER(x) ((sptr_t)(x) >> AMEM_ALIGN)
#define TAG_INTEGER(x) ((void*)(((sptr_t)(x) << AMEM_ALIGN) | INTEGER_TAG))

#define TAG_AOBJECT(ptr) (*((uint32_t*)(ptr)) = AOBJ_MAGIC)

#endif //_ALIB_TAGGED_POINTER_H_
