//
// Created by james on 10/17/17.
//

#include "obj/aobj.h"

/*
 * memory map:
 *   +-----------+
 *   |  padding  |
 *   +-----------+
 *   |   ameta   |
 *   +-----------+
 *   | aobj_meta |
 *   +-----------+
 *   |  members  |
 *   +-----------+
 */

// this header is invisible for developer
// it size is times of 8 byte
typedef struct aobj_hdr {
  uint32_t isa;
  uint32_t refcnt;
  aobj_clean_func clean;
  char object[0];
} aobj_hdr_s, *aobj_hdr_t;

#define aobj_hdr(ptr) ((aobj_hdr_t)((char*)(ptr) - sizeof(aobj_hdr_s)))


aobj _aobj_alloc(size_t size, aobj_init_func init, void *data) {
  aobj_hdr_t meta = amalloc(size + sizeof(aobj_hdr_s)); // obj size + header size
  return init(meta, data);
}

aobj _aobj_init(void *ptr, aobj_meta_t meta) {
  if (ptr == NULL) return NULL;
  aobj_hdr_t hdr = ptr;
  hdr->isa = meta->isa;
  hdr->clean = meta->clean;
  hdr->refcnt = 1;
  TAG_AOBJECT(hdr->object);
  return hdr->object;
}

void _aobj_retain(aobj id) {
  if (id == NULL) return;
  if (TAGGED_AOBJECT(id)) {
    aobj_hdr_t hdr = aobj_hdr(id);
    hdr->refcnt++;
  }
}

void _aobj_release(aobj id) {
  if (id == NULL) return;
  if (TAGGED_AOBJECT(id)) {
    aobj_hdr_t hdr = aobj_hdr(id);
    hdr->refcnt--;
    if (hdr->refcnt <= 0) {
      if (hdr->clean != NULL)
        hdr->clean(id);
      afree(hdr);
    }
  }
}
