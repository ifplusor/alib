/**
 * threadlocal.h - thread specific data
 */

#ifndef _ALIB_THREADLOCAL_H_
#define _ALIB_THREADLOCAL_H_

#include "acom.h"

#ifdef __cplusplus
extren "C" {
#endif

#if defined(_WIN32) && !defined(__PTHREAD__)
#include <Windows.h>

typedef DWORD tls_key_t;

#define ALIB_TLS_DEFN __declspec(thread)

#else
#include <pthread.h>

typedef pthread_key_t tls_key_t;

#define ALIB_TLS_DEFN __thread

#endif


typedef void (*tls_destruct_func)(void*);

bool tls_create_key(tls_key_t *key, tls_destruct_func destructor);
bool tls_delete_key(tls_key_t key);
void *tls_get_value(tls_key_t key);
bool tls_set_value(tls_key_t key, void *value);


#ifdef __cplusplus
};
#endif

#endif //_ALIB_THREADLOCAL_H_
