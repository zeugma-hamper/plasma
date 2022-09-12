
/* (c)  oblong industries */

#ifndef OB_HASH_BROWNS
#define OB_HASH_BROWNS

#include <stddef.h>  // for size_t
#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-api.h"
#include "libLoam/c/ob-attrs.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Hashes the specified \a key (of \a length bytes) using the
 * Bob Jenkins "lookup3" algorithm.  \a initval is the initial
 * value to use.
 */
OB_LOAM_API OB_HOT unt32 ob_jenkins_hash (const void *key, size_t length,
                                          unt32 initval);

/**
 * Hashes the specified \a key (of \a length bytes) using the
 * Bob Jenkins "lookup3" algorithm.  Produces two different
 * 32-bit hashes.  On entry, \a inout1 and \a inout2 contain the
 * two initial values, and on return they contain the two
 * hash values.
 */
OB_LOAM_API OB_HOT void ob_jenkins_hash2 (const void *key, size_t length,
                                          unt32 *inout1, unt32 *inout2);

/**
 * Hashes the specified \a key (of \a length bytes) using the
 * CityHash64 algorithm developed by Google.
 *
 * \note This implements CityHash v1.0.2, which was released on May 8, 2011.
 * In particular, note that it produces different hash values than the
 * subsequent version of CityHash, v1.0.3, which was released on
 * October 6, 2011.
 */
OB_LOAM_API OB_HOT unt64 ob_city_hash64 (const void *key, size_t length);

/**
 * Hashes the specified \a key (of \a length bytes) using the
 * CityHash64 algorithm developed by Google.  The hash can be
 * seeded with \a seed0 and \a seed1.  If you only need one seed,
 * Google recommends putting your seed in \a seed1, and passing
 * OB_CONST_U64(0x9ae16a3b2f90404f) for seed0.
 */
OB_LOAM_API OB_HOT unt64 ob_city_hash64_with_seeds (const void *key,
                                                    size_t length, unt64 seed0,
                                                    unt64 seed1);

/**
 * Given a 64-bit integer \a key, returns a 64-bit hash code.
 * This is a good hash that achieves avalanche, so it's possible
 * to use only a portion of the hash code (e. g. truncate to 32
 * bits, or use in a power-of-two hash table).
 * (Beware that the integer hash functions provided by TR1 and
 * Boost generally do not have this property.)
 */
static inline unt64 ob_hash_unt64 (unt64 key)
{
  unt64 h = key;
  // This is the 64-bit finalizer from MurmurHash3
  // http://code.google.com/p/smhasher/wiki/MurmurHash3
  // (public domain, Austin Appleby)
  h ^= h >> 33;
  h *= OB_CONST_U64 (0xff51afd7ed558ccd);
  h ^= h >> 33;
  h *= OB_CONST_U64 (0xc4ceb9fe1a85ec53);
  h ^= h >> 33;
  return h;
}

/**
 * Given a 32-bit integer, returns a 32-bit hash code.
 */
static inline unt32 ob_hash_unt32 (unt32 key)
{
  unt32 h = key;
  /* murmur3 32-bit finalizer */
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;
  return h;
}

/**
 * Given a size_t integer, returns a size_t hash code.
 */
static inline size_t ob_hash_size_t (size_t key)
{
#if defined(__LP64__) || defined(_WIN64)
  return ob_hash_unt64 (key);
#else
  return ob_hash_unt32 (key);
#endif
}

/**
 * Given a 64-bit integer, returns a size_t hash code.
 */
static inline size_t ob_hash_unt64_to_size_t (unt64 key)
{
  return (size_t) ob_hash_unt64 (key);
}

/**
 * Given a 32-bit integer, returns a size_t hash code.
 */
static inline size_t ob_hash_unt32_to_size_t (unt32 key)
{
  return ob_hash_size_t (key);
}

/* ob_hash_2xunt64_to_unt64() comes from HashLen16() and Hash128to64()
 * in CityHash, so reproduce Google copyright here: */

// Copyright (c) 2011 Google, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// CityHash, by Geoff Pike and Jyrki Alakuijala

/**
 * Given two 64-bit integers, returns an unt64 hash code.
 */
static inline unt64 ob_hash_2xunt64_to_unt64 (unt64 u, unt64 v)
{
  // Murmur-inspired hashing.
  const unt64 kMul = OB_CONST_U64 (0x9ddfea08eb382d69);
  unt64 a = (u ^ v) * kMul;
  a ^= (a >> 47);
  unt64 b = (v ^ a) * kMul;
  b ^= (b >> 47);
  b *= kMul;
  return b;
}

/**
 * Given two size_t integers, returns a size_t hash code.
 */
static inline size_t ob_hash_2xsize_t_to_size_t (size_t u, size_t v)
{
#ifdef __LP64__
  return ob_hash_2xunt64_to_unt64 (u, v);
#else
  unt64 x = u;
  x <<= 32;
  x ^= v;
  return (size_t) ob_hash_unt64 (x);
#endif
}

#ifdef __cplusplus
}
#endif

#endif /* OB_HASH_BROWNS */
