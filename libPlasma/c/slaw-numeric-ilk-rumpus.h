
/* (c)  oblong industries */

#ifndef SLAW_NUMERIC_ILK_RUMPUS_HORSEPLAY
#define SLAW_NUMERIC_ILK_RUMPUS_HORSEPLAY

#include "libLoam/c/ob-api.h"

/*
 the following gubbish should make, e.g.,

 slaw slaw_int32 (int32 val);
 bool slaw_is_int32 (bslaw s);
 bool slaw_is_int32_array (bslaw s);
 const int32 *slaw_int32_emit (bslaw s);
 const int32 *slaw_int32_emit_nocheck (bslaw s);
 slaw slaw_int32_array_raw (int64 N, int32 **array_out);
 slaw slaw_int32_array_empty (int64 N);
 slaw slaw_int32_array_filled (int64 N, int32 val);
 slaw slaw_int32_array (const int32 *src, int64 N);
 const int32 *slaw_int32_array_emit (bslaw s);
 const int32 *slaw_int32_array_emit_nth (bslaw s, int64 N);

 slaw slaw_int32_arrays_concat_f (slaw s1, ...);
 slaw slaw_int32_arrays_concat (bslaw s1, ...);
 slaw slaw_int32_array_concat_carray_f (slaw s, const int32 *src, int64 N);
 slaw slaw_int32_array_concat_carray (bslaw s, const int32 *src, int64 N);
*/

#define SLAWBEGET(type)                                                        \
  OB_PLASMA_API slaw slaw_##type (type val);                                   \
  OB_PLASMA_API bool slaw_is_##type (bslaw s);                                 \
  OB_PLASMA_API bool slaw_is_##type##_array (bslaw s);                         \
  OB_PLASMA_API const type *slaw_##type##_emit_nocheck (bslaw s);              \
  OB_PLASMA_API const type *slaw_##type##_emit (bslaw s);                      \
  OB_PLASMA_API slaw slaw_##type##_array_raw (int64 N, type **array_out);      \
  OB_PLASMA_API slaw slaw_##type##_array_empty (int64 N);                      \
  OB_PLASMA_API slaw slaw_##type##_array_filled (int64 N, type val);           \
  OB_PLASMA_API slaw slaw_##type##_array (const type *src, int64 N);           \
  OB_PLASMA_API const type *slaw_##type##_array_emit (bslaw s);                \
  OB_PLASMA_API const type *slaw_##type##_array_emit_nth (bslaw s, int64 N);   \
                                                                               \
  OB_PLASMA_API slaw slaw_##type##_arrays_concat_f (slaw s1, ...) OB_SENTINEL; \
  OB_PLASMA_API slaw slaw_##type##_arrays_concat (bslaw s1, ...) OB_SENTINEL;  \
  OB_PLASMA_API slaw slaw_##type##_array_concat_carray_f (slaw s,              \
                                                          const type *src,     \
                                                          int64 N);            \
  OB_PLASMA_API slaw slaw_##type##_array_concat_carray (bslaw s,               \
                                                        const type *src,       \
                                                        int64 N);


SLAWBEGET (int32);
SLAWBEGET (unt32);
SLAWBEGET (int64);
SLAWBEGET (unt64);
SLAWBEGET (float32);
SLAWBEGET (float64);

SLAWBEGET (int8);
SLAWBEGET (unt8);
SLAWBEGET (int16);
SLAWBEGET (unt16);



SLAWBEGET (int32c);
SLAWBEGET (unt32c);
SLAWBEGET (int64c);
SLAWBEGET (unt64c);
SLAWBEGET (float32c);
SLAWBEGET (float64c);

SLAWBEGET (int8c);
SLAWBEGET (unt8c);
SLAWBEGET (int16c);
SLAWBEGET (unt16c);



