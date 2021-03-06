/**
 * amem.h - memory management
 *
 * @author James Yin <ywhjames@hotmail.com>
 *
 * @note
 *   alloc memory with alignment, and record size.
 */
#include "alib/amem.h"

#include <stdio.h>
#include <stdlib.h>

#include "alib/concurrent/atomic.h"

#define PADDING_SIZE (ALIGN_BASE(AMEM_ALIGN) + sizeof(ameta_s))

#define amem_align_default(base) amem_align((char*)(base) + sizeof(ameta_s), AMEM_ALIGN)

#define update_amalloc_stat_alloc(__n)   \
  do {                                   \
    atomic_llong_fetch_add(&used_memory, __n); \
  } while (0)

#define update_amalloc_stat_free(__n)    \
  do {                                   \
    atomic_llong_fetch_sub(&used_memory, __n); \
  } while (0)

static atomic_llong used_memory = 0;

#if __linux__
#include <execinfo.h>
void print_stacktrace() {
  int size = 16;
  void* array[16];
  int stack_num = backtrace(array, size);
  char** stacktrace = backtrace_symbols(array, stack_num);
  for (int i = 0; i < stack_num; ++i) {
    fprintf(stderr, "%s\n", stacktrace[i]);
  }
  free(stacktrace);
}
#else
void print_stacktrace() {}
#endif

static void amalloc_default_oom(size_t size) {
  fprintf(stderr, "amalloc: Out of memory trying to allocate %zu bytes\n", size);
  print_stacktrace();
  fflush(stderr);
  abort();
}

static void (*amalloc_oom_handler)(size_t) = amalloc_default_oom;

/*
 * memory map:
 *   +-----------+
 *   |  padding  |
 *   +-----------+
 *   |   ameta   |
 *   +-----------+ <-- 2^b byte aligned
 *   |   space   |
 *   +-----------+
 */

// times of 8 byte
typedef struct amem_meta {
  void* base;
  size_t size;
  char space[0];
} ameta_s, *ameta_t;

#define amem_meta(ptr) ((ameta_t)((char*)(ptr) - sizeof(ameta_s)))

void* amalloc(size_t size) {
  void* base = malloc(size + PADDING_SIZE);

  if (!base) {
    amalloc_oom_handler(size);
    return NULL;
  } else {
    void* ptr = amem_align_default(base);
    ameta_t meta = amem_meta(ptr);
    meta->base = base;
    meta->size = size;
    update_amalloc_stat_alloc(size + PADDING_SIZE);
    return ptr;
  }
}

void* acalloc(size_t nmemb, size_t size) {
  void* base = calloc(1, nmemb * size + PADDING_SIZE);

  if (!base) {
    amalloc_oom_handler(size);
    return NULL;
  } else {
    void* ptr = amem_align_default(base);
    ameta_t meta = amem_meta(ptr);
    meta->base = base;
    meta->size = size;
    update_amalloc_stat_alloc(size + PADDING_SIZE);
    return ptr;
  }
}

void* arealloc(void* ptr, size_t size) {
  // When you pass NULL pointer to realloc, it behaves like malloc
  if (ptr == NULL){
    return amalloc(size);
  }

  ameta_t meta = amem_meta(ptr);
  void* realptr = meta->base;
  size_t oldsize = meta->size;
  void* newptr = realloc(realptr, size + PADDING_SIZE);

  if (!newptr) {
    amalloc_oom_handler(size);
    return NULL;
  } else {
    ptr = amem_align_default(newptr);
    meta = amem_meta(ptr);
    meta->base = newptr;
    meta->size = size;
    update_amalloc_stat_free(oldsize);
    update_amalloc_stat_alloc(size);
    return ptr;
  }
}

void afree(void* ptr) {
  if (ptr == NULL) {
    return;
  }

  ameta_t meta = amem_meta(ptr);
  void* realptr = meta->base;
  size_t oldsize = meta->size;
  update_amalloc_stat_free(oldsize + PADDING_SIZE);
  free(realptr);
}

size_t amalloc_size(void* ptr) {
  size_t size = amem_meta(ptr)->size;
  return size + PADDING_SIZE;
}

size_t amalloc_used_memory(void) {
  return (size_t)atomic_llong_load(&used_memory);
}

void amalloc_set_oom_handler(void (*oom_handler)(size_t)) {
  amalloc_oom_handler = oom_handler;
}
