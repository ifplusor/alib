//
// Created by james on 10/22/17.
//

#include <astr.h>
#include "dynastr.h"

void dstr_clean(aobj id);

static aobj_meta_s dynastr_meta = {
    .isa = FOUR_CHARS_TO_INT('D', 'S', 'T', 'R'),
    .refcnt = 0,
    .clean = dstr_clean,
};

aobj dstr_init(void *ptr, void *data) {
  aobj id = aobj_init(ptr, &dynastr_meta);
  if (id) {
    dstr_t dstr = GET_AOBJECT(id);
    strlen_t str = data;
    dstr->size = str->len;
    dstr->len = astrnlen(str->ptr, dstr->size);
    strncpy(dstr->str, str->ptr, dstr->len);
    dstr->str[dstr->len] = '\0';
  }
  return id;
}

void dstr_clean(aobj id) {
  if (TAGGED_AOBJECT(id)) {
    dstr_t dstr = GET_AOBJECT(id);
  }
}

aobj dstr(const char *ptr, size_t len) {
  strlen_s str = {.ptr=(char*)ptr, .len=len};
  aobj id = aobj_alloc_with_ex(dstr_s, dstr_init, &str, len+1);
  if (id != NULL) {
    dstr_t dstr = GET_AOBJECT(id);
  }
  return id;
}

ds dstr2cstr(aobj id) {
  if (id == NULL) return NULL;
  dstr_t dstr = GET_AOBJECT(id);
  return dstr->str;
}

aobj cstr2dstr(ds ptr) {
  if (ptr == NULL) return NULL;
  dstr_t dstr = (dstr_t) ((char*)ptr - sizeof(dstr_s));
  return TAG_AOBJECT(dstr);
}
