/**
 * atypes.h - alib types define header
 *
 * @author James Yin <ywhjames@hotmail.com>
 */

#ifndef _ALIB_TYPES_H_
#define _ALIB_TYPES_H_

// include standard library definition
#include <stddef.h>

// define EOF
#ifdef _WIN32
#define EOL "\r\n"
#else
#define EOL "\n"
#endif

// define inline alias for c in win32
#if defined(_WIN32) && !defined(__cplusplus)
#define inline __inline
#endif


//
// define base types:
//   sint8_t, sint16_t, sint32_t, sint64_t
//   uint8_t, uint16_t, uint32_t, uint64_t
//   sptr_t,  uptr_t

#include <stdint.h>

// design for Standard C Language
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef int8_t sint8_t;
typedef int16_t sint16_t;
typedef int32_t sint32_t;
typedef int64_t sint64_t;

// int with pointer length
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

typedef unsigned char uchar;

#ifdef __cplusplus
}
#endif /* __cplusplus */


//
// define boolean type:

#ifndef _WIN32
#include <stdbool.h>
#endif

#ifndef __bool_true_false_are_defined
#define bool sint8_t
#define true 1
#define false 0
#endif /* __bool_true_false_are_defined */


//
// define some marcos:

#define LOW8BIT(x) ((uint32_t)(x) & 0xFF)

#ifdef  FOUR_CHARS_TO_INT
#error Conflicting Macro "FOUR_CHARS_TO_INT"
#else
#define FOUR_CHARS_TO_INT(c1, c2, c3, c4) (LOW8BIT(c1) << 24 | LOW8BIT(c2) << 16 | LOW8BIT(c3) << 8 | LOW8BIT(c4))
#endif

#ifdef  TW0_CHARS_TO_INT
#error Conflicting Macro "TW0_CHARS_TO_INT"
#else
#define TW0_CHARS_TO_INT(c1, c2) (LOW8BIT(c1) << 8 | LOW8BIT(c2))
#endif

// magic number
#define AMEM_MAGIC FOUR_CHARS_TO_INT('A', 'M', 'e', 'm')
#define AOBJ_MAGIC FOUR_CHARS_TO_INT('A', 'O', 'b', 'j')

// call function with param
#define __(obj, func, ...) func(obj, ##__VA_ARGS__)


//
// define declare modifiers:

#ifndef ALIB_EXP_DECL
#  ifdef __cplusplus
#    define ALIB_EXP_DECL  extern "C"
#    define ALIB_EXP_DEFN  extern "C"
#  else
#    define ALIB_EXP_DECL  extern
#    define ALIB_EXP_DEFN  extern
#  endif
#endif

#ifndef ALIB_CALL_CNV
#define ALIB_CALL_CNV
#endif


// include object subsystem header
#include "obj/aobj.h"


#endif //_ALIB_TYPES_H_
