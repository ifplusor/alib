#ifndef _ALIB_PINT_H_
#define _ALIB_PINT_H_

#include "obj/aobj.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef void *pint_t;

aobj pint(sptr_t v);

sptr_t pint_get(aobj a);

aobj pint_plus(aobj a, aobj b);
aobj pint_minus(aobj a, aobj b);
aobj pint_times(aobj a, aobj b);
aobj pint_over(aobj a, aobj b);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_ALIB_PINT_H_