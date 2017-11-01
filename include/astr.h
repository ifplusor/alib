//
// Created by james on 10/16/17.
//

#ifndef _ALIB_ASTR_H_
#define _ALIB_ASTR_H_

#include <stdio.h>
#include <string.h>

#include "amem.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

char *astrdup(const char *s);
size_t astrnlen(const char *s, size_t n);
char *astrndup(const char *s, size_t n);
char *astrnstr(const char *s1, const char *s2, size_t n);

typedef struct str_len {
  char *ptr;
  size_t len;
} strlen_s, *strlen_t;

extern const strlen_s strlen_empty;
#define str_empty strlen_empty.ptr

typedef union str_cur {
  char *ptr;
  size_t idx;
} strcur_s, *strcur_t;

typedef struct str_pos {
  size_t so, eo;
} strpos_s, *strpos_t;

//bool strpos_wlc(strpos_s w, strpos_s lc);
//bool strpos_wrc(strpos_s w, strpos_s rc);
inline bool strpos_wlc(strpos_s w, strpos_s lc) {
  return lc.so < w.so && lc.eo > w.so ? true : false;
}

inline bool strpos_wrc(strpos_s w, strpos_s rc) {
  return rc.so < w.eo && rc.eo > w.eo ? true : false;
}

typedef struct str_iter {
  unsigned char *ptr;
  size_t len, cur;
} striter_s, *striter_t;

bool striter_init(striter_t self, unsigned char *ptr, size_t len);
bool striter_reset(striter_t self);
int striter_next(striter_t self);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_ALIB_ASTR_H_
