#ifndef _ALIB_LIST_H_
#define _ALIB_LIST_H_

#include "aobj.h"

typedef struct list *list_t;

typedef struct list {
  aobj car, cdr;
} list_s;

aobj cons(aobj a, aobj b);
aobj car(aobj list);
aobj cdr(aobj list);

#endif //_ALIB_LIST_H_
