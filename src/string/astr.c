/**
 * astr.c
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#include "alib/string/astr.h"

const strlen_s strlen_empty = {.ptr = "", .len = 0};  // "" have \0

// extend for stand library
// ================================

char* astrdup(const char* src) {
  return astrndup(src, RSIZE_MAX);
}

char* astrndup(const char* src, size_t count) {
  size_t destsz = strnlen_s(src, count) + 1;
  char* dest = (char*)amalloc(destsz);
  strncpy_s(dest, destsz, src, count);
  return dest;
}

char* astrnstr(const char* s1, const char* s2, size_t n) {
  int i;
  size_t needle_len;

  if (0 == (needle_len = strnlen(s2, n))) {
    return (char*)s1;
  }

  for (i = 0; i <= (int)(n - needle_len); i++) {
    if ((s1[0] == s2[0]) && (0 == strncmp(s1, s2, needle_len))) {
      return (char*)s1;
    }
    s1++;
  }
  return NULL;
}
