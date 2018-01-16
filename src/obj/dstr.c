//
// Created by james on 10/22/17.
//

#include <std/astr.h>
#include "obj/dstr.h"

void dstr_clean(aobj id);

static aobj_meta_s dynastr_meta = {
    .isa = FOUR_CHARS_TO_INT('D', 'S', 'T', 'R'),
    .clean = dstr_clean,
};

aobj dstr_init(void *ptr, void *data) {
  dstr_t id = aobj_init(ptr, &dynastr_meta);
  if (id) {
    strlen_t str = data;
    id->len = str->len;
    memcpy(id->str, str->ptr, str->len);
    id->str[id->len] = '\0';
    id->size = str->len;
  }
  return id;
}

void dstr_clean(aobj id) {
  if (TAGGED_AOBJECT(id)) {
    dstr_t dstr = id;
  }
}

dstr_t dstr(strlen_t str) {
  dstr_t id = aobj_alloc_with_ex(dstr_s, dstr_init, str, str->len + 1);
  if (id != NULL) {
    // TODO
  }
  return id;
}

dstr_t dstr_with_buf(const char *buf, size_t len) {
  strlen_s str = {.ptr=(char*)buf, .len=len};
  return dstr(&str);
}

dstr_t dstr_with_cstr(const char *cstr) {
  return dstr_with_buf(cstr, strlen(cstr));
}

ds dstr2cstr(dstr_t id) {
  if (id == NULL) return NULL;
  dstr_t dstr = id;
  return dstr->str;
}

dstr_t cstr2dstr(ds ptr) {
  if (ptr == NULL) return NULL;
  dstr_t dstr = (dstr_t) ((char*)ptr - sizeof(dstr_s));
  return dstr;
}
