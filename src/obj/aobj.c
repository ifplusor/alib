//
// Created by james on 10/17/17.
//

#include "obj/aobj.h"

aobj _aobj_alloc(size_t size, aobj_init_func init, void *data) {
  aobj_meta_t meta = amalloc(size + sizeof(aobj_meta_s));
  return init(meta, data);
}

aobj aobj_init(void *ptr, aobj_meta_t meta) {
  if (ptr == NULL) return NULL;
  aobj_meta_t m = ptr;
  *m = *meta;
  m->refcnt = 1;
  return TAG_AOBJECT(m->object);
}

void aobj_retain(aobj id) {
  if (id == NULL) return;
  if (TAGGED_AOBJECT(id)) {
    aobj_meta_t meta = aobj_meta(GET_AOBJECT(id));
    meta->refcnt++;
  }
}

void aobj_release(aobj id) {
  if (id == NULL) return;
  if (TAGGED_AOBJECT(id)) {
    aobj_meta_t meta = aobj_meta(GET_AOBJECT(id));
    meta->refcnt--;
    if (meta->refcnt <= 0) {
      if (meta->clean != NULL)
        meta->clean(id);
      afree(meta);
    }
  }
}