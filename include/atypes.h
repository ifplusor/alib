//
// Created by james on 10/16/17.
//

#ifndef _ALIB_TYPES_H_
#define _ALIB_TYPES_H_

#include <stddef.h>

#ifdef _WIN32
#define EOL "\r\n"
#else
#define EOL "\n"
#endif

#if defined(_WIN32) && !defined(__cplusplus)
#define inline __inline
#endif

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

// define base types:
//   sint8_t, sint16_t, sint32_t, sint64_t
//   uint8_t, uint16_t, uint32_t, uint64_t
//   sptr_t, uptr_t

#include <stdint.h>

typedef int8_t sint8_t;
typedef int16_t sint16_t;
typedef int32_t sint32_t;
typedef int64_t sint64_t;

#ifdef _WIN32
#include <wtypes.h>
typedef LONG_PTR  sptr_t;
typedef ULONG_PTR uptr_t;
#elif defined(__LP64__)
typedef sint64_t sptr_t;
typedef uint64_t uptr_t;
#else
typedef sint32_t sptr_t;
typedef uint32_t uptr_t;
#endif

#ifndef _WIN32
#include <stdbool.h>
#endif

#ifndef __bool_true_false_are_defined
#define bool sint8_t
#define true 1
#define false 0
#endif /* __bool_true_false_are_defined */

#define LOW8BIT(x) ((uint32_t)(x) & 0xFF)

#ifdef  FOUR_CHARS_TO_INT
#error Conflicting Macro "FOUR_CHARS_TO_INT"
#endif

#define FOUR_CHARS_TO_INT(c1, c2, c3, c4) (LOW8BIT(c1) << 24 | LOW8BIT(c2) << 16 | LOW8BIT(c3) << 8 | LOW8BIT(c4))

#ifdef  TW0_CHARS_TO_INT
#error Conflicting Macro "TW0_CHARS_TO_INT"
#endif

#define TW0_CHARS_TO_INT(c1, c2) (LOW8BIT(c1) << 8 | LOW8BIT(c2))

#define __(obj, func, ...) func(obj, ##__VA_ARGS__)

#ifdef __cplusplus
}
#endif /* __cplusplus */

#include "aobj.h"

#endif //_ALIB_TYPES_H_
