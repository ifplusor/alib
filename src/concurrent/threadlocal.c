/**
 * threadlocal.c
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#include "alib/concurrent/threadlocal.h"

#if defined(__STDC_NO_THREADS__)

#if defined(_WIN32) && !defined(__PTHREAD__)

// FIXME: deal with auto free tls in Windows

#endif

int tss_create(tss_t* key, tss_dtor_t destructor) {
  if (key == NULL) {
    return thrd_error;
  }

#if defined(_WIN32) && !defined(__PTHREAD__)
  *key = TlsAlloc();
  if (*key == TLS_OUT_OF_INDEXES) {
    return thrd_error;
  }
  if (destructor != NULL) {
    // NOTE: windos平台下，tls不能注册自动释放函数，因此存在内存泄漏问题
    //       TLS 的解决方案只有在 DLL 框架下才有用
  }
#else
  int ret = pthread_key_create(key, destructor);
  if (ret != 0) {
    return thrd_error;
  }
#endif

  return thrd_success;
}

void tss_delete(tss_t tss_id) {
#if defined(_WIN32) && !defined(__PTHREAD__)
  TlsFree(tss_id);
#else
  (void)pthread_key_delete(tss_id);
#endif
}

void* tss_get(tss_t tss_key) {
#if defined(_WIN32) && !defined(__PTHREAD__)
  void* value = TlsGetValue(tss_key);
#else
  void* value = pthread_getspecific(tss_key);
#endif
  return value;
}

int tss_set(tss_t tss_id, void* val) {
#if defined(_WIN32) && !defined(__PTHREAD__)
  BOOL ret = TlsSetValue(tss_id, val);
  if (!ret) {
    return thrd_error;
  }
#else
  int ret = pthread_setspecific(tss_id, val);
  if (ret != 0) {
    return thrd_error;
  }
#endif
  return thrd_success;
}

#if defined(_WIN32) && !defined(__PTHREAD__)
BOOL CALLBACK InitHandleFunction(PINIT_ONCE InitOnce, PVOID Parameter, PVOID* lpContext) {
  void (*init_func)(void) = Parameter;
  init_func();
  return TRUE;
}
#endif

void call_once(once_flag* flag, void (*init_func)(void)) {
#if defined(_WIN32) && !defined(__PTHREAD__)
  (void)InitOnceExecuteOnce(flag, InitHandleFunction, init_func, NULL);
#else
  (void)pthread_once(flag, init_func);
#endif
}

#endif  // __STDC_NO_THREADS__
