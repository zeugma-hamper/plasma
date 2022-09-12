
/* (c)  oblong industries */

#ifndef OB_MATH_HOMEWORK
#define OB_MATH_HOMEWORK


#include <math.h>

#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-api.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Return value for ob_fpclassify()
typedef enum {
  OB_FP_INFINITE,
  OB_FP_NAN,
  OB_FP_NORMAL,
  OB_FP_SUBNORMAL,
  OB_FP_ZERO
} ob_fp_enum;

/// This is a combination of the C99 "fpclassify" and "signbit" functions.
/// Returns a value to indicate the class of the supplied number,
/// and sets "negative" to true if the sign bit is set.
OB_LOAM_API ob_fp_enum ob_fpclassify (float64 num, bool *negative);

/**
 * Cross-platform versions of round(), trunc(), roundf(), and truncf().
 */
#ifdef _MSC_VER
static inline float64 ob_round (float64 x)
{
  return (x < 0.0 ? ceil (x - 0.5) : floor (x + 0.5));
}

static inline float32 ob_roundf (float32 x)
{
  return (x < 0.0 ? ceil (x - 0.5) : floor (x + 0.5));
}

static inline float64 ob_trunc (float64 x)
{
  return (x < 0.0 ? ceil (x) : floor (x));
}

static inline float32 ob_truncf (float32 x)
{
  return (x < 0.0 ? ceil (x) : floor (x));
}
#else
#define ob_round(x) round (x)
#define ob_roundf(x) roundf (x)
#define ob_trunc(x) trunc (x)
#define ob_truncf(x) truncf (x)
#endif

#ifdef __cplusplus
}
#endif


#endif /* OB_MATH_HOMEWORK */
