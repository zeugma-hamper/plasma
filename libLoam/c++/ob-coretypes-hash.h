
/* (c)  oblong industries */

#ifndef SCRAPPLE_BUILDS_STRONGER_HUMANS
#define SCRAPPLE_BUILDS_STRONGER_HUMANS


#include <cstddef>

#include <libLoam/c/ob-coretypes.h>
#include <libLoam/c/ob-types.h>


#define OB_HASH_PROTO(type) std::size_t hash_value (type const &item)


// core complex types
OB_HASH_PROTO (int8c);
OB_HASH_PROTO (unt8c);
OB_HASH_PROTO (int16c);
OB_HASH_PROTO (unt16c);
OB_HASH_PROTO (int32c);
OB_HASH_PROTO (unt32c);
OB_HASH_PROTO (int64c);
OB_HASH_PROTO (unt64c);
OB_HASH_PROTO (float32c);
OB_HASH_PROTO (float64c);

// 2-vects
OB_HASH_PROTO (v2int8);
OB_HASH_PROTO (v2unt8);
OB_HASH_PROTO (v2int16);
OB_HASH_PROTO (v2unt16);
OB_HASH_PROTO (v2int32);
OB_HASH_PROTO (v2unt32);
OB_HASH_PROTO (v2int64);
OB_HASH_PROTO (v2unt64);
OB_HASH_PROTO (v2float32);
OB_HASH_PROTO (v2float64);

// 3-vects
OB_HASH_PROTO (v3int8);
OB_HASH_PROTO (v3unt8);
OB_HASH_PROTO (v3int16);
OB_HASH_PROTO (v3unt16);
OB_HASH_PROTO (v3int32);
OB_HASH_PROTO (v3unt32);
OB_HASH_PROTO (v3int64);
OB_HASH_PROTO (v3unt64);
OB_HASH_PROTO (v3float32);
OB_HASH_PROTO (v3float64);

// 4-vects
OB_HASH_PROTO (v4int8);
OB_HASH_PROTO (v4unt8);
OB_HASH_PROTO (v4int16);
OB_HASH_PROTO (v4unt16);
OB_HASH_PROTO (v4int32);
OB_HASH_PROTO (v4unt32);
OB_HASH_PROTO (v4int64);
OB_HASH_PROTO (v4unt64);
OB_HASH_PROTO (v4float32);
OB_HASH_PROTO (v4float64);

// complex 2-vects
OB_HASH_PROTO (v2int8c);
OB_HASH_PROTO (v2unt8c);
OB_HASH_PROTO (v2int16c);
OB_HASH_PROTO (v2unt16c);
OB_HASH_PROTO (v2int32c);
OB_HASH_PROTO (v2unt32c);
OB_HASH_PROTO (v2int64c);
OB_HASH_PROTO (v2unt64c);
OB_HASH_PROTO (v2float32c);
OB_HASH_PROTO (v2float64c);

// complex 3-vects
OB_HASH_PROTO (v3int8c);
OB_HASH_PROTO (v3unt8c);
OB_HASH_PROTO (v3int16c);
OB_HASH_PROTO (v3unt16c);
OB_HASH_PROTO (v3int32c);
OB_HASH_PROTO (v3unt32c);
OB_HASH_PROTO (v3int64c);
OB_HASH_PROTO (v3unt64c);
OB_HASH_PROTO (v3float32c);
OB_HASH_PROTO (v3float64c);

// complex 4-vects
OB_HASH_PROTO (v4int8c);
OB_HASH_PROTO (v4unt8c);
OB_HASH_PROTO (v4int16c);
OB_HASH_PROTO (v4unt16c);
OB_HASH_PROTO (v4int32c);
OB_HASH_PROTO (v4unt32c);
OB_HASH_PROTO (v4int64c);
OB_HASH_PROTO (v4unt64c);
OB_HASH_PROTO (v4float32c);
OB_HASH_PROTO (v4float64c);

// int8 multivectors
OB_HASH_PROTO (m2int8);
OB_HASH_PROTO (m3int8);
OB_HASH_PROTO (m4int8);
OB_HASH_PROTO (m5int8);

// unt8 multivectors
OB_HASH_PROTO (m2unt8);
OB_HASH_PROTO (m3unt8);
OB_HASH_PROTO (m4unt8);
OB_HASH_PROTO (m5unt8);

// int16 multivectors
OB_HASH_PROTO (m2int16);
OB_HASH_PROTO (m3int16);
OB_HASH_PROTO (m4int16);
OB_HASH_PROTO (m5int16);

// unt16 multivectors
OB_HASH_PROTO (m2unt16);
OB_HASH_PROTO (m3unt16);
OB_HASH_PROTO (m4unt16);
OB_HASH_PROTO (m5unt16);

// int32 multivectors
OB_HASH_PROTO (m2int32);
OB_HASH_PROTO (m3int32);
OB_HASH_PROTO (m4int32);
OB_HASH_PROTO (m5int32);

// unt32 multivectors
OB_HASH_PROTO (m2unt32);
OB_HASH_PROTO (m3unt32);
OB_HASH_PROTO (m4unt32);
OB_HASH_PROTO (m5unt32);

// int64 multivectors
OB_HASH_PROTO (m2int64);
OB_HASH_PROTO (m3int64);
OB_HASH_PROTO (m4int64);
OB_HASH_PROTO (m5int64);

// unt64 multivectors
OB_HASH_PROTO (m2unt64);
OB_HASH_PROTO (m3unt64);
OB_HASH_PROTO (m4unt64);
OB_HASH_PROTO (m5unt64);

// float32 multivectors
OB_HASH_PROTO (m2float32);
OB_HASH_PROTO (m3float32);
OB_HASH_PROTO (m4float32);
OB_HASH_PROTO (m5float32);

// float64 multivectors
OB_HASH_PROTO (m2float64);
OB_HASH_PROTO (m3float64);
OB_HASH_PROTO (m4float64);
OB_HASH_PROTO (m5float64);

#undef OB_HASH_PROTO

#endif
