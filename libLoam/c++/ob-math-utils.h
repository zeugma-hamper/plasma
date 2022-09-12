
/* (c)  oblong industries */

#ifndef OB_MATH_UTIL_TINYHELPERS
#define OB_MATH_UTIL_TINYHELPERS


#include <libLoam/c/ob-api.h>
#include <libLoam/c/ob-types.h>


// basic out-of-bounds IEEE format testing
OB_LOAMXX_API bool obIsNAN (float32 val);
OB_LOAMXX_API bool obIsNAN (float64 val);

OB_LOAMXX_API bool obIsINF (float32 val);
OB_LOAMXX_API bool obIsINF (float64 val);

OB_LOAMXX_API bool obIsPOSINF (float32 val);
OB_LOAMXX_API bool obIsPOSINF (float64 val);

OB_LOAMXX_API bool obIsNEGINF (float32 val);
OB_LOAMXX_API bool obIsNEGINF (float64 val);

OB_LOAMXX_API float64 obBound (float64 input, float64 min, float64 max);


inline int32 obMax (const int32 a, const int32 b)
{
  return (a > b ? a : b);
}
inline int32 obMin (const int32 a, const int32 b)
{
  return (a < b ? a : b);
}

inline unt32 obMax (const unt32 a, const unt32 b)
{
  return (a > b ? a : b);
}
inline unt32 obMin (const unt32 a, const unt32 b)
{
  return (a < b ? a : b);
}

inline int64 obMax (const int64 a, const int64 b)
{
  return (a > b ? a : b);
}
inline int64 obMin (const int64 a, const int64 b)
{
  return (a < b ? a : b);
}

inline unt64 obMax (const unt64 a, const unt64 b)
{
  return (a > b ? a : b);
}
inline unt64 obMin (const unt64 a, const unt64 b)
{
  return (a < b ? a : b);
}


inline float32 obMax (const float32 a, const float32 b)
{
  return (a > b ? a : b);
}
inline float32 obMin (const float32 a, const float32 b)
{
  return (a < b ? a : b);
}

inline float64 obMax (const float64 a, const float64 b)
{
  return (a > b ? a : b);
}
inline float64 obMin (const float64 a, const float64 b)
{
  return (a < b ? a : b);
}


#endif
