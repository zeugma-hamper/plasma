
/* (c)  oblong industries */

#ifndef OB_TYPES_CRENELLATIONS
#define OB_TYPES_CRENELLATIONS


// ob-coretypes has platform-specific knowledge

#include "libLoam/c/ob-coretypes.h"


// the rest of these definitions are platform-independent

/**
 * byte is a synonym for unt8
 */
typedef unt8 byte;


/**
 * The minimum value that an unt8 can hold.
 */
#define UNT8_ZERO ((unt8) 0)

/**
 * Synonym for UNT8_ZERO
 */
#define BYTE_ZERO ((byte) 0)

/**
 * The minimum value that an unt16 can hold.
 */
#define UNT16_ZERO ((unt16) 0)

/**
 * The minimum value that an unt32 can hold.
 */
#define UNT32_ZERO ((unt32) 0)

/**
 * The minimum value that an unt64 can hold.
 */
#define UNT64_ZERO ((unt64) 0LL)


/**
 * The maximum value that an unt8 can hold.
 */
#define UNT8_ALLBITS ((unt8) 0xff)

/**
 * Synonym for UNT8_ALLBITS
 */
#define BYTE_ALLBITS ((byte) 0xff)

/**
 * The maximum value that an unt16 can hold.
 */
#define UNT16_ALLBITS ((unt16) 0xffff)

/**
 * The maximum value that an unt32 can hold.
 */
#define UNT32_ALLBITS ((unt32) 0xffffffff)

/**
 * The maximum value that an unt64 can hold.
 */
#define UNT64_ALLBITS ((unt64) 0xffffffffffffffffLL)


/**
 * The maximum value that an int8 can hold.
 */
#define OB_INT8_MAX 127

/**
 * The minimum value that an int8 can hold.
 */
#define OB_INT8_MIN -128

/**
 * The maximum value that an int16 can hold.
 */
#define OB_INT16_MAX 32767

/**
 * The minimum value that an int16 can hold.
 */
#define OB_INT16_MIN -32768

/**
 * The maximum value that an int32 can hold.
 */
#define OB_INT32_MAX (0x7FFFFFFF)

/**
 * The minimum value that an int32 can hold.
 */
#define OB_INT32_MIN (-OB_INT32_MAX - 1)

/**
 * The maximum value that an int64 can hold.
 */
#define OB_INT64_MAX OB_CONST_I64 (9223372036854775807)

/**
 * The minimum value that an int64 can hold.
 */
#define OB_INT64_MIN OB_CONST_I64 (-9223372036854775808)


// TODO: How should we Doxygenate these structs?  Very repetitive!

//
// scalars & complexities
//
// clang-format off

typedef struct { int8 re, im; } int8c;
typedef struct { unt8 re, im; } unt8c;
typedef struct { int16 re, im; } int16c;
typedef struct { unt16 re, im; } unt16c;

typedef struct { int32 re, im; } int32c;
typedef struct { unt32 re, im; } unt32c;
typedef struct { int64 re, im; } int64c;
typedef struct { unt64 re, im; } unt64c;
typedef struct { float32 re, im; } float32c;
typedef struct { float64 re, im; } float64c;


//
// two-, three-, and four element vectors (scalar elements)
//


typedef struct { int8 x, y; } v2int8;
typedef struct { unt8 x, y; } v2unt8;
typedef struct { int16 x, y; } v2int16;
typedef struct { unt16 x, y; } v2unt16;
typedef struct { int32 x, y; } v2int32;
typedef struct { unt32 x, y; } v2unt32;
typedef struct { int64 x, y; } v2int64;
typedef struct { unt64 x, y; } v2unt64;
typedef struct { float32 x, y; } v2float32;
typedef struct { float64 x, y; } v2float64;

typedef struct { int8 x, y, z; } v3int8;
typedef struct { unt8 x, y, z; } v3unt8;
typedef struct { int16 x, y, z; } v3int16;
typedef struct { unt16 x, y, z; } v3unt16;
typedef struct { int32 x, y, z; } v3int32;
typedef struct { unt32 x, y, z; } v3unt32;
typedef struct { int64 x, y, z; } v3int64;
typedef struct { unt64 x, y, z; } v3unt64;
typedef struct { float32 x, y, z; } v3float32;
typedef struct { float64 x, y, z; } v3float64;

typedef struct { int8 x, y, z, w; } v4int8;
typedef struct { unt8 x, y, z, w; } v4unt8;
typedef struct { int16 x, y, z, w; } v4int16;
typedef struct { unt16 x, y, z, w; } v4unt16;
typedef struct { int32 x, y, z, w; } v4int32;
typedef struct { unt32 x, y, z, w; } v4unt32;
typedef struct { int64 x, y, z, w; } v4int64;
typedef struct { unt64 x, y, z, w; } v4unt64;
typedef struct { float32 x, y, z, w; } v4float32;
typedef struct { float64 x, y, z, w; } v4float64;


//
// two-, three-, and four element vectors (complex elements)
//


typedef struct { int8c x, y; } v2int8c;
typedef struct { unt8c x, y; } v2unt8c;
typedef struct { int16c x, y; } v2int16c;
typedef struct { unt16c x, y; } v2unt16c;
typedef struct { int32c x, y; } v2int32c;
typedef struct { unt32c x, y; } v2unt32c;
typedef struct { int64c x, y; } v2int64c;
typedef struct { unt64c x, y; } v2unt64c;
typedef struct { float32c x, y; } v2float32c;
typedef struct { float64c x, y; } v2float64c;

typedef struct { int8c x, y, z; } v3int8c;
typedef struct { unt8c x, y, z; } v3unt8c;
typedef struct { int16c x, y, z; } v3int16c;
typedef struct { unt16c x, y, z; } v3unt16c;
typedef struct { int32c x, y, z; } v3int32c;
typedef struct { unt32c x, y, z; } v3unt32c;
typedef struct { int64c x, y, z; } v3int64c;
typedef struct { unt64c x, y, z; } v3unt64c;
typedef struct { float32c x, y, z; } v3float32c;
typedef struct { float64c x, y, z; } v3float64c;

typedef struct { int8c x, y, z, w; } v4int8c;
typedef struct { unt8c x, y, z, w; } v4unt8c;
typedef struct { int16c x, y, z, w; } v4int16c;
typedef struct { unt16c x, y, z, w; } v4unt16c;
typedef struct { int32c x, y, z, w; } v4int32c;
typedef struct { unt32c x, y, z, w; } v4unt32c;
typedef struct { int64c x, y, z, w; } v4int64c;
typedef struct { unt64c x, y, z, w; } v4unt64c;
typedef struct { float32c x, y, z, w; } v4float32c;
typedef struct { float64c x, y, z, w; } v4float64c;

// clang-format on
// multivectors (autogenerated by ob-mvtypes.pl)

#include "ob-mvtypes.h"

/**
 * for containers, a generic function pointer
 */

typedef void (*voidfunc) (void);

#endif
