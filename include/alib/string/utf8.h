/**
 * utf8.h - utf8 utils
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#ifndef __ALIB_UTF8_H__
#define __ALIB_UTF8_H__

#include "../atypes.h"

#ifdef __cplusplus
extern "C" {
#endif

#define utf8_word_dist(wp, s, e) ((wp)[e] - (wp)[s])

size_t utf8_word_len(const char* s, size_t len);
int utf8_word_pos(const char* in_s, size_t in_slen, size_t* out_wp);

#ifdef __cplusplus
};
#endif

#endif  // __ALIB_UTF8_H__
