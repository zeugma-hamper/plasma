
/* (c)  oblong industries */

#include "ob-coretypes-hash.h"

#include <libLoam/c/ob-hash.h>


#define OB_HASH_VALUE(type)                                                    \
  std::size_t hash_value (type const &item)                                    \
  {                                                                            \
    return (std::size_t) ob_city_hash64 (&item, sizeof (item));                \
  }


// core complex types
OB_HASH_VALUE (int8c);
OB_HASH_VALUE (unt8c);
OB_HASH_VALUE (int16c);
OB_HASH_VALUE (unt16c);
OB_HASH_VALUE (int32c);
OB_HASH_VALUE (unt32c);
OB_HASH_VALUE (int64c);
OB_HASH_VALUE (unt64c);
OB_HASH_VALUE (float32c);
OB_HASH_VALUE (float64c);

// 2-vects
OB_HASH_VALUE (v2int8);
OB_HASH_VALUE (v2unt8);
OB_HASH_VALUE (v2int16);
OB_HASH_VALUE (v2unt16);
OB_HASH_VALUE (v2int32);
OB_HASH_VALUE (v2unt32);
OB_HASH_VALUE (v2int64);
OB_HASH_VALUE (v2unt64);
OB_HASH_VALUE (v2float32);
OB_HASH_VALUE (v2float64);

// 3-vects
OB_HASH_VALUE (v3int8);
OB_HASH_VALUE (v3unt8);
OB_HASH_VALUE (v3int16);
OB_HASH_VALUE (v3unt16);
OB_HASH_VALUE (v3int32);
OB_HASH_VALUE (v3unt32);
OB_HASH_VALUE (v3int64);
OB_HASH_VALUE (v3unt64);
OB_HASH_VALUE (v3float32);
OB_HASH_VALUE (v3float64);

// 4-vects
OB_HASH_VALUE (v4int8);
OB_HASH_VALUE (v4unt8);
OB_HASH_VALUE (v4int16);
OB_HASH_VALUE (v4unt16);
OB_HASH_VALUE (v4int32);
OB_HASH_VALUE (v4unt32);
OB_HASH_VALUE (v4int64);
OB_HASH_VALUE (v4unt64);
OB_HASH_VALUE (v4float32);
OB_HASH_VALUE (v4float64);

// complex 2-vects
OB_HASH_VALUE (v2int8c);
OB_HASH_VALUE (v2unt8c);
OB_HASH_VALUE (v2int16c);
OB_HASH_VALUE (v2unt16c);
OB_HASH_VALUE (v2int32c);
OB_HASH_VALUE (v2unt32c);
OB_HASH_VALUE (v2int64c);
OB_HASH_VALUE (v2unt64c);
OB_HASH_VALUE (v2float32c);
OB_HASH_VALUE (v2float64c);

// complex 3-vects
OB_HASH_VALUE (v3int8c);
OB_HASH_VALUE (v3unt8c);
OB_HASH_VALUE (v3int16c);
OB_HASH_VALUE (v3unt16c);
OB_HASH_VALUE (v3int32c);
OB_HASH_VALUE (v3unt32c);
OB_HASH_VALUE (v3int64c);
OB_HASH_VALUE (v3unt64c);
OB_HASH_VALUE (v3float32c);
OB_HASH_VALUE (v3float64c);

// complex 4-vects
OB_HASH_VALUE (v4int8c);
OB_HASH_VALUE (v4unt8c);
OB_HASH_VALUE (v4int16c);
OB_HASH_VALUE (v4unt16c);
OB_HASH_VALUE (v4int32c);
OB_HASH_VALUE (v4unt32c);
OB_HASH_VALUE (v4int64c);
OB_HASH_VALUE (v4unt64c);
OB_HASH_VALUE (v4float32c);
OB_HASH_VALUE (v4float64c);

// int8 multivectors
OB_HASH_VALUE (m2int8);
OB_HASH_VALUE (m3int8);
OB_HASH_VALUE (m4int8);
OB_HASH_VALUE (m5int8);

// unt8 multivectors
OB_HASH_VALUE (m2unt8);
OB_HASH_VALUE (m3unt8);
OB_HASH_VALUE (m4unt8);
OB_HASH_VALUE (m5unt8);

// int16 multivectors
OB_HASH_VALUE (m2int16);
OB_HASH_VALUE (m3int16);
OB_HASH_VALUE (m4int16);
OB_HASH_VALUE (m5int16);

// unt16 multivectors
OB_HASH_VALUE (m2unt16);
OB_HASH_VALUE (m3unt16);
OB_HASH_VALUE (m4unt16);
OB_HASH_VALUE (m5unt16);

// int32 multivectors
OB_HASH_VALUE (m2int32);
OB_HASH_VALUE (m3int32);
OB_HASH_VALUE (m4int32);
OB_HASH_VALUE (m5int32);

// unt32 multivectors
OB_HASH_VALUE (m2unt32);
OB_HASH_VALUE (m3unt32);
OB_HASH_VALUE (m4unt32);
OB_HASH_VALUE (m5unt32);

// int64 multivectors
OB_HASH_VALUE (m2int64);
OB_HASH_VALUE (m3int64);
OB_HASH_VALUE (m4int64);
OB_HASH_VALUE (m5int64);

// unt64 multivectors
OB_HASH_VALUE (m2unt64);
OB_HASH_VALUE (m3unt64);
OB_HASH_VALUE (m4unt64);
OB_HASH_VALUE (m5unt64);

// float32 multivectors
OB_HASH_VALUE (m2float32);
OB_HASH_VALUE (m3float32);
OB_HASH_VALUE (m4float32);
OB_HASH_VALUE (m5float32);

// float64 multivectors
OB_HASH_VALUE (m2float64);
OB_HASH_VALUE (m3float64);
OB_HASH_VALUE (m4float64);
OB_HASH_VALUE (m5float64);

#undef OB_HASH_VALUE
