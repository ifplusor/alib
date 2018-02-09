/**
 * dynabuf.h - dynamic string buffer
 *
 * @author James Yin <ywhjames@hotmail.com>
 *
 * Note:
 *   dynabuf is also string builder and stream parser
 */

#ifndef _ALIB_DYNABUF_H_
#define _ALIB_DYNABUF_H_

#include "acom.h"
#include "stream.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if !DYNABUF_EXTEND_SPACE_SIZE
#define DYNABUF_EXTEND_SPACE_SIZE 1024
#endif

typedef struct dynamic_buffer {
  char *_buffer;
  size_t _size, _cur;
} dynabuf_s, *dynabuf_t;

dynabuf_t dynabuf_alloc();
bool dynabuf_free(dynabuf_t self);

bool dynabuf_init(dynabuf_t self, size_t size);
bool dynabuf_clean(dynabuf_t self);
bool dynabuf_reset(dynabuf_t self);

strlen_s dynabuf_content(dynabuf_t self);
strlen_s dynabuf_split(dynabuf_t self, strpos_s pos);
size_t dynabuf_length(dynabuf_t self);
bool dynabuf_empty(dynabuf_t self);

//
// string builder api
char *dynabuf_write(dynabuf_t self, const char *src, size_t len);
char *dynabuf_write_eos(dynabuf_t self, const char *src, size_t len);

//
// stream parser api
int dynabuf_consume_until(dynabuf_t self, stream_t stream, const char *delim, strpos_t out_pos);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif //_ALIB_DYNABUF_H_
