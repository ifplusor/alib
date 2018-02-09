/**
 * aobj.c - object subsystem implement
 *
 * @author James Yin <ywhjames@hotmail.com>
 * @version 10/17/17
 */

#include "obj/aobj.h"
//#include "atomic.h"

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
typedef struct _aobj_hdr {
  uint32_t isa;
  uint32_t refcnt;
  aobj_clean_func clean;
  aobj_hash_func hash;
  char object[0];
} aobj_hdr_s, *aobj_hdr_t;

#define aobj_hdr(ptr) ((aobj_hdr_t)((char*)(ptr) - sizeof(aobj_hdr_s)))

static uint64_t _aobj_hash_default(aobj id) {
  return (uint64_t) id;
}


aobj _aobj_alloc(size_t size, aobj_init_func init, void *data) {
  aobj_hdr_t meta = amalloc(size + sizeof(aobj_hdr_s)); // obj size + header size
  return init(meta, data);
}

aobj _aobj_init(void *ptr, aobj_meta_t meta) {
  if (ptr == NULL) return NULL;
  aobj_hdr_t hdr = ptr;
  hdr->isa = meta->isa;
  hdr->clean = meta->clean;
  hdr->hash = meta->hash == NULL ? _aobj_hash_default : meta->hash;
  hdr->refcnt = 1;
  TAG_AOBJECT(hdr->object);
  return hdr->object;
}

void _aobj_retain(aobj id) {
  if (id == NULL) return;
  if (TAGGED_AOBJECT(id)) {
    aobj_hdr_t hdr = aobj_hdr(id);
//    alib_add_fetch(hdr->refcnt, 1);
    hdr->refcnt++;
  }
}

void _aobj_release(aobj id) {
  if (id == NULL) return;
  if (TAGGED_AOBJECT(id)) {
    aobj_hdr_t hdr = aobj_hdr(id);
//    alib_sub_fetch(hdr->refcnt, 1);
    hdr->refcnt--;
    if (hdr->refcnt <= 0) {
      if (hdr->clean != NULL)
        hdr->clean(id);
      afree(hdr);
    }
  }
}

bool _aobj_instanceof(aobj id, uint32_t identifier) {
  if (id == NULL) return false;
  if (TAGGED_AOBJECT(id)) {
    aobj_hdr_t hdr = aobj_hdr(id);
    return hdr->isa == identifier;
  }
  return false;
}
