
/* (c)  oblong industries */

#ifndef _GNU_SOURCE
#define _GNU_SOURCE  // to get fpclassify() and signbit() from math.h
#endif
#include <math.h>
#include "libLoam/c/ob-math.h"
#include "libLoam/c/ob-sys.h"

#ifdef _MSC_VER
#include <float.h>  // for _fpclass stuff

ob_fp_enum ob_fpclassify (float64 num, bool *negative)
{
  int fpc = _fpclass (num);
  *negative = false;
  switch (fpc)
    {
      // all the negatives fall thru
      case _FPCLASS_NINF:
        *negative = true;
      case _FPCLASS_PINF:
        return OB_FP_INFINITE;
      case _FPCLASS_NN:
        *negative = true;
      case _FPCLASS_PN:
        return OB_FP_NORMAL;
      case _FPCLASS_ND:
        *negative = true;
      case _FPCLASS_PD:
        return OB_FP_SUBNORMAL;
      case _FPCLASS_NZ:
        *negative = true;
      case _FPCLASS_PZ:
        return OB_FP_ZERO;
      default:
        return OB_FP_NAN;
    }
}

#else

ob_fp_enum ob_fpclassify (float64 num, bool *negative)
{
  *negative = signbit (num);
  switch (fpclassify (num))
    {
      case FP_INFINITE:
        return OB_FP_INFINITE;
      case FP_NORMAL:
        return OB_FP_NORMAL;
      case FP_SUBNORMAL:
        return OB_FP_SUBNORMAL;
      case FP_ZERO:
        return OB_FP_ZERO;
      default:
        return OB_FP_NAN;
    }
}

#endif /* _MSC_VER */
