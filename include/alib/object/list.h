/**
 * list.h - list object
 *
 * @author James Yin <ywhjames@hotmail.com>
 */

#ifndef _ALIB_LIST_H_
#define _ALIB_LIST_H_

#include "aobj.h"

// clang-format off
aclass(list,
  aobj car;
  aobj cdr;
);
// clang-format on

afunc_delc(list, cons, list_t, aobj a, aobj b);
afunc_delc(list, car, aobj, list_t list);
afunc_delc(list, cdr, aobj, list_t list);

#endif  //_ALIB_LIST_H_
