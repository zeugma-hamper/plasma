
/* (c)  oblong industries */

#include <stdlib.h>
#include <libLoam/c/ob-retorts.h>
#include <libLoam/c++/ObRetort.h>

using namespace oblong::loam;

ob_retort return_ob_retort (void)
{
  return OB_OK;
}

ObRetort return_ObRetort (void)
{
  return OB_OK;
}

typedef ob_retort (*nesty_ob_retort_func) (void *, int64);

ob_retort nesty_ob_retort (void *f, int64 depth)
{
  ob_retort ret = depth ? ((nesty_ob_retort_func) f) (f, depth - 1) : OB_OK;

  if (ret < 0)
    {
      abort ();
      return OB_OK;  //never reached, avoids MSVC 'not all control paths return a value' warning
    }
  else
    return ret;
}

typedef ObRetort (*nesty_ObRetort_func) (void *, int64);

ObRetort nesty_ObRetort (void *f, int64 depth)
{
  ObRetort ret (depth ? ((nesty_ObRetort_func) f) (f, depth - 1) : OB_OK);

  if (ret.IsError ())
    {
      abort ();
      return OB_OK;  //never reached, avoids MSVC 'not all control paths return a value' warning
    }
  else
    return ret;
}
