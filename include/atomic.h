/**
 * atomic.h - atomic interface for various
 *
 * @author james
 * @version 2/1/18
 *
 * Note:
 *   we support there operators in multi-platform
 *
 *   原子操作包含三方面语义：
 *       1. 操作本身是不可分割的(Atomicity);
 *       2. 一个线程对某个数据的操作何时对另外一个线程可见(Visibility);
 *       3. 执行的顺序是否可以被重排(Ordering)
 */

#ifndef _ALIB_ATOMIC_H_
#define _ALIB_ATOMIC_H_


#if defined(__ATOMIC_RELAXED) && !defined(__sun) && (!defined(__clang__) || !defined(__APPLE__) || __apple_build_version__ > 4210057)
/* Implementation using __atomic macros. */

#define alib_add_fetch(var, count) \
    __atomic_add_fetch(&var, (count), __ATOMIC_RELAXED)

#define alib_fetch_add(var, old, count) do { \
    old = __atomic_fetch_add(&var, (count), __ATOMIC_RELAXED); \
} while(0)

#define alib_sub_fetch(var, count) \
    __atomic_sub_fetch(&var, (count), __ATOMIC_RELAXED)

#define alib_load_n(var, dst) do { \
    dst = __atomic_load_n(&var, __ATOMIC_RELAXED); \
} while(0)

#define alib_store_n(var, value) \
    __atomic_store_n(&var, value, __ATOMIC_RELAXED)

#define ALIB_ATOMIC_API "atomic-builtin"


#elif defined(HAVE_ATOMIC)
/* Implementation using __sync macros. */

#define alib_add_fetch(var, count) \
    __sync_add_and_fetch(&var, (count))

#define alib_fetch_add(var, old, count) do { \
    old  = __sync_fetch_and_add(&var, (count)); \
} while(0)

#define alib_sub_fetch(var, count) \
    __sync_sub_and_fetch(&var, (count))

#define alib_load_n(var, dst) do { \
    dst = __sync_sub_and_fetch(&var, 0); \
} while(0)

#define alib_store_n(var, value) do { \
    while(!__sync_bool_compare_and_swap(&var, var, value)); \
} while(0)

#define ALIB_ATOMIC_API "sync-builtin"


#elif defined(WIN32)
/* Implementation using windows mutex. */
#include <windows.h>

#define alib_add_fetch(var, count) do { \
    WaitForSingleObject(&var ## _mutex, INFINITE); \
    var += (count); \
    ReleaseMutex(&var ## _mutex); \
} while(0)

#define alib_fetch_add(var, old, count) do { \
    WaitForSingleObject(&var ## _mutex, INFINITE); \
    old = var; \
    var += (count); \
    ReleaseMutex(&var ## _mutex); \
} while(0)

#define alib_sub_fetch(var, count) do { \
    WaitForSingleObject(&var ## _mutex, INFINITE); \
    var -= (count); \
    ReleaseMutex(&var ## _mutex); \
} while(0)

#define alib_load_n(var, dst) do { \
    WaitForSingleObject(&var ## _mutex, INFINITE); \
    dst = var; \
    ReleaseMutex(&var ## _mutex); \
} while(0)

#define alib_store_n(var, value) do { \
    WaitForSingleObject(&var ## _mutex, INFINITE); \
    var = value; \
    ReleaseMutex(&var ## _mutex); \
} while(0)

#define ALIB_ATOMIC_API "win-mutex"


#else
/* Implementation using pthread mutex. */
#include <pthread.h>

#define alib_add_fetch(var, count) do { \
    pthread_mutex_lock(&var ## _mutex); \
    var += (count); \
    pthread_mutex_unlock(&var ## _mutex); \
} while(0)

#define alib_fetch_add(var, old, count) do { \
    pthread_mutex_lock(&var ## _mutex); \
    old = var; \
    var += (count); \
    pthread_mutex_unlock(&var ## _mutex); \
} while(0)

#define alib_sub_fetch(var, count) do { \
    pthread_mutex_lock(&var ## _mutex); \
    var -= (count); \
    pthread_mutex_unlock(&var ## _mutex); \
} while(0)

#define alib_load_n(var, dst) do { \
    pthread_mutex_lock(&var ## _mutex); \
    dst = var; \
    pthread_mutex_unlock(&var ## _mutex); \
} while(0)

#define alib_store_n(var, value) do { \
    pthread_mutex_lock(&var ## _mutex); \
    var = value; \
    pthread_mutex_unlock(&var ## _mutex); \
} while(0)

#define ALIB_ATOMIC_API "pthread-mutex"

#endif

#endif //_ALIB_ATOMIC_H_
