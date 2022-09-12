
/* (c)  oblong industries */

#include "libLoam/c/ob-log.h"
#include "libPlasma/c/pool.h"
#include "libPlasma/c/private/pool_impl.h"
#include "libPlasma/c/private/pool-ugly-callback.h"

ob_retort pool_create_semaphores (pool_hose ph)
{
  return ph->lock_ops->plop_create (ph);
}


ob_retort pool_destroy_semaphores (pool_hose ph, bool it_should_exist)
{
  return ph->lock_ops->plop_destroy (ph, it_should_exist);
}


ob_retort pool_open_semaphores (pool_hose ph)
{
  return ph->lock_ops->plop_open (ph);
}


ob_retort pool_close_semaphores (pool_hose ph)
{
  return ph->lock_ops->plop_close (ph);
}


ob_retort pool_deposit_lock (pool_hose ph)
{
  POOL_UGLY_CALLBACK (POOL_UGLY_CALLBACK_DEPOSIT_LOCK,
                      POOL_UGLY_CALLBACK_PRE_ACQUIRE);
  ob_retort err = ph->lock_ops->plop_lock (ph, POOL_SEM_DEPOSIT_LOCK_IDX);
  POOL_UGLY_CALLBACK (POOL_UGLY_CALLBACK_DEPOSIT_LOCK,
                      POOL_UGLY_CALLBACK_POST_ACQUIRE);
  return err;
}


ob_retort pool_deposit_unlock (pool_hose ph)
{
  POOL_UGLY_CALLBACK (POOL_UGLY_CALLBACK_DEPOSIT_LOCK,
                      POOL_UGLY_CALLBACK_PRE_RELEASE);
  ob_retort err = ph->lock_ops->plop_unlock (ph, POOL_SEM_DEPOSIT_LOCK_IDX);
  POOL_UGLY_CALLBACK (POOL_UGLY_CALLBACK_DEPOSIT_LOCK,
                      POOL_UGLY_CALLBACK_POST_RELEASE);
  return err;
}


ob_retort pool_notification_lock (pool_hose ph)
{
  POOL_UGLY_CALLBACK (POOL_UGLY_CALLBACK_NOTIFICATION_LOCK,
                      POOL_UGLY_CALLBACK_PRE_ACQUIRE);
  ob_retort err = ph->lock_ops->plop_lock (ph, POOL_SEM_NOTIFICATION_LOCK_IDX);
  POOL_UGLY_CALLBACK (POOL_UGLY_CALLBACK_NOTIFICATION_LOCK,
                      POOL_UGLY_CALLBACK_POST_ACQUIRE);
  return err;
}


ob_retort pool_notification_unlock (pool_hose ph)
{
  POOL_UGLY_CALLBACK (POOL_UGLY_CALLBACK_NOTIFICATION_LOCK,
                      POOL_UGLY_CALLBACK_PRE_RELEASE);
  ob_retort err =
    ph->lock_ops->plop_unlock (ph, POOL_SEM_NOTIFICATION_LOCK_IDX);
  POOL_UGLY_CALLBACK (POOL_UGLY_CALLBACK_NOTIFICATION_LOCK,
                      POOL_UGLY_CALLBACK_POST_RELEASE);
  return err;
}

static ob_retort dummy_create (pool_hose ph)
{
  OB_FATAL_BUG_CODE (0x20112000, "Attempted to call undefined create method");
}

static ob_retort dummy_destroy (pool_hose ph, bool ignore)
{
  return OB_OK;
}

static ob_retort dummy_open (pool_hose ph)
{
  OB_FATAL_BUG_CODE (0x20112001, "Attempted to call undefined open method");
}

static ob_retort dummy_close (pool_hose ph)
{
  return OB_OK;
}

static ob_retort dummy_lock (pool_hose ph, int idx)
{
  OB_FATAL_BUG_CODE (0x20112002,
                     "Attempted to call undefined lock method (index %d)", idx);
}

static ob_retort dummy_unlock (pool_hose ph, int idx)
{
  return OB_OK;
}

const pool_lock_ops pool_null_ops = {dummy_create, dummy_destroy, dummy_open,
                                     dummy_close,  dummy_lock,    dummy_unlock};
