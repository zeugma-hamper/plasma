
/* (c)  oblong industries */

/* The 64-bit atomic operations for ARM were adapted from this file:
 *
 * https://github.com/ivmai/libatomic_ops/blob/master/src/atomic_ops/sysdeps/gcc/arm.h
 *
 * which has this license:
 *
 * Copyright (c) 1991-1994 by Xerox Corporation.  All rights reserved.
 * Copyright (c) 1996-1999 by Silicon Graphics.  All rights reserved.
 * Copyright (c) 1999-2003 by Hewlett-Packard Company. All rights reserved.
 * Copyright (c) 2008-2017 Ivan Maidanski
 *
 * THIS MATERIAL IS PROVIDED AS IS, WITH ABSOLUTELY NO WARRANTY EXPRESSED
 * OR IMPLIED.  ANY USE IS AT YOUR OWN RISK.
 *
 * Permission is hereby granted to use or copy this program
 * for any purpose,  provided the above notices are retained on all copies.
 *
 * Permission to modify the code and to distribute modified code is granted,
 * provided the above notices are retained, and a notice that the code was
 * modified is included with the above copyright notice.
 */

#include "libLoam/c/ob-atomic.h"
#include "libLoam/c/ob-pthread.h"

#if defined(__APPLE__)
#include "libkern/OSAtomic.h"
#endif

#if defined(_WIN64) || defined(__LP64__)
#define OB64
#endif

#if defined(_MSC_VER)

#ifndef _WIN64
// adapted from
// http://groups.google.com/group/comp.programming.threads/msg/969d34b276bd9b04
/**
 * If *dest == *xcmp, set *dest = *xxchg and return true.
 * else return false.
 */
static bool ob_private_cas64 (int64 *dest, const int64 *xcmp,
                              const int64 *xxchg)
{
  // clang-format off
  __asm
    { mov             esi, [xxchg]            ; exchange
      mov             ebx, [esi + 0]
      mov             ecx, [esi + 4]

      mov             esi, [xcmp]             ; comparand
      mov             eax, [esi + 0]
      mov             edx, [esi + 4]

      mov             edi, [dest]             ; destination
      lock cmpxchg8b  [edi]
      mov             eax, 0;
      setz    al;
    };
  // clang-format on
  // return value is in %eax; no return statement needed
}

// taken from
// http://groups.google.com/group/comp.programming.threads/msg/969d34b276bd9b04
/**
 * If *dest == *xcmp, set *dest = *xxchg and return true.
 * else set *xmp = *dest and return false.
 */
static bool ob_private_cas64_update (int64 *dest, int64 *xcmp,
                                     const int64 *xxchg)
{
  // clang-format off
  __asm
    { mov             esi, [xxchg]            ; exchange
      mov             ebx, [esi + 0]
      mov             ecx, [esi + 4]

      mov             esi, [xcmp]             ; comparand
      mov             eax, [esi + 0]
      mov             edx, [esi + 4]

      mov             edi, [dest]             ; destination
      lock cmpxchg8b  [edi]
      jz              yyyy;

      mov             [esi + 0], eax;
      mov             [esi + 4], edx;

yyyy:
      mov             eax, 0;
      setz    al;
    };
  // clang-format on
  // return value is in %eax; no return statement needed
}

static inline int64 val64_compare_and_swap (int64 *loc, int64 guess,
                                            int64 shall_be)
{
  ob_private_cas64_update (loc, &guess, &shall_be);
  return guess;
}

#endif  // _WIN64

/**
 * If *loc equals \a was, then set *loc to \a shall_be and return
 * true.  Otherwise, leave *loc unchanged and return false.
 */
bool ob_atomic_int64_compare_and_swap (int64 *loc, int64 was, int64 shall_be)
{
#ifdef _WIN64
  return (was == InterlockedCompareExchange64 (loc, shall_be, was));
#else
  return ob_private_cas64 (loc, &was, &shall_be);
#endif
}

/**
 * Atomically increment *loc by \a addend, and return the new value.
 */
int64 ob_atomic_int64_add (int64 *loc, int64 addend)
{
#ifdef _WIN64
  return addend + InterlockedExchangeAdd64 (loc, addend);
#else
  int64 before = ob_atomic_int64_ref (loc);
  int64 after;
  do
    {
      after = before + addend;
    }
  while (!ob_private_cas64_update (loc, &before, &after));
  return after;
#endif
}

#endif /* _MSC_VER */


#ifdef __GNUC__
#include "libLoam/c/valgrind/memcheck.h"
#define val64_compare_and_swap __sync_val_compare_and_swap
#else
#define VALGRIND_MAKE_MEM_DEFINED_IF_ADDRESSABLE(loc, len) 0
#endif

int64 ob_atomic_int64_ref (const int64 *loc)
{
  OB_MEMORY_BARRIER ();
#ifdef OB64
  return *loc;
#elif defined(__APPLE__)
  int64 *castaway = (int64 *) loc;
  return OSAtomicAdd64Barrier (0, castaway);
#elif defined(__arm__)
  int64 result;

  // clang-format off
  __asm__ __volatile__(
    "       ldrexd  %0, [%1]"
    : "=&r" (result)
    : "r" (loc)
    /* : no clobber */);
  // clang-format on
  return result;
#else
  int64 *castaway = (int64 *) loc;
  return val64_compare_and_swap (castaway, 0, 0);
#endif
}

