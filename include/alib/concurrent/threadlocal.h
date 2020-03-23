/**
 * threadlocal.h - thread specific data
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#ifndef __ALIB_THREAD_LOCAL_H__
#define __ALIB_THREAD_LOCAL_H__

#include "../acom.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#if !defined(__STDC_NO_THREADS__)

#include <threads.h>

#else  // __STDC_NO_THREADS__

#if defined(_WIN32) && !defined(__PTHREAD__)
#include <Windows.h>
typedef DWORD tss_t;
#define thread_local __declspec(thread)
#else
#include <pthread.h>
typedef pthread_key_t tss_t;
#define thread_local __thread
typedef pthread_once_t once_flag;
#define ONCE_FLAG_INIT PTHREAD_ONCE_INIT
#endif

enum { thrd_success = 0, thrd_nomem, thrd_timedout, thrd_busy, thrd_error };

typedef void (*tss_dtor_t)(void*);

int tss_create(tss_t* key, tss_dtor_t destructor);
void tss_delete(tss_t tss_id);
void* tss_get(tss_t tss_key);
int tss_set(tss_t key, void* value);

void call_once(once_flag* flag, void (*init_func)(void));

#endif  // __STDC_NO_THREADS__

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif  // __ALIB_THREAD_LOCAL_H__
