/**
 * stream.h - input stream
 *
 * @author James Yin <ywhjames@hotmail.com>
 */

#ifndef _ALIB_STREAM_H_
#define _ALIB_STREAM_H_

#include "acom.h"

#ifdef __cplusplus
extern "C" {
#endif

//
// stream api

struct stream;
typedef struct stream *stream_t;

typedef bool (*stream_destruct_func)(stream_t);
typedef int (*stream_getc_func)(stream_t);
typedef int (*stream_ungetc_func)(stream_t, int);
typedef void (*stream_rewind_func)(stream_t);

typedef struct stream_func {
  stream_destruct_func sf_destruct;
  stream_getc_func sf_getc;
  stream_ungetc_func sf_ungetc;
  stream_rewind_func sf_rewind;
} stream_func_l, *stream_func_t;

typedef enum stream_type {
  stream_type_file = 1,
  stream_type_string = 2,
} stream_type_e;

typedef struct stream {
  stream_func_t _func;
} stream_s;

stream_t stream_construct(stream_type_e type, void *src);
bool stream_destruct(stream_t self);

int stream_getc(stream_t self);
int stream_ungetc(stream_t self, int c);
void stream_rewind(stream_t self);


// file stream
typedef struct file_stream {
  stream_s header;
  FILE *fp;
} file_stream_s, *file_stream_t;

// string stream
typedef struct string_stream {
  stream_s header;
  char *str;
  striter_s iter;
} string_stream_s, *string_stream_t;

#ifdef __cplusplus
};
#endif

#endif //_ALIB_STREAM_H_
