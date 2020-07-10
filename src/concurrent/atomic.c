/**
 * atomic.c
 *
 * @author James Yin <ywhjames@hotmail.com>
 */
#include "alib/concurrent/atomic.h"

#if defined(__STDC_NO_ATOMICS__)

long atomic_long_load(volatile atomic_long* atom) {
#if defined(_WIN32)
  return InterlockedAdd(atom, 0);
#endif  // _WIN32
}

void atomic_long_store(volatile atomic_long* atom, long value) {
#if defined(_WIN32)
  InterlockedExchange(atom, value);
#endif  // _WIN32
}

long atomic_long_fetch_add(volatile atomic_long* atom, long value) {
#if defined(_WIN32)
  return InterlockedExchangeAdd(atom, value) - value;
#endif  // _WIN32
}

long atomic_long_fetch_sub(volatile atomic_long* atom, long value) {
  return atomic_long_fetch_add(atom, -value);
}

llong atomic_llong_load(volatile atomic_llong* atom) {
#if defined(_WIN32)
  return InterlockedAdd64(atom, 0);
#endif  // _WIN32
}

void atomic_llong_store(volatile atomic_llong* atom, llong value) {
#if defined(_WIN32)
  InterlockedExchange64(atom, value);
#endif  // _WIN32
}

llong atomic_llong_fetch_add(volatile atomic_llong* atom, llong value) {
#if defined(_WIN32)
  return InterlockedExchangeAdd64(atom, value) - value;
#endif  // _WIN32
}

llong atomic_llong_fetch_sub(volatile atomic_llong* atom, llong value) {
  return atomic_llong_fetch_add(atom, -value);
}

#endif  // __STDC_NO_ATOMICS__
