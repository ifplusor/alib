/**
 * dynabuf.c
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#include "alib/string/dynabuf.h"

// dynamic buffer
// ========================================

dynabuf_t dynabuf_alloc() {
  dynabuf_t buf = malloc(sizeof(dynabuf_s));
  if (buf != NULL) {
    buf->_buffer = NULL;
  }
  return buf;
}

bool dynabuf_free(dynabuf_t self) {
  if (self == NULL) {
    return false;
  }
  dynabuf_clean(self);
  free(self);
  return true;
}

/**
 * dynabuf_init - initialize
 * @param self - dynabuf object
 * @param size - initial size
 */
bool dynabuf_init(dynabuf_t self, size_t size) {
  if (self == NULL) {
    return false;
  }

  // init with '\0'
  self->_cur = 1;
  if (size != 0) {
    // size 为 16 的倍数
    size = ROUND_UP_16(size + 1);
    self->_buffer = malloc(size);
    if (self->_buffer == NULL) {
      return false;
    }
    self->_buffer[0] = '\0';
    self->_size = size;
  } else {
    self->_buffer = str_empty;
    self->_size = 1;
  }
  return true;
}

bool dynabuf_clean(dynabuf_t self) {
  if (dynabuf_reset(self)) {
    if (self->_buffer != str_empty) {
      free(self->_buffer);
    }
    self->_buffer = str_empty;
    self->_size = 1;
    return true;
  }
  return false;
}

bool dynabuf_reset(dynabuf_t self) {
  if (self == NULL) {
    return false;
  }
  self->_cur = 1;
  return true;
}

/**
 * dynabuf_content - 返回 dynabuf 中的内容
 */
strlen_s dynabuf_content(dynabuf_t self) {
  return (strlen_s){.ptr = self->_buffer + 1, .len = self->_cur - 1};
}

/**
 * dynabuf_splite - 截取 dynabuf 中由 pos 指定的区段
 */
strlen_s dynabuf_split(dynabuf_t self, strpos_s pos) {
  return (strlen_s){.ptr = self->_buffer + pos.so, .len = pos.eo - pos.so};
}

size_t dynabuf_length(dynabuf_t self) {
  return self->_cur - 1;
}

bool dynabuf_empty(dynabuf_t self) {
  return self->_cur == 1;
}

// string builder
// =========================

const static size_t dynabuf_extend_space_size = ROUND_UP_16(DYNABUF_EXTEND_SPACE_SIZE);

static inline size_t dynabuf_extend_size(size_t old_size, size_t insert_size) {
  return old_size + (insert_size / dynabuf_extend_space_size + 1) * dynabuf_extend_space_size;
}

static inline void dynabuf_adjust_memory(dynabuf_t self, size_t len) {
  if (self->_cur + len > self->_size) {
    // extend buffer
    size_t new_size = dynabuf_extend_size(self->_size, len);
    char* new_buffer = malloc(new_size);
    if (new_buffer == NULL) {
      ALOG_FATAL("memory error!");
    }
    memcpy(new_buffer, self->_buffer, self->_cur);
    if (self->_buffer != str_empty)
      free(self->_buffer);
    self->_buffer = new_buffer;
    self->_size = new_size;
  }
}

char* dynabuf_write(dynabuf_t self, const char* src, size_t len) {
  //  if (self == NULL) return NULL;

  dynabuf_adjust_memory(self, len);

  char* dest = memcpy(self->_buffer + self->_cur, src, len);
  self->_cur += len;

  return dest;
}

char* dynabuf_write_eos(dynabuf_t self, const char* src, size_t len) {
  //  if (self == NULL) return NULL;

  dynabuf_adjust_memory(self, len + 1);

  char* dest = memcpy(self->_buffer + self->_cur, src, len);
  self->_cur += len;

  // append '\0'
  self->_buffer[self->_cur] = '\0';
  self->_cur++;

  return dest;
}

// parser
// =========================

/**
 * dynabuf_consume_until - consume chars from stream until specific delimiters or EOF is arrived
 */
int dynabuf_consume_until(dynabuf_t self, stream_t stream, const char* delim, strpos_t out_pos) {
  unsigned char buf[256], *pb = buf;
  unsigned char* s = (unsigned char*)delim;
  int ch;

  // start position
  if (out_pos) {
    out_pos->so = self->_cur;
  }

  if (delim == NULL || delim[0] == '\0') {
    // empty delimiter
    ch = 0;
  } else {
    if (delim[1] == '\0') {
      // only one delimiter
      while ((ch = stream_getc(stream)) != EOF && ch != *s) {
        *(pb++) = (unsigned char)ch;
        if ((pb - buf) == 256) {
          dynabuf_write(self, (char*)buf, 256);
          pb = buf;
        }
      }
    } else {
      unsigned char table[256];
      unsigned char* p = memset(table, 0, 64);
      memset(p + 64, 0, 64);
      memset(p + 128, 0, 64);
      memset(p + 192, 0, 64);

      do {
        p[*s++] = 1;
      } while (*s);

      while ((ch = stream_getc(stream)) != EOF && !p[ch]) {
        *(pb++) = (unsigned char)ch;
        if ((pb - buf) == 256) {
          dynabuf_write(self, (char*)buf, 256);
          pb = buf;
        }
      }
    }

    dynabuf_write(self, (char*)buf, pb - buf);
  }

  // end position
  if (out_pos) {
    out_pos->eo = self->_cur;
  }

  return ch;
}
