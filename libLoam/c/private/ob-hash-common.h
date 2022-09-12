
/* (c)  oblong industries */

#ifndef OB_HASH_COMMON_COLD
#define OB_HASH_COMMON_COLD

#include "libLoam/c/ob-hash.h"

/* hash stuff is so important, we want gcc to always optimize it */

#if defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 404)
#pragma GCC optimize("O3", "inline-limit=1000000", "strict-aliasing",          \
                     "omit-frame-pointer")
#endif

#define INLINIEST static inline OB_ALWAYS_INLINE OB_HOT

/* This endianness detection stuff is from the Bob Jenkins code and
 * is a little scary.  It could probably be simplified. */

#ifdef linux
#include <endian.h> /* attempt to define endianness */
#endif

/*
 * My best guess at if you are big-endian or little-endian.  This may
 * need adjustment.
 */
#if (defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN)                         \
     && __BYTE_ORDER == __LITTLE_ENDIAN)                                       \
  || (defined(i386) || defined(__i386__) || defined(__i486__)                  \
      || defined(__i586__) || defined(__i686__) || defined(__LITTLE_ENDIAN__)  \
      || defined(_M_IX86) || defined(_M_X64) || defined(_M_AMD64)              \
      || defined(_M_IA64) || defined(vax) || defined(MIPSEL))
#define HASH_LITTLE_ENDIAN 1
#define HASH_BIG_ENDIAN 0
#elif (defined(__BYTE_ORDER) && defined(__BIG_ENDIAN)                          \
       && __BYTE_ORDER == __BIG_ENDIAN)                                        \
  || (defined(sparc) || defined(POWERPC) || defined(mc68000) || defined(sel)   \
      || defined(__BIG_ENDIAN__))
#define HASH_LITTLE_ENDIAN 0
#define HASH_BIG_ENDIAN 1
#else
#define HASH_LITTLE_ENDIAN 0
#define HASH_BIG_ENDIAN 0
#endif

/* In the original Google code, HashLen16() was a thin wrapper around
 * Hash128to64(), but I just smushed them into one function. */

// Hash 128 input bits down to 64 bits of output.
// This is intended to be a reasonably good hash function.
#define HashLen16 ob_hash_2xunt64_to_unt64

#endif /* OB_HASH_COMMON_COLD */
