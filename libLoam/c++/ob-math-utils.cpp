
/* (c)  oblong industries */

#include <libLoam/c++/ob-math-utils.h>

#include <libLoam/c/ob-math.h>


bool obIsNAN (float64 val)
{
  bool negative;
  switch (ob_fpclassify (val, &negative))
    {
      case OB_FP_NAN:
        return true;
      /* fall through */
      default:
        return false;
    }
}


bool obIsNAN (float32 val)
{
  return obIsNAN ((float64) val);
}


bool obIsINF (float64 val)
{
  bool negative;
  switch (ob_fpclassify (val, &negative))
    {
      case OB_FP_INFINITE:
        return true;
      /* fall through */
      default:
        return false;
    }
}

bool obIsINF (float32 val)
{
  return obIsINF ((float64) val);
}


bool obIsPOSINF (float64 val)
{
  bool negative;
  switch (ob_fpclassify (val, &negative))
    {
      case OB_FP_INFINITE:
        if (!negative)
          return true;
      /* fall through */
      default:
        return false;
    }
}

bool obIsPOSINF (float32 val)
{
  return obIsPOSINF ((float64) val);
}


bool obIsNEGINF (float64 val)
{
  bool negative;
  switch (ob_fpclassify (val, &negative))
    {
      case OB_FP_INFINITE:
        if (negative)
          return true;
      /* fall through */
      default:
        return false;
    }
}

bool obIsNEGINF (float32 val)
{
  return obIsNEGINF ((float64) val);
}


float64 obBound (float64 input, float64 min, float64 max)
{
  if (input < min)
    return min;
  if (input > max)
    return max;
  return input;
}
