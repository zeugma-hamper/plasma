#include "libLoam/c/private/ob-hash-common.h"
#include "libLoam/c/ob-endian.h"
#include <string.h>

/* Original is from http://code.google.com/p/cityhash/
 * This has been modified from the original to fit into libLoam more nicely,
 * such as by using ob_swap functions, and we reuse the endian detection
 * from the Jenkins hash.  (Though both points are somewhat moot,
 * since all of Oblong's supported platforms are little endian.)
 * Additionally, I converted CityHash from C++ to C, but that was very
 * straightforward, since it was written in very C-like C++.
 * And this is just CityHash64; the larger versions are omitted.
 */

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
//
// This file provides CityHash64() and related functions.
//
// It's probably possible to create even faster hash functions by
// writing a program that systematically explores some of the space of
// possible hash functions, by using SIMD instructions, or by
// compromising on hash quality.

#define STATIC_CAST(type, castee) ((type)(castee))

typedef struct
{ unt64 first;
  unt64 second;
} unt128;

INLINIEST unt64 UNALIGNED_LOAD64(const char *p) {
  unt64 result;
  memcpy(&result, p, sizeof(result));
  return result;
}

INLINIEST unt32 UNALIGNED_LOAD32(const char *p) {
  unt32 result;
  memcpy(&result, p, sizeof(result));
  return result;
}

#if HASH_LITTLE_ENDIAN

#define uint32_in_expected_order(x) (x)
#define uint64_in_expected_order(x) (x)

#elif HASH_BIG_ENDIAN

#define uint32_in_expected_order(x) (ob_swap32 (x))
#define uint64_in_expected_order(x) (ob_swap64 (x))

#else

#error Fiddle with the code to get big endian or little endian to be true!

#endif  // HASH_LITTLE_ENDIAN

INLINIEST unt64 Fetch64(const char *p) {
  return uint64_in_expected_order(UNALIGNED_LOAD64(p));
}

INLINIEST unt32 Fetch32(const char *p) {
  return uint32_in_expected_order(UNALIGNED_LOAD32(p));
}

// Some primes between 2^63 and 2^64 for various uses.
static const unt64 k0 = 0xc3a5c85c97cb3127ULL;
static const unt64 k1 = 0xb492b66fbe98f273ULL;
static const unt64 k2 = 0x9ae16a3b2f90404fULL;
static const unt64 k3 = 0xc949d7c7509e6557ULL;

// Bitwise right rotate.  Normally this will compile to a single
// instruction, especially if the shift is a manifest constant.
INLINIEST unt64 Rotate(unt64 val, int shift) {
  // Avoid shifting by 64: doing so yields an undefined result.
  return shift == 0 ? val : ((val >> shift) | (val << (64 - shift)));
}

// Equivalent to Rotate(), but requires the second arg to be non-zero.
// On x86-64, and probably others, it's possible for this to compile
// to a single instruction if both args are already in registers.
INLINIEST unt64 RotateByAtLeast1(unt64 val, int shift) {
  return (val >> shift) | (val << (64 - shift));
}

INLINIEST unt64 ShiftMix(unt64 val) {
  return val ^ (val >> 47);
}

INLINIEST unt64 HashLen0to16(const char *s, size_t len) {
  if (len > 8) {
    unt64 a = Fetch64(s);
    unt64 b = Fetch64(s + len - 8);
    return HashLen16(a, RotateByAtLeast1(b + len, len)) ^ b;
  }
  if (len >= 4) {
    unt64 a = Fetch32(s);
    return HashLen16(len + (a << 3), Fetch32(s + len - 4));
  }
  if (len > 0) {
    unt8 a = s[0];
    unt8 b = s[len >> 1];
    unt8 c = s[len - 1];
    unt32 y = STATIC_CAST (unt32, a) + (STATIC_CAST (unt32, b) << 8);
    unt32 z = len + (STATIC_CAST (unt32, c) << 2);
    return ShiftMix(y * k2 ^ z * k3) * k2;
  }
  return k2;
}

