//
// Created by james on 11/6/17.
//

#include "obj/pint.h"

aobj pint(sptr_t v) {
  return TAG_INTEGER(v);
}

sptr_t pint_get(aobj a) {
  return GET_INTEGER(a);
}

aobj pint_plus(aobj a, aobj b) {
  sptr_t ai = GET_INTEGER(a);
  sptr_t bi = GET_INTEGER(b);
  return TAG_INTEGER(ai + bi);
}

aobj pint_minus(aobj a, aobj b) {
  sptr_t ai = GET_INTEGER(a);
  sptr_t bi = GET_INTEGER(b);
  return TAG_INTEGER(ai - bi);
}

aobj pint_times(aobj a, aobj b) {
  sptr_t ai = GET_INTEGER(a);
  sptr_t bi = GET_INTEGER(b);
  return TAG_INTEGER(ai * bi);
}

aobj pint_over(aobj a, aobj b) {
  sptr_t ai = GET_INTEGER(a);
  sptr_t bi = GET_INTEGER(b);
  return TAG_INTEGER(ai / bi);
}