void ob_atomic_int64_set (int64 *loc, int64 shall_be)
{
#ifdef OB64
  *loc = shall_be;
#elif defined(__APPLE__)
  int64 guess;
  do
    {
      guess = ob_atomic_int64_ref (loc);
    }
  while (!ob_atomic_int64_compare_and_swap (loc, guess, shall_be));
#elif defined(__arm__)
  int64 old_val;
  int status;

  do
    {
      // clang-format off
    __asm__ __volatile__(
      "       ldrexd  %0, [%3]\n"
      "       strexd  %1, %4, [%3]"
      : "=&r" (old_val), "=&r" (status), "+m" (*loc)
      : "r" (loc), "r" (shall_be)
      : "cc");
      // clang-format on
    }
  while (status);
#else
  /* So, here's the thing.  The only way we have to atomically write a
   * 64-bit integer is with the __sync_val_compare_and_swap instrinsic
   * (which turns into cmpxchg8b under the covers).  Okay, it's not the
   * only way, because there's also the fild/fist trick from
   * http://tinyurl.com/y9x6nuz but that ran into some semi-unknown
   * issues in bug 1421 comment 2.  Anyway, we're using cmpxchg8b
   * to implement an unconditional write.  Since cmpxchg8b is
   * conditional, though, we have to "guess" the previous contents of the
   * memory correctly in order for the cmpxchg8b to succeed.  So
   * we start with 0 as our guess, and after that we use the value
   * returned by an unsuccessful cmpxchg8b as our guess, and then
   * we just loop until we get it right.  So that means we'll do
   * one cmpxchg8b if the old value happened to be zero, we'll do
   * two cmpxchg8bs if the old value was nonzero but there was
   * no contention, and we'll do arbitrarily many (but hopefully
   * not infinite) if there is contention.
   *
   * Anyway, that's all very fascinating, but the real reason I'm
   * writing this comment is to explain the valgrind consequences.
   * Normally, valgrind is fine if you write to a location with
   * undefined contents (never been written to before), because
   * that's how you make it defined, after all.  But the thing here
   * is that we're implementing a write by doing a read!  So valgrind
   * has fits if we try to write to an undefined int64, because
   * valgrind thinks the control flow of this function depends on
   * that undefined value.  (And technically, valgrind is right,
   * since the number of loops depends on whether the old value was
   * 0 or not.  It's just that we know that this local behavior
   * doesn't affect the rest of the program, but valgrind doesn't
   * know that.)  So anyway, the upshot is this wonderful macro
   * from memcheck.h, which will make valgrind believe the previous
   * contents of the int64 are defined, even if they aren't.
   */
  (void) VALGRIND_MAKE_MEM_DEFINED_IF_ADDRESSABLE (loc, sizeof (int64));
  int64 guess = 0, actual;
  while (guess != (actual = val64_compare_and_swap (loc, guess, shall_be)))
    guess = actual;
#endif
  OB_MEMORY_BARRIER ();
}

#ifndef _MSC_VER
bool ob_atomic_int64_compare_and_swap (int64 *loc, int64 was, int64 shall_be)
{
#if defined(__APPLE__) && !defined(__LP64__)
  return OSAtomicCompareAndSwap64Barrier (was, shall_be, loc);
#elif defined(__arm__)
  int64 tmp;
  int result = 1;

  do
    {
      // clang-format off
    __asm__ __volatile__(
      "       ldrexd  %0, [%1]\n"     /* get original to r1 & r2 */
      : "=&r"(tmp)
      : "r"(loc)
      /* : no clobber */);
      // clang-format on
      if (tmp != was)
        break;
      // clang-format off
    __asm__ __volatile__(
      "       strexd  %0, %2, [%3]\n" /* store new one if matched */
      : "=&r"(result), "+m"(*loc)
      : "r"(shall_be), "r"(loc)
      : "cc");
      // clang-format on
    }
  while (result);
  return !result; /* if succeded, return 1 else 0 */
#else
  return __sync_bool_compare_and_swap (loc, was, shall_be);
#endif
}

int64 ob_atomic_int64_add (int64 *loc, int64 addend)
{
#if defined(__APPLE__) && !defined(__LP64__)
  return OSAtomicAdd64Barrier (addend, loc);
#elif defined(__arm__)
  int64 tmp;
  int status;

  do
    {
      // clang-format off
    __asm__ __volatile__(
      "       ldrexd  %0, [%1]\n"     /* get original to r1 & r2 */
      : "=&r"(tmp)
      : "r"(loc)
      /* : no clobber */);
      // clang-format on
      tmp += addend;
      // clang-format off
    __asm__ __volatile__(
      "       strexd  %0, %2, [%3]\n" /* store new one if matched */
      : "=&r"(status), "+m"(*loc)
      : "r"(tmp), "r"(loc)
      : "cc");
      // clang-format on
    }
  while (status);
  return tmp;
#else
  return __sync_add_and_fetch (loc, addend);
#endif
}
#endif /* _MSC_VER */
