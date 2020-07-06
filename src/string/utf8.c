/**
 * utf8.c
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#include "alib/string/utf8.h"

// clang-format off
unsigned char utf8_size_table[256] = {
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
  3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
  4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 1, 1
};
// clang-format on

#define UTF8_WORD_SIZE(s) (utf8_size_table[(unsigned char)*(s)])

size_t utf8_word_len(const char* s, size_t slen) {
  size_t wlen = 0;
  size_t i = 0;
  while (i < slen) {
    i += UTF8_WORD_SIZE(s + i);
    wlen++;
  }

  return wlen;
}

/**
 * utf8_word_position - 取得utf8编码字符串的字位置
 * @param in_s - string buffer
 * @param in_slen - length of string in buffer s
 * @param out_wp - the size of buffer wp must bigger than slen
 * @return
 */
int utf8_word_pos(const char* in_s, size_t in_slen, size_t* out_wp) {
  size_t wlen = 0;
  size_t i = 0;
  while (i < in_slen) {
    for (size_t n = UTF8_WORD_SIZE(in_s + i); n > 0 && i < in_slen; n--) {
      out_wp[i++] = wlen;
    }
    wlen++;
  }
  out_wp[i] = wlen;  // upper bound
  return 0;
}
