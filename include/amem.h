/**
 * amem.h - memory management
 */

#ifndef _ALIB_MEMORY_H_
#define _ALIB_MEMORY_H_

#include "atypes.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef AMEM_ALIGN
#if AMEM_ALIGN < 3
#error The align factor must greater than or equal to 3
#endif
#else
#define AMEM_ALIGN 3
#endif

#define ALIGN_BASE(b) (((uptr_t)0x01 << (b)) - 1)
#define ALIGN_MASK(b) (~ALIGN_BASE(b))

#define amem_align(base, b) \
  ((void*)(((uptr_t)((char*)(base) + ALIGN_BASE(b))) & ALIGN_MASK(b)))

#ifdef WIN32
extern HANDLE used_memory_mutex;
#elif defined(_PTHREAD_H)
extern pthread_mutex_t used_memory_mutex;
#endif


// alib memory management api
// ===============================

void *amalloc(size_t size);
void *acalloc(size_t nmemb, size_t size);
void *arealloc(void *ptr, size_t size);
void afree(void *ptr);

size_t amalloc_size(void *ptr);

size_t amalloc_used_memory(void);
void amalloc_set_oom_handler(void (*oom_handler)(size_t));

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_ALIB_MEMORY_H_
