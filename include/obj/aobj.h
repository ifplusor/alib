/**
 * aobj.h - object subsystem base header
 *
 * Note:
 *   尽管OOP的三大支柱是：封装、继承、多态。但我设计alib库，并不是要实现面向对象的语言特性（如果需要，为什么不直接使用面向对象语言？）。
 *   我们切入的核心是——内存管理，C开发者赖以生存的基本技能。
 *   alib仅仅是一个标准C语言的扩展库，我们没有理由在C语言中实现一个基于可达性分析的GC层。
 *   因此，aobj仅提供一套便于内存管理的，基于引用计数方案的，“对象”设计框架。
 *   aobj使用引用计数维持对象，并提供了对象生命周期中的构造和析构切入点。
 *   其中：
 *     构造函数 是调用aobj_alloc时传入的aobj_init_func类型回调函数;
 *     析构函数 是在对象构造过程中，调用aobj_init注入的aobje_meta中携带的aobj_clean_func类型回调函数;
 */

#ifndef _ALIB_OBJECT_H_
#define _ALIB_OBJECT_H_

// use tagged pointer
#include "taggedpointer.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void *aobj;  // void* 兼容原生指针

typedef aobj (*aobj_init_func)(void *ptr, void *data);
typedef void (*aobj_clean_func)(aobj id);

// 对象类型元数据
typedef struct aobj_meta {
  uint32_t isa;           // type magic number
  aobj_clean_func clean;  // destruct function
} aobj_meta_s, *aobj_meta_t;

ALIB_EXP_DECL aobj _aobj_alloc(size_t size, aobj_init_func init, void *data);
ALIB_EXP_DECL aobj _aobj_init(void *ptr, aobj_meta_t meta);
ALIB_EXP_DECL void _aobj_retain(aobj id);
ALIB_EXP_DECL void _aobj_release(aobj id);
ALIB_EXP_DECL bool _aobj_instanceof(aobj id, uint32_t identifier);

#ifdef __cplusplus
};
#endif


//
// alib object subsystem api

#define aobj_alloc(type, init) \
  _aobj_alloc(sizeof(type), init, NULL)
#define aobj_alloc_with(type, init, data) \
  _aobj_alloc(sizeof(type), init, data)
#define aobj_alloc_with_ex(type, init, data, size) \
  _aobj_alloc(sizeof(type) + (size), init, data)

#define aobj_init(type, ptr) \
  _aobj_init(ptr, &_aobj_##type##_meta)

// reference count operator
#define _retain(id) _aobj_retain(id)
#define _release(id) _aobj_release(id)

#define _instanceof(type, id) \
  _aobj_instanceof(id, _aobj_##type##_identifier)


//
// class declare/define macros:

#define ameta(type, id, clean_func) \
const uint32_t _aobj_##type##_identifier = (id); \
\
static aobj_meta_s _aobj_##type##_meta = { \
  .isa = (id), \
  .clean = clean_func \
};

// Note: we need a filed 'uint32_t magic' for mark object
#define aclass(type, ...) \
struct _aobj_##type; \
typedef struct _aobj_##type *type##_t; \
typedef struct _aobj_##type { \
  uint32_t magic; \
  __VA_ARGS__ \
} type##_s; \
\
extern const uint32_t _aobj_##type##_identifier;

#define afunc_delc(type, message, rtype, ...) \
  ALIB_EXP_DECL rtype type##_##message ( __VA_ARGS__ )

#define afunc_defn(type, message, rtype, ...) \
  ALIB_EXP_DEFN rtype type##_##message ( __VA_ARGS__ )

#define _alloc(type, factory, ...) \
  type##_##factory ( __VA_ARGS__ )

#define _(type, id, message, ...) \
  type##_##message ( id, ##__VA_ARGS__ )

#endif //_ALIB_OBJECT_H_
