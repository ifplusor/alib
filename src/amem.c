//
// Created by james on 10/16/17.
//
/*
 * Description:  alloc memory with alignment, and record size.
 */

#include <stdio.h>
#include <stdlib.h>

#include "amem.h"
#include "atomicvar.h"

#define PADDING_SIZE (ALIGN_BASE(AMEM_ALIGN) + sizeof(ameta_s))

#define amem_align_default(base) amem_align((char*)(base) + sizeof(ameta_s), AMEM_ALIGN)

#define update_amalloc_stat_alloc(__n) do { \
    atomicIncr(used_memory, __n); \
} while(0)

#define update_amalloc_stat_free(__n) do { \
    atomicDecr(used_memory, __n); \
} while(0)

static size_t used_memory = 0;
pthread_mutex_t used_memory_mutex = PTHREAD_MUTEX_INITIALIZER;

#include <execinfo.h>
void print_stacktrace() {
  int size = 16;
  void *array[16];
  int stack_num = backtrace(array, size);
  char **stacktrace = backtrace_symbols(array, stack_num);
  for (int i = 0; i < stack_num; ++i) {
    fprintf(stderr, "%s\n", stacktrace[i]);
  }
  free(stacktrace);
}

static void amalloc_default_oom(size_t size) {
  fprintf(stderr, "amalloc: Out of memory trying to allocate %zu bytes\n", size);
  print_stacktrace();
  fflush(stderr);
  abort();
}

static void (*amalloc_oom_handler)(size_t) = amalloc_default_oom;

void *amalloc(size_t size) {
  void *base = malloc(size + PADDING_SIZE);

  if (!base) {
    amalloc_oom_handler(size);
    return NULL;
  } else {
    void *ptr = amem_align_default(base);
    ameta_t meta = amem_meta(ptr);
    meta->base = base;
    meta->size = size;
    update_amalloc_stat_alloc(size + PADDING_SIZE);
    return ptr;
  }
}

void *acalloc(size_t nmemb, size_t size) {
  // TODO:
  void *base = calloc(1, size + PADDING_SIZE);

  if (!base) {
    amalloc_oom_handler(size);
    return NULL;
  } else {
    void *ptr = amem_align_default(base);
    ameta_t meta = amem_meta(ptr);
    meta->base = base;
    meta->size = size;
    update_amalloc_stat_alloc(size + PADDING_SIZE);
    return ptr;
  }
}

void *arealloc(void *ptr, size_t size) {
  if (ptr == NULL) return amalloc(size);

  ameta_t meta = amem_meta(ptr);
  void *realptr = meta->base;
  size_t oldsize = meta->size;
  void *newptr = realloc(realptr, size + PADDING_SIZE);

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

void afree(void *ptr) {
  if (ptr == NULL) return;

  ameta_t meta = amem_meta(ptr);
  void *realptr = meta->base;
  size_t oldsize = meta->size;
  update_amalloc_stat_free(oldsize + PADDING_SIZE);
  free(realptr);
}

size_t amalloc_size(void *ptr) {
  size_t size = amem_meta(ptr)->size;
  return size + PADDING_SIZE;
}

size_t amalloc_used_memory(void) {
  size_t um;
  atomicGet(used_memory, um);
  return um;
}

void amalloc_set_oom_handler(void (*oom_handler)(size_t)) {
  amalloc_oom_handler = oom_handler;
}
