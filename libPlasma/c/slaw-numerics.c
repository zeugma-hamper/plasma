
/* (c)  oblong industries */

#include "libPlasma/c/slaw.h"

#include "libPlasma/c/slaw-numeric-ilk-rumpus.h"
#include "libPlasma/c/private/plasma-private.h"
#include <string.h>
#include <stdarg.h>


#define OSIZE(type) ((sizeof (type) + 7) / 8)

#define SLAW_NUM_EMIT_SINGLETON_NOCHECK(s, type)                               \
  ((type *) (sizeof (type) > 4                                                 \
               ? ((void *) (s + 1))                                            \
               : (void *) (SLAW_SPECIAL_BYTES (s, sizeof (type)))))

#define SLAW_NUM_ALLOC_SINGLETON(ilky)                                         \
  bool wee = (sizeof (ilky) <= 4);                                             \
  struct _slaw *s = slaw_alloc (1 + (wee ? 0 : OSIZE (ilky)));                 \
  if (!s)                                                                      \
    return NULL;                                                               \
  s->o = (SLAW_##ilky | SLAW_NUMERIFY (sizeof (ilky)));                        \
  if (!wee && sizeof (ilky) != OSIZE (ilky) * 8)                               \
    (&s->o)[OSIZE (ilky)] = 0; /* make sure padding is initialized */

#define SLAW_NUM_INSTALL_SINGLETON(type, val)                                  \
  memcpy (SLAW_NUM_EMIT_SINGLETON_NOCHECK (s, type), &val, sizeof (type));     \
  return (slaw) s

#define ENSLAWIFY(type)                                                        \
  slaw slaw_##type (type val)                                                  \
  {                                                                            \
    SLAW_NUM_ALLOC_SINGLETON (type);                                           \
    SLAW_NUM_INSTALL_SINGLETON (type, val);                                    \
  }                                                                            \
                                                                               \
  bool slaw_is_##type (bslaw s)                                                \
  {                                                                            \
    return s ? (SLAW_IS_NUMERIC_ILK (s, type, false)) : false;                 \
  }                                                                            \
                                                                               \
  bool slaw_is_##type##_array (bslaw s)                                        \
  {                                                                            \
    return s ? (SLAW_IS_NUMERIC_ILK (s, type, true)) : false;                  \
  }                                                                            \
                                                                               \
  const type *slaw_##type##_emit_nocheck (bslaw s)                             \
  {                                                                            \
    return s ? SLAW_NUM_EMIT_SINGLETON_NOCHECK (s, type) : NULL;               \
  }                                                                            \
                                                                               \
  const type *slaw_##type##_emit (bslaw s)                                     \
  {                                                                            \
    if (!s || !SLAW_IS_NUMERIC_ILK (s, type, false))                           \
      return NULL;                                                             \
    return SLAW_NUM_EMIT_SINGLETON_NOCHECK (s, type);                          \
  }                                                                            \
  slaw slaw_##type##_array_raw (int64 N, type **array_out)                     \
  {                                                                            \
    slaw sarr = slaw_numeric_array_raw (SLAW_##type, sizeof (type), N);        \
    if (array_out)                                                             \
      *array_out = (type *) SLAW_N_ARRAY_FIRST_ELEM (sarr);                    \
    return sarr;                                                               \
  }                                                                            \
                                                                               \
  slaw slaw_##type##_array_empty (int64 N)                                     \
  {                                                                            \
    slaw sarr = slaw_numeric_array_raw (SLAW_##type, sizeof (type), N);        \
    slaw s, first;                                                             \
    if (sarr && (first = (slaw) SLAW_N_ARRAY_FIRST_ELEM (sarr))                \
        && (s = sarr + slaw_octlen (sarr)))                                    \
      memset (first, 0, 8 * (s - first));                                      \
    return sarr;                                                               \
  }                                                                            \
                                                                               \
  slaw slaw_##type##_array_filled (int64 N, type val)                          \
  {                                                                            \
    slaw sarr = slaw_numeric_array_raw (SLAW_##type, sizeof (type), N);        \
    type *elem;                                                                \
    if (sarr && (elem = (type *) SLAW_N_ARRAY_FIRST_ELEM (sarr)))              \
      {                                                                        \
        for (; N > 0; N--)                                                     \
          {                                                                    \
            memcpy (elem, &val, sizeof (type));                                \
            elem++;                                                            \
          }                                                                    \
      }                                                                        \
    return sarr;                                                               \
  }                                                                            \
                                                                               \
  slaw slaw_##type##_array (const type *src, int64 N)                          \
  {                                                                            \
    if (!src)                                                                  \
      return NULL;                                                             \
    slaw sarr = slaw_numeric_array_raw (SLAW_##type, sizeof (type), N);        \
    type *elem;                                                                \
    if (sarr && (elem = (type *) SLAW_N_ARRAY_FIRST_ELEM (sarr)))              \
      memcpy (elem, src, N * sizeof (type));                                   \
    return sarr;                                                               \
  }                                                                            \
                                                                               \
  const type *slaw_##type##_array_emit (bslaw s)                               \
  {                                                                            \
    if (!s)                                                                    \
      return NULL;                                                             \
    if (!SLAW_IS_NUMERIC_ILK (s, type, true))                                  \
      return NULL;                                                             \
    if (SLAW_N_ARRAY_BREADTH (s) < 1)                                          \
      return NULL;                                                             \
    return ((type *) SLAW_N_ARRAY_FIRST_ELEM (s));                             \
  }                                                                            \
                                                                               \
  const type *slaw_##type##_array_emit_nth (bslaw s, int64 N)                  \
  {                                                                            \
    if (!s)                                                                    \
      return NULL;                                                             \
    if (!SLAW_IS_NUMERIC_ILK (s, type, true))                                  \
      return NULL;                                                             \
    if (N >= (int64) SLAW_N_ARRAY_BREADTH (s))                                 \
      return NULL;                                                             \
    return ((type *) SLAW_N_ARRAY_FIRST_ELEM (s)) + N;                         \
  }                                                                            \
                                                                               \
  slaw slaw_##type##_arrays_concat_f (slaw s1, ...)                            \
  {                                                                            \
    slabu *sb = NULL;                                                          \
    ob_retort err = OB_OK;                                                     \
    if (!s1)                                                                   \
      return NULL;                                                             \
    if (!(sb = slabu_new ()))                                                  \
      return NULL;                                                             \
    va_list vargies;                                                           \
    va_start (vargies, s1);                                                    \
    slaw s = s1;                                                               \
    int64 N, q, br = 0;                                                        \
    while (s)                                                                  \
      {                                                                        \
        if (!SLAW_IS_NUMERIC_ILK (s, type, true))                              \
          continue;                                                            \
        ob_err_accum (&err, slabu_list_add_x (sb, s));                         \
        br += SLAW_N_ARRAY_BREADTH (s);                                        \
        s = va_arg (vargies, slaw);                                            \
      }                                                                        \
    va_end (vargies);                                                          \
    if (err < OB_OK)                                                           \
      {                                                                        \
        slabu_free (sb);                                                       \
        return NULL;                                                           \
      }                                                                        \
    slaw sarr = slaw_numeric_array_raw (SLAW_##type, sizeof (type), br);       \
    if (!sarr)                                                                 \
      {                                                                        \
        slabu_free (sb);                                                       \
        return NULL;                                                           \
      }                                                                        \
    type *src, *elem = (type *) SLAW_N_ARRAY_FIRST_ELEM (sarr);                \
    for (q = 0, s = sb->esses[0]; q < sb->numEsses; q++, s = sb->esses[q])     \
      {                                                                        \
        src = (type *) SLAW_N_ARRAY_FIRST_ELEM (s);                            \
        N = SLAW_N_ARRAY_BREADTH (s);                                          \
        memcpy (elem, src, N * sizeof (type));                                 \
        elem += N;                                                             \
      }                                                                        \
    slabu_free (sb);                                                           \
    return sarr;                                                               \
  }                                                                            \
                                                                               \
  slaw slaw_##type##_arrays_concat (bslaw s1, ...)                             \
  {                                                                            \
    slabu *sb = NULL;                                                          \
    ob_retort err = OB_OK;                                                     \
    if (!s1)                                                                   \
      return NULL;                                                             \
    if (!(sb = slabu_new ()))                                                  \
      return NULL;                                                             \
    va_list vargies;                                                           \
    va_start (vargies, s1);                                                    \
    bslaw s = s1;                                                              \
    int64 N, q, br = 0;                                                        \
    while (s)                                                                  \
      {                                                                        \
        if (!SLAW_IS_NUMERIC_ILK (s, type, true))                              \
          continue;                                                            \
        ob_err_accum (&err, slabu_list_add_z (sb, s));                         \
        br += SLAW_N_ARRAY_BREADTH (s);                                        \
        s = va_arg (vargies, slaw);                                            \
      }                                                                        \
    va_end (vargies);                                                          \
    if (err < OB_OK)                                                           \
      {                                                                        \
        slabu_free (sb);                                                       \
        return NULL;                                                           \
      }                                                                        \
    slaw sarr = slaw_numeric_array_raw (SLAW_##type, sizeof (type), br);       \
    if (!sarr)                                                                 \
      {                                                                        \
        slabu_free (sb);                                                       \
        return NULL;                                                           \
      }                                                                        \
    type *src, *elem = (type *) SLAW_N_ARRAY_FIRST_ELEM (sarr);                \
    for (q = 0, s = sb->esses[0]; q < sb->numEsses; q++, s = sb->esses[q])     \
      {                                                                        \
        src = (type *) SLAW_N_ARRAY_FIRST_ELEM (s);                            \
        N = SLAW_N_ARRAY_BREADTH (s);                                          \
        memcpy (elem, src, N * sizeof (type));                                 \
        elem += N;                                                             \
      }                                                                        \
    slabu_free (sb);                                                           \
    return sarr;                                                               \
  }                                                                            \
                                                                               \
  slaw slaw_##type##_array_concat_carray_f (slaw s, const type *src, int64 N)  \
  {                                                                            \
    if (!s)                                                                    \
      return NULL;                                                             \
    slaw s2 = slaw_##type##_array (src, N);                                    \
    slaw sarr = slaw_##type##_arrays_concat_f (s, s2, NULL);                   \
    if (!sarr)                                                                 \
      slaw_free (s2);                                                          \
    return sarr;                                                               \
  }                                                                            \
                                                                               \
  slaw slaw_##type##_array_concat_carray (bslaw s, const type *src, int64 N)   \
  {                                                                            \
    if (!s)                                                                    \
      return NULL;                                                             \
    slaw s2 = slaw_##type##_array (src, N);                                    \
    slaw sarr = slaw_##type##_arrays_concat (s, s2, NULL);                     \
    slaw_free (s2);                                                            \
    return sarr;                                                               \
  }


ENSLAWIFY (int8);
ENSLAWIFY (unt8);
ENSLAWIFY (int16);
ENSLAWIFY (unt16);

ENSLAWIFY (int32);
ENSLAWIFY (unt32);
ENSLAWIFY (int64);
ENSLAWIFY (unt64);
ENSLAWIFY (float32);
ENSLAWIFY (float64);


ENSLAWIFY (int8c);
ENSLAWIFY (unt8c);
ENSLAWIFY (int16c);
ENSLAWIFY (unt16c);

ENSLAWIFY (int32c);
ENSLAWIFY (unt32c);
ENSLAWIFY (int64c);
ENSLAWIFY (unt64c);
ENSLAWIFY (float32c);
ENSLAWIFY (float64c);


ENSLAWIFY (v2int8);
ENSLAWIFY (v2unt8);
ENSLAWIFY (v2int16);
ENSLAWIFY (v2unt16);

ENSLAWIFY (v2int32);
ENSLAWIFY (v2unt32);
ENSLAWIFY (v2int64);
ENSLAWIFY (v2unt64);
ENSLAWIFY (v2float32);
ENSLAWIFY (v2float64);


ENSLAWIFY (v3int8);
ENSLAWIFY (v3unt8);
ENSLAWIFY (v3int16);
ENSLAWIFY (v3unt16);

ENSLAWIFY (v3int32);
ENSLAWIFY (v3unt32);
ENSLAWIFY (v3int64);
ENSLAWIFY (v3unt64);
ENSLAWIFY (v3float32);
ENSLAWIFY (v3float64);


ENSLAWIFY (v4int8);
ENSLAWIFY (v4unt8);
ENSLAWIFY (v4int16);
ENSLAWIFY (v4unt16);

ENSLAWIFY (v4int32);
ENSLAWIFY (v4unt32);
ENSLAWIFY (v4int64);
ENSLAWIFY (v4unt64);
ENSLAWIFY (v4float32);
ENSLAWIFY (v4float64);


ENSLAWIFY (v2int8c);
ENSLAWIFY (v2unt8c);
ENSLAWIFY (v2int16c);
ENSLAWIFY (v2unt16c);

ENSLAWIFY (v2int32c);
ENSLAWIFY (v2unt32c);
ENSLAWIFY (v2int64c);
ENSLAWIFY (v2unt64c);
ENSLAWIFY (v2float32c);
ENSLAWIFY (v2float64c);


ENSLAWIFY (v3int8c);
ENSLAWIFY (v3unt8c);
ENSLAWIFY (v3int16c);
ENSLAWIFY (v3unt16c);

ENSLAWIFY (v3int32c);
ENSLAWIFY (v3unt32c);
ENSLAWIFY (v3int64c);
ENSLAWIFY (v3unt64c);
ENSLAWIFY (v3float32c);
ENSLAWIFY (v3float64c);


ENSLAWIFY (v4int8c);
ENSLAWIFY (v4unt8c);
ENSLAWIFY (v4int16c);
ENSLAWIFY (v4unt16c);

ENSLAWIFY (v4int32c);
ENSLAWIFY (v4unt32c);
ENSLAWIFY (v4int64c);
ENSLAWIFY (v4unt64c);
ENSLAWIFY (v4float32c);
ENSLAWIFY (v4float64c);


ENSLAWIFY (m2int8);
ENSLAWIFY (m2unt8);
ENSLAWIFY (m2int16);
ENSLAWIFY (m2unt16);

ENSLAWIFY (m2int32);
ENSLAWIFY (m2unt32);
ENSLAWIFY (m2int64);
ENSLAWIFY (m2unt64);
ENSLAWIFY (m2float32);
ENSLAWIFY (m2float64);


ENSLAWIFY (m3int8);
ENSLAWIFY (m3unt8);
ENSLAWIFY (m3int16);
ENSLAWIFY (m3unt16);

ENSLAWIFY (m3int32);
ENSLAWIFY (m3unt32);
ENSLAWIFY (m3int64);
ENSLAWIFY (m3unt64);
ENSLAWIFY (m3float32);
ENSLAWIFY (m3float64);


ENSLAWIFY (m4int8);
ENSLAWIFY (m4unt8);
ENSLAWIFY (m4int16);
ENSLAWIFY (m4unt16);

ENSLAWIFY (m4int32);
ENSLAWIFY (m4unt32);
ENSLAWIFY (m4int64);
ENSLAWIFY (m4unt64);
ENSLAWIFY (m4float32);
ENSLAWIFY (m4float64);


ENSLAWIFY (m5int8);
ENSLAWIFY (m5unt8);
ENSLAWIFY (m5int16);
ENSLAWIFY (m5unt16);

ENSLAWIFY (m5int32);
ENSLAWIFY (m5unt32);
ENSLAWIFY (m5int64);
ENSLAWIFY (m5unt64);
ENSLAWIFY (m5float32);
ENSLAWIFY (m5float64);


#undef ENSLAWIFY
#undef SLAW_NUM_EMIT_SINGLETON
#undef SLAW_NUM_INSTALL_SINGLETON
#undef SLAW_NUM_ALLOC_SINGLETON



slaw slaw_numeric_array_raw (slaw_oct unit_ilk, unt32 unit_blen, unt64 breadth)
{
  if (unit_blen < 1 || (unit_blen > SLAW_NUMERIC_MAX_UNIT_BSIZE + 1))
    return NULL;
  slaw s;
  unt64 arr_olen = ((unit_blen * breadth) + 7) / 8;
  s = slaw_alloc (2 + arr_olen);
  if (!s)
    return NULL;
  s->o =
    (unit_ilk | SLAW_NUMERIFY (unit_blen) | breadth | SLAW_NUMERIC_ARRAY_ILK);

  if (0 != (7 & (unit_blen * breadth)))
    (&s->o)[arr_olen] = 0; /* make sure padding is initialized */

  return s;
}


int64 slaw_numeric_array_count (bslaw s)
{
  if (!s || !SLAW_IS_NUMERIC_ARRAY (s))
    return -1;
  return SLAW_N_ARRAY_BREADTH (s);
}

// Oh, slippery NULL handling:
// https://gitlab.oblong.com/platform/docs/-/wikis/libPlasma-and-NULL
const void *slaw_numeric_array_emit (bslaw s)
{
  if (!s)
    return NULL;
  if (!SLAW_IS_NUMERIC_ARRAY (s))
    return NULL;
  return SLAW_N_ARRAY_FIRST_ELEM (s);
}


bool slaw_is_numeric (bslaw s)
{
  return s ? SLAW_IS_NUMERIC (s) : false;
}



bool slaw_is_numeric_8 (bslaw s)
{
  return (!s) ? false : SLAW_IS_NUMERIC_8BIT (s);
}

bool slaw_is_numeric_16 (bslaw s)
{
  return (!s) ? false : SLAW_IS_NUMERIC_16BIT (s);
}

bool slaw_is_numeric_32 (bslaw s)
{
  return (!s) ? false : SLAW_IS_NUMERIC_32BIT (s);
}

bool slaw_is_numeric_64 (bslaw s)
{
  return (!s) ? false : SLAW_IS_NUMERIC_64BIT (s);
}

bool slaw_is_numeric_float (bslaw s)
{
  return (!s) ? false : SLAW_IS_NUMERIC_FLOAT (s);
}

bool slaw_is_numeric_int (bslaw s)
{
  return (!s) ? false : (SLAW_IS_NUMERIC_INT (s));
}

bool slaw_is_numeric_unt (bslaw s)
{
  return (!s) ? false : (SLAW_IS_NUMERIC_UNT (s));
}

bool slaw_is_numeric_complex (bslaw s)
{
  return (!s) ? false : SLAW_IS_NUMERIC_COMPLEX (s);
}

bool slaw_is_numeric_vector (bslaw s)
{
  return (!s) ? false : SLAW_IS_NUMERIC_VECTOR (s);
}

bool slaw_is_numeric_multivector (bslaw s)
{
  return (!s) ? false : SLAW_IS_NUMERIC_MVEC (s);
}

bool slaw_is_numeric_array (bslaw s)
{
  return (!s) ? false : SLAW_IS_NUMERIC_ARRAY (s);
}

int slaw_numeric_vector_dimension (bslaw s)
{
  return (s ? SLAW_NUMERIC_VECTOR_WIDTH (s) : 0);
}

int slaw_numeric_unit_bsize (bslaw s)
{
  return (slaw_is_numeric (s) ? SLAW_NUMERIC_UNIT_BSIZE (s) : 0);
}

const void *slaw_numeric_nonarray_emit (bslaw s)
{
  unt32 bsize = SLAW_NUMERIC_UNIT_BSIZE (s);
  if (bsize > 4)
    return (const void *) (s + 1);
  else
    return SLAW_SPECIAL_BYTES (s, bsize);
}
