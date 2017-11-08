#ifndef _ALIB_PINT_H_
#define _ALIB_PINT_H_

#include "obj/aobj.h"

typedef void *pint_t;

#define pint(v) ((pint_t)TAG_INTEGER(v))
#define pint_get(a) ((sptr_t)GET_INTEGER(a))
#define pint_plus(a, b) ((pint_t)TAG_INTEGER(GET_INTEGER(a) + GET_INTEGER(b)))
#define pint_minus(a, b) ((pint_t)TAG_INTEGER(GET_INTEGER(a) - GET_INTEGER(b)))
#define pint_times(a, b) ((pint_t)TAG_INTEGER(GET_INTEGER(a) * GET_INTEGER(b)))
#define pint_over(a, b) ((pint_t)TAG_INTEGER(GET_INTEGER(a) / GET_INTEGER(b)))

#endif //_ALIB_PINT_H_