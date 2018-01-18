/**
 * astr.h - string utils
 *
 * Note:
 *   this have some features which extend standard library
 */

#ifndef _ALIB_STRING_H_
#define _ALIB_STRING_H_

#include <stdio.h>
#include <string.h>

#include "amem.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// extend standard library
// ========================

char *astrdup(const char *s);
size_t astrnlen(const char *s, size_t n);
char *astrndup(const char *s, size_t n);
char *astrnstr(const char *s1, const char *s2, size_t n);

// str_len
// ========================

typedef struct str_len {
  char *ptr;
  size_t len;
} strlen_s, *strlen_t;

extern const strlen_s strlen_empty;
#define str_empty strlen_empty.ptr

// str_pos
// ========================

typedef struct str_pos {
  size_t so, eo;
} strpos_s, *strpos_t;

static inline bool strpos_wlc(strpos_s w, strpos_s lc) {
  return lc.so < w.so && lc.eo > w.so ? true : false;
}

static inline bool strpos_wrc(strpos_s w, strpos_s rc) {
  return rc.so < w.eo && rc.eo > w.eo ? true : false;
}

// str_iter
// ========================

typedef struct str_iter {
  unsigned char *ptr;
  size_t len, cur;
} striter_s, *striter_t;

static inline bool striter_init(striter_t self,
                                unsigned char *ptr,
                                size_t len) {
  self->ptr = ptr;
  self->len = len;
  self->cur = 0;
  return true;
}

static inline bool striter_reset(striter_t self) {
  self->cur = 0;
  return true;
}

static inline int striter_cur(striter_t self) {
  if (self->cur >= self->len) return EOF;
  return self->ptr[self->cur];
}

static inline int striter_next(striter_t self) {
  if (self->cur >= self->len) return EOF;
  return self->ptr[self->cur++];
}

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_ALIB_STRING_H_
