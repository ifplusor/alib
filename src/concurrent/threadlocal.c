/**
 * threadlocal.c
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#include "alib/concurrent/threadlocal.h"

#if defined(_WIN32) && !defined(__PTHREAD__)

// TODO: deal with auto free tls in Windows

#endif

bool tls_create_key(tls_key_t* key, tls_destruct_func destructor) {
  if (key == NULL) {
    return false;
  }

#if defined(_WIN32) && !defined(__PTHREAD__)
  *key = ::TlsAlloc();
  if (*key == TLS_OUT_OF_INDEXES) {
    return false;
  }
  if (destructor != NULL) {
    // NOTE: windos平台下，tls不能注册自动释放函数，因此存在内存泄漏问题
    //       TLS 的解决方案只有在 DLL 框架下才有用
  }
#else
  int ret = pthread_key_create(key, destructor);
  if (ret != 0) {
    return false;
  }
#endif

  return true;
}

bool tls_delete_key(tls_key_t key) {
#if defined(_WIN32) && !defined(__PTHREAD__)
  WINBOOL ret = ::TlsFree(key);
  if (!ret) {
    return false;
  }
#else
  int ret = pthread_key_delete(key);
  if (ret != 0) {
    return false;
  }
#endif
  return true;
}

void* tls_get_value(tls_key_t key) {
#if defined(_WIN32) && !defined(__PTHREAD__)
  void* value = ::TlsGetValue(key);
#else
  void* value = pthread_getspecific(key);
#endif
  return value;
}

bool tls_set_value(tls_key_t key, void* value) {
#if defined(_WIN32) && !defined(__PTHREAD__)
  WINBOOL ret = ::TlsSetValue(key, value);
  if (!ret) {
    return false;
  }
#else
  int ret = pthread_setspecific(key, value);
  if (ret != 0) {
    return false;
  }
#endif
  return true;
}
