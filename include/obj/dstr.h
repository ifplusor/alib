/**
 * dstr.h - dynamic string object
 *
 * Note:
 *   dstr is designed as not null-terminated string, but it is compatible
 */

#ifndef _ALIB_DYNASTR_H_
#define _ALIB_DYNASTR_H_

#include "obj/aobj.h"

#if __cplusplus
extern "C" {
#endif

typedef char *ds;

aclass(dstr,
   size_t len;
   size_t size;
   char str[0];
)

dstr_t dstr(strlen_t str);
dstr_t dstr_with_buf(const char *buf, size_t len);
dstr_t dstr_with_cstr(const char *cstr);

ds dstr2cstr(dstr_t id);
dstr_t cstr2dstr(ds ptr);

#if __cplusplus
};
#endif

#endif //_ALIB_DYNASTR_H_
