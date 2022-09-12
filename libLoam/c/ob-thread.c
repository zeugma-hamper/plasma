
/* (c)  oblong industries */

#include "libLoam/c/ob-thread.h"

ob_retort ob_lock_static_mutex (ob_static_mutex *m)
{
#ifdef _MSC_VER
  CRITICAL_SECTION *crit = ob_fetch_critical (m);
  if (!crit)
    return OB_NO_MEM;
  EnterCriticalSection (crit);
#else
  int err = pthread_mutex_lock (m);
  if (err)
    return ob_errno_to_retort (err);
#endif
  return OB_OK;
}

ob_retort ob_unlock_static_mutex (ob_static_mutex *m)
{
#ifdef _MSC_VER
  CRITICAL_SECTION *crit = ob_fetch_critical (m);
  if (!crit)
    return OB_NO_MEM;
  LeaveCriticalSection (crit);
#else
  int err = pthread_mutex_unlock (m);
  if (err)
    return ob_errno_to_retort (err);
#endif
  return OB_OK;
}

ob_retort ob_once (ob_once_t *o, void (*f) (void))
{
#ifdef _MSC_VER
  ob_retort tort = ob_lock_static_mutex (&o->mut);
  if (tort < OB_OK)
    return tort;
  if (!o->done)
    {
      f ();
      o->done = true;
    }
  return ob_unlock_static_mutex (&o->mut);
#else
  int err = pthread_once (o, f);
  if (err)
    return ob_errno_to_retort (err);
  return OB_OK;
#endif
}
