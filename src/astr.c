//
// Created by james on 10/16/17.
//

#include "astr.h"

const strlen_s strlen_empty = {.ptr = "", .len = 0};

// extend for stand library
// ================================

char *astrdup(const char *s) {
  char *dup = (char *) amalloc(sizeof(char) * (strlen(s) + 1));
  if (dup != NULL)
    strcpy(dup, s);
  return dup;
}

size_t astrnlen(const char *s, size_t n) {
  size_t i;
  for (i = 0; i < n; i++) {
    if (s[i] == '\0') return i;
  }
  return n;
}

char *astrndup(const char *s, size_t n) {
  size_t len = strnlen(s, n);
  char *dup = amalloc(sizeof(char) * (len + 1));
  strncpy(dup, s, len);
  dup[len] = '\0';
  return dup;
}

char *astrnstr(const char *s1, const char *s2, size_t n) {
  int i;
  size_t needle_len;

  if (0 == (needle_len = strnlen(s2, n)))
    return (char *) s1;

  for (i = 0; i <= (int) (n - needle_len); i++) {
    if ((s1[0] == s2[0]) && (0 == strncmp(s1, s2, needle_len)))
      return (char *) s1;
    s1++;
  }
  return NULL;
}

//
// =======================

inline bool strpos_wlc(strpos_s w, strpos_s lc) {
  return lc.so < w.so && lc.eo > w.so ? true : false;
}

inline bool strpos_wrc(strpos_s w, strpos_s rc) {
  return rc.so < w.eo && rc.eo > w.eo ? true : false;
}

//
// ========================

bool striter_init(striter_t self, unsigned char *ptr, size_t len) {
  self->ptr = ptr;
  self->len = len;
  self->cur = 0;
  return true;
}

bool striter_reset(striter_t self) {
  self->cur = 0;
  return true;
}

int striter_cur(striter_t self) {
  if (self->cur >= self->len) return EOF;
  return self->ptr[self->cur];
}

int striter_next(striter_t self) {
  if (self->cur >= self->len) return EOF;
  return self->ptr[self->cur++];
}
