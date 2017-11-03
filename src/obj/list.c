//
// Created by james on 10/17/17.
//

#include "obj/list.h"

void list_clean(aobj id);

static aobj_meta_s list_meta = {
    .isa = FOUR_CHARS_TO_INT('L', 'I', 'S', 'T'),
    .refcnt = 0,
    .clean = list_clean,
};

aobj list_init(void *ptr, void *data) {
  aobj id = aobj_init(ptr, &list_meta);
  if (id) {
    list_t con = GET_AOBJECT(id);
    con->car = con->cdr = NULL;
  }
  return id;
}

void list_clean(aobj id) {
  if (TAGGED_AOBJECT(id)) {
    list_t con = GET_AOBJECT(id);
    _release(con->car);
    _release(con->cdr);
    // TODO: optimize recursion for avoid stack overflow
  }
}

aobj list_cons(aobj a, aobj b) {
  aobj id = aobj_alloc(list_s, list_init);
  if (id != NULL) {
    list_t con = GET_AOBJECT(id);
    _retain(a);
    con->car = a;
    _retain(b);
    con->cdr = b;
  }
  return id;
}

aobj list_car(aobj list) {
  list_t con = GET_AOBJECT(list);
  return con->car;
}

aobj list_cdr(aobj list) {
  list_t con = GET_AOBJECT(list);
  return con->cdr;
}
