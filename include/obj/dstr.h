/**
 * dstr.h - string object (which be stored in dictionary)
 *
 * @author James Yin <ywhjames@hotmail.com>
 *
 * Note:
 *   dstr is designed as not null-terminated string, but it is compatible.
 *   now, dstr is constant, and we store it in a global dictionary
 */

#ifndef _ALIB_DICTSTR_H_
#define _ALIB_DICTSTR_H_

#include "obj/aobj.h"
#include "std/astr.h"

#if __cplusplus
extern "C" {
#endif

typedef char *cstr;

aclass(dstr,
   size_t len;
   size_t size;
   char str[0];
)

dstr_t dstr(strlen_t str);
dstr_t dstr_with_buf(const char *buf, size_t len);
dstr_t dstr_with_cstr(const char *cstr);

cstr dstr2cstr(dstr_t id);
dstr_t cstr2dstr(cstr ptr);

#if __cplusplus
};
#endif

#endif //_ALIB_DICTSTR_H_
