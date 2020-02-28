/**
 * dstr.h - string object (which be stored in dictionary)
 *
 * @author James Yin <ywhjames@hotmail.com>
 *
 * Note:
 *   dstr is designed as not null-terminated string, but it is compatible.
 *   now, dstr is constant, and we store it in a global dictionary
 */

#ifndef __ALIB_DSTR_H__
#define __ALIB_DSTR_H__

#include "../string/astr.h"
#include "aobj.h"

#if __cplusplus
extern "C" {
#endif

typedef char* cstr;

// clang-format off
aclass(dstr,
  size_t len;
  size_t size;
  char str[0];
);
// clang-format on

dstr_t dstr(strlen_t str);
dstr_t dstr_with_buf(const char* buf, size_t len);
dstr_t dstr_with_cstr(const char* cstr);

cstr dstr2cstr(dstr_t id);
dstr_t cstr2dstr(cstr ptr);

#if __cplusplus
};
#endif

#endif  // __ALIB_DSTR_H__
