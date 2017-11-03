//
// Created by james on 10/22/17.
//

#ifndef _ALIB_DYNASTR_H_
#define _ALIB_DYNASTR_H_

#include "obj/aobj.h"

#if __cplusplus
extern "C" {
#endif

typedef char *ds;

typedef struct dyna_str {
  size_t len, size;
  char str[0];
} dstr_s, *dstr_t;

aobj dstr(const char *ptr, size_t len);

ds dstr2cstr(aobj id);
aobj cstr2dstr(ds ptr);

#if __cplusplus
};
#endif

#endif //_ALIB_DYNASTR_H_
