/**
 * taggedpointer.h - tagged pointer
 *
 * @author James Yin <ywhjames@hotmail.com>
 *
 * Note：
 *   amem 中进行内存分配时，会使用对齐策略，因此我们可以直接指针的基础上做一些 hack
 */
#ifndef _ALIB_TAGGED_POINTER_H_
#define _ALIB_TAGGED_POINTER_H_

#include "../amem.h"

#define POINTER_TAG 0x00
#define INTEGER_TAG 0x01

#define GET_TAG(x) (((uptr_t)(x)) & 0x07U)  // 8 bytes aligned

//
// raw pointer

#define TAGGED_POINTER(ptr) (GET_TAG(ptr) == POINTER_TAG)

//
// integer pointer

#define TAGGED_INTEGER(x) (GET_TAG(x) == INTEGER_TAG)

#define GET_INTEGER(x) (((sptr_t)(x)) >> AMEM_ALIGN)

#define TAG_INTEGER(x) ((void*)((((sptr_t)(x)) << AMEM_ALIGN) | INTEGER_TAG))

//
// aobj pointer

#define TAGGED_AOBJECT(id) (TAGGED_POINTER(id) && *((uint32_t*)(id)) == AOBJ_MAGIC)

#define TAG_AOBJECT(ptr) (*((uint32_t*)(ptr)) = AOBJ_MAGIC)

#endif  //_ALIB_TAGGED_POINTER_H_
