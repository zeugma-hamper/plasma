
/* (c)  oblong industries */

#ifndef OB_CORETYPES_MADNESS
#define OB_CORETYPES_MADNESS

/**
 * signed 8-bit integer.  Print with "%d"
 */
typedef signed char int8;
/**
 * unsigned 8-bit integer.  Print with "%u"
 */
typedef unsigned char unt8;
/**
 * signed 16-bit integer.  Print with "%d"
 */
typedef signed short int16;
/**
 * unsigned 16-bit integer.  Print with "%u"
 */
typedef unsigned short unt16;
/**
 * signed 32-bit integer.  Print with "%d"
 */
typedef signed int int32;
/**
 * unsigned 32-bit integer.  Print with "%u"
 */
typedef unsigned int unt32;

#ifdef _MSC_VER

/**
 * signed 64-bit integer.  Print with "%" OB_FMT_64 "d"
 */
typedef __int64 int64;
/**
 * unsigned 64-bit integer.  Print with "%" OB_FMT_64 "u"
 */
typedef unsigned __int64 unt64;

/**
 * format modifier for int64 or unt64
 */
#define OB_FMT_64 "I64"
/**
 * format modifier for size_t
 */
#define OB_FMT_SIZE "I"

/**
 * Create a literal of type int64
 */
#define OB_CONST_I64(val) (val##I64)
/**
 * Create a literal of type unt64
 */
#define OB_CONST_U64(val) (val##UI64)

#elif defined(__LP64__)  // 64-bit POSIX

#ifdef __APPLE__

/**
 * signed 64-bit integer.  Print with "%" OB_FMT_64 "d"
 */
typedef long long int64;
/**
 * unsigned 64-bit integer.  Print with "%" OB_FMT_64 "u"
 */
typedef unsigned long long unt64;

/**
 * format modifier for int64 or unt64
 */
#define OB_FMT_64 "ll"
/**
 * format modifier for size_t
 */
#define OB_FMT_SIZE "z"

/**
 * Create a literal of type int64
 */
#define OB_CONST_I64(val) (val##LL)
/**
 * Create a literal of type unt64
 */
#define OB_CONST_U64(val) (val##ULL)

#else

/**
 * signed 64-bit integer.  Print with "%" OB_FMT_64 "d"
 */
typedef long int64;
/**
 * unsigned 64-bit integer.  Print with "%" OB_FMT_64 "u"
 */
typedef unsigned long unt64;

/**
 * format modifier for int64 or unt64
 */
#define OB_FMT_64 "l"
/**
 * format modifier for size_t
 */
#define OB_FMT_SIZE "z"

/**
 * Create a literal of type int64
 */
#define OB_CONST_I64(val) (val##L)
/**
 * Create a literal of type unt64
 */
#define OB_CONST_U64(val) (val##UL)

/**
 * \cond INTERNAL
 * Because libPlasma/c++ insists on peeking behind the curtain...
 */
#define OB_INT64_IS_LONG 1
/** \endcond */

#endif

#else  // 32-bit POSIX

/**
 * signed 64-bit integer.  Print with "%" OB_FMT_64 "d"
 */
typedef long long int64;
/**
 * unsigned 64-bit integer.  Print with "%" OB_FMT_64 "u"
 */
typedef unsigned long long unt64;

/**
 * format modifier for int64 or unt64
 */
#define OB_FMT_64 "ll"
/**
 * format modifier for size_t
 */
#define OB_FMT_SIZE "z"

/**
 * Create a literal of type int64
 */
#define OB_CONST_I64(val) (val##LL)
/**
 * Create a literal of type unt64
 */
#define OB_CONST_U64(val) (val##ULL)

#endif

/**
 * 32-bit floating point type.
 */
typedef float float32;
/**
 * 64-bit floating point type.
 */
typedef double float64;

#ifndef __cplusplus /* C++ has bool, true, false built in */

#include <stdbool.h>

#endif

#if defined(__GNUC__) && (__GNUC__ >= 4)
/* prefer to use the gcc builtin functions, so we don't have to worry about
 * whether _ISOC99_SOURCE was defined when math.h was included */

/**
 * Not a Number.
 */
#define OB_NAN __builtin_nan ("")  // (0.0 / 0.0)

/**
 * Positive infinity.
 */
#define OB_POSINF __builtin_inf ()  // (1.0 / 0.0)

/**
 * Negative infinity.
 */
#define OB_NEGINF -__builtin_inf ()  // (-1.0 / 0.0)

#elif defined(_MSC_VER)

/**
 * \cond INTERNAL
 */
static const unt32 OB_NAN_unt32 =
  0xFFC00000;  //(float bytes that result from 0.f / 0.f)
static const unt32 OB_POSINF_unt32 =
  0x7F800000;  //(float bytes that result from 1.f / 0.f)
static const unt32 OB_NEGINF_unt32 =
  0xFF800000;  //(float bytes that result from -1.f / 0.f)

#define OB_NAN32 (*((float32 *) &OB_NAN_unt32))
#define OB_POSINF32 (*((float32 *) &OB_POSINF_unt32))
#define OB_NEGINF32 (*((float32 *) &OB_NEGINF_unt32))

static const unt64 OB_NAN_unt64 =
  0xFFF8000000000000;  //(float bytes that result from 0.0 / 0.0)
static const unt64 OB_POSINF_unt64 =
  0x7FF0000000000000;  //(float bytes that result from 1.0 / 0.0)
static const unt64 OB_NEGINF_unt64 =
  0xFFF0000000000000;  //(float bytes that result from -1.0 / 0.0)

#define OB_NAN64 (*((float64 *) &OB_NAN_unt64))
#define OB_POSINF64 (*((float64 *) &OB_POSINF_unt64))
#define OB_NEGINF64 (*((float64 *) &OB_NEGINF_unt64))
                       /** \endcond */

/**
 * Not a Number.
 */
#define OB_NAN OB_NAN32

/**
 * Positive infinity.
 */
#define OB_POSINF OB_POSINF32

/**
 * Negative infinity.
 */
#define OB_NEGINF OB_NEGINF32

#else
// without gcc or msvc, just have to hope we picked up the C99 definitions from math.h

/**
 * Not a Number.
 */
#define OB_NAN NAN          // (0.0 / 0.0)

/**
 * Positive infinity.
 */
#define OB_POSINF INFINITY  // (1.0 / 0.0)

/**
 * Negative infinity.
 */
#define OB_NEGINF (-1.0 * INFINITY)  // (-1.0 / 0.0)
#endif


#endif /* OB_CORETYPES_MADNESS */