SLAWBEGET (v2int32);
SLAWBEGET (v2unt32);
SLAWBEGET (v2int64);
SLAWBEGET (v2unt64);
SLAWBEGET (v2float32);
SLAWBEGET (v2float64);

SLAWBEGET (v2int8);
SLAWBEGET (v2unt8);
SLAWBEGET (v2int16);
SLAWBEGET (v2unt16);



SLAWBEGET (v3int32);
SLAWBEGET (v3unt32);
SLAWBEGET (v3int64);
SLAWBEGET (v3unt64);
SLAWBEGET (v3float32);
SLAWBEGET (v3float64);

SLAWBEGET (v3int8);
SLAWBEGET (v3unt8);
SLAWBEGET (v3int16);
SLAWBEGET (v3unt16);



SLAWBEGET (v4int32);
SLAWBEGET (v4unt32);
SLAWBEGET (v4int64);
SLAWBEGET (v4unt64);
SLAWBEGET (v4float32);
SLAWBEGET (v4float64);

SLAWBEGET (v4int8);
SLAWBEGET (v4unt8);
SLAWBEGET (v4int16);
SLAWBEGET (v4unt16);



SLAWBEGET (v2int32c);
SLAWBEGET (v2unt32c);
SLAWBEGET (v2int64c);
SLAWBEGET (v2unt64c);
SLAWBEGET (v2float32c);
SLAWBEGET (v2float64c);

SLAWBEGET (v2int8c);
SLAWBEGET (v2unt8c);
SLAWBEGET (v2int16c);
SLAWBEGET (v2unt16c);


SLAWBEGET (v3int32c);
SLAWBEGET (v3unt32c);
SLAWBEGET (v3int64c);
SLAWBEGET (v3unt64c);
SLAWBEGET (v3float32c);
SLAWBEGET (v3float64c);

SLAWBEGET (v3int8c);
SLAWBEGET (v3unt8c);
SLAWBEGET (v3int16c);
SLAWBEGET (v3unt16c);


SLAWBEGET (v4int32c);
SLAWBEGET (v4unt32c);
SLAWBEGET (v4int64c);
SLAWBEGET (v4unt64c);
SLAWBEGET (v4float32c);
SLAWBEGET (v4float64c);

SLAWBEGET (v4int8c);
SLAWBEGET (v4unt8c);
SLAWBEGET (v4int16c);
SLAWBEGET (v4unt16c);



SLAWBEGET (m2int32);
SLAWBEGET (m2unt32);
SLAWBEGET (m2int64);
SLAWBEGET (m2unt64);
SLAWBEGET (m2float32);
SLAWBEGET (m2float64);

SLAWBEGET (m2int8);
SLAWBEGET (m2unt8);
SLAWBEGET (m2int16);
SLAWBEGET (m2unt16);



SLAWBEGET (m3int32);
SLAWBEGET (m3unt32);
SLAWBEGET (m3int64);
SLAWBEGET (m3unt64);
SLAWBEGET (m3float32);
SLAWBEGET (m3float64);

SLAWBEGET (m3int8);
SLAWBEGET (m3unt8);
SLAWBEGET (m3int16);
SLAWBEGET (m3unt16);



SLAWBEGET (m4int32);
SLAWBEGET (m4unt32);
SLAWBEGET (m4int64);
SLAWBEGET (m4unt64);
SLAWBEGET (m4float32);
SLAWBEGET (m4float64);

SLAWBEGET (m4int8);
SLAWBEGET (m4unt8);
SLAWBEGET (m4int16);
SLAWBEGET (m4unt16);



SLAWBEGET (m5int32);
SLAWBEGET (m5unt32);
SLAWBEGET (m5int64);
SLAWBEGET (m5unt64);
SLAWBEGET (m5float32);
SLAWBEGET (m5float64);

SLAWBEGET (m5int8);
SLAWBEGET (m5unt8);
SLAWBEGET (m5int16);
SLAWBEGET (m5unt16);


#undef SLAWBEGET


#endif