// This probably works well for 16-byte strings as well, but it may be overkill
// in that case.
INLINIEST unt64 HashLen17to32(const char *s, size_t len) {
  unt64 a = Fetch64(s) * k1;
  unt64 b = Fetch64(s + 8);
  unt64 c = Fetch64(s + len - 8) * k2;
  unt64 d = Fetch64(s + len - 16) * k0;
  return HashLen16(Rotate(a - b, 43) + Rotate(c, 30) + d,
                   a + Rotate(b ^ k3, 20) - c + len);
}

// Return a 16-byte hash for 48 bytes.  Quick and dirty.
// Callers do best to use "random-looking" values for a and b.
INLINIEST unt128 HashFor48Bytes(
    unt64 w, unt64 x, unt64 y, unt64 z, unt64 a, unt64 b) {
  a += w;
  b = Rotate(b + a + z, 21);
  unt64 c = a;
  a += x;
  a += y;
  b += Rotate(a, 44);
  unt128 result;
  result.first = a + z;
  result.second = b + c;
  return result;
}

// Return a 16-byte hash for s[0] ... s[31], a, and b.  Quick and dirty.
INLINIEST unt128 WeakHashLen32WithSeeds(
    const char* s, unt64 a, unt64 b) {
  return HashFor48Bytes(Fetch64(s),
                        Fetch64(s + 8),
                        Fetch64(s + 16),
                        Fetch64(s + 24),
                        a,
                        b);
}

// Return an 8-byte hash for 33 to 64 bytes.
INLINIEST unt64 HashLen33to64(const char *s, size_t len) {
  unt64 z = Fetch64(s + 24);
  unt64 a = Fetch64(s) + (len + Fetch64(s + len - 16)) * k0;
  unt64 b = Rotate(a + z, 52);
  unt64 c = Rotate(a, 37);
  a += Fetch64(s + 8);
  c += Rotate(a, 7);
  a += Fetch64(s + 16);
  unt64 vf = a + z;
  unt64 vs = b + Rotate(a, 31) + c;
  a = Fetch64(s + 16) + Fetch64(s + len - 32);
  z = Fetch64(s + len - 8);
  b = Rotate(a + z, 52);
  c = Rotate(a, 37);
  a += Fetch64(s + len - 24);
  c += Rotate(a, 7);
  a += Fetch64(s + len - 16);
  unt64 wf = a + z;
  unt64 ws = b + Rotate(a, 31) + c;
  unt64 r = ShiftMix((vf + ws) * k2 + (wf + vs) * k0);
  return ShiftMix(r * k0 + vs) * k2;
}

unt64 ob_city_hash64(const void *key, size_t len) {
  const char *s = (const char *) key;

  if (len <= 32) {
    if (len <= 16) {
      return HashLen0to16(s, len);
    } else {
      return HashLen17to32(s, len);
    }
  } else if (len <= 64) {
    return HashLen33to64(s, len);
  }

  // For strings over 64 bytes we hash the end first, and then as we
  // loop we keep 56 bytes of state: v, w, x, y, and z.
  unt64 x = Fetch64(s);
  unt64 y = Fetch64(s + len - 16) ^ k1;
  unt64 z = Fetch64(s + len - 56) ^ k0;
  unt128 v = WeakHashLen32WithSeeds(s + len - 64, len, y);
  unt128 w = WeakHashLen32WithSeeds(s + len - 32, len * k1, k0);
  z += ShiftMix(v.second) * k1;
  x = Rotate(z + x, 39) * k1;
  y = Rotate(y, 33) * k1;

  // Decrease len to the nearest multiple of 64, and operate on 64-byte chunks.
  len = (len - 1) & ~STATIC_CAST (size_t, 63);
  do {
    x = Rotate(x + y + v.first + Fetch64(s + 16), 37) * k1;
    y = Rotate(y + v.second + Fetch64(s + 48), 42) * k1;
    x ^= w.second;
    y ^= v.first;
    z = Rotate(z ^ w.first, 33);
    v = WeakHashLen32WithSeeds(s, v.second * k1, x + w.first);
    w = WeakHashLen32WithSeeds(s + 32, z + w.second, y);
    const unt64 tmp = z;
    z = x;
    x = tmp;
    s += 64;
    len -= 64;
  } while (len != 0);
  return HashLen16(HashLen16(v.first, w.first) + ShiftMix(y) * k1 + z,
                   HashLen16(v.second, w.second) + x);
}
