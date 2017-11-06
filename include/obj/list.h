#ifndef _ALIB_LIST_H_
#define _ALIB_LIST_H_

#include "aobj.h"

typedef struct list *list_t;

typedef struct list {
  uint32_t magic;
  aobj car, cdr;
} list_s;

list_t list_cons(aobj a, aobj b);
aobj list_car(list_t list);
aobj list_cdr(list_t list);

#endif //_ALIB_LIST_H_
