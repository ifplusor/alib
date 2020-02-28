/**
 * list.h
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#include "alib/object/list.h"

void list_clean(aobj id);

// clang-format off
ameta(list,
  FOUR_CHARS_TO_INT('L', 'I', 'S', 'T'),
  list_clean
);
// clang-format on

aobj list_init(void* ptr, void* data) {
  list_t id = aobj_init(list, ptr);
  if (id) {
    id->car = id->cdr = NULL;
  }
  return id;
}

void list_clean(aobj id) {
  if (TAGGED_AOBJECT(id)) {
    list_t con = id;
    _release(con->car);
    _release(con->cdr);
    // TODO: optimize recursion for avoid stack overflow
  }
}

afunc_defn(list, cons, list_t, aobj a, aobj b) {
  list_t con = aobj_alloc(list_s, list_init);
  if (con != NULL) {
    _retain(a);
    con->car = a;
    _retain(b);
    con->cdr = b;
  }
  return con;
}

afunc_defn(list, car, aobj, list_t list) {
  return list->car;
}

afunc_defn(list, cdr, aobj, list_t list) {
  return list->cdr;
}
