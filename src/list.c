//
// Created by james on 10/17/17.
//

#include "list.h"

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
    aobj_release(con->car);
    aobj_release(con->cdr);
  }
}

aobj cons(aobj a, aobj b) {
  aobj id = aobj_alloc(list_s, list_init);
  if (id != NULL) {
    list_t con = GET_AOBJECT(id);
    aobj_retain(a);
    con->car = a;
    aobj_retain(b);
    con->cdr = b;
  }
  return id;
}

aobj car(aobj list) {
  list_t con = GET_AOBJECT(list);
  return con->car;
}

aobj cdr(aobj list) {
  list_t con = GET_AOBJECT(list);
  return con->cdr;
}
