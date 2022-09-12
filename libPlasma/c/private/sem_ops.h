
/* (c)  oblong industries */

/* Somewhat inappropriately named now, this file is more like
 * "pool locking ops".  The functions declared here are now merely
 * wrappers which call into the vtable (pool_lock_ops) of the hose.
 *
 * There are two implementations of locking: semaphores (pool_sem_ops,
 * which used to be the only way) and flock (pool_flock_ops).  See
 * bug 3770 for all the gory details of how we got here and why.
 */

#ifndef SEM_OPS_RESEARCH
#define SEM_OPS_RESEARCH

#include "libPlasma/c/pool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Semaphore operations for use by pool implementations.  Only used
 * for locally hosted pools.
 */

typedef struct
{
  ob_retort (*plop_create) (pool_hose ph);
  ob_retort (*plop_destroy) (pool_hose ph, bool it_should_exist);
  ob_retort (*plop_open) (pool_hose ph);
  ob_retort (*plop_close) (pool_hose ph);
  ob_retort (*plop_lock) (pool_hose ph, int idx);
  ob_retort (*plop_unlock) (pool_hose ph, int idx);
} pool_lock_ops;

/**
 * Implementation of pool_lock_ops using semaphores.
 */
extern OB_HIDDEN const pool_lock_ops pool_sem_ops;

/**
 * Implementation of pool_lock_ops using flock().
 */
extern OB_HIDDEN const pool_lock_ops pool_flock_ops;

/**
 * Non-implementation of pool_lock_ops.
 */
extern OB_HIDDEN const pool_lock_ops pool_null_ops;

/**
 * Create and initialize per-pool semaphores.
 */

ob_retort pool_create_semaphores (pool_hose ph) OB_HIDDEN;

/**
 * Destroy per-pool semaphores.
 */

ob_retort pool_destroy_semaphores (pool_hose ph,
                                   bool it_should_exist) OB_HIDDEN;

/**
 * Open the semaphores.  Must precede any other semaphore operation,
 * other than create or destroy.
 */

ob_retort pool_open_semaphores (pool_hose ph) OB_HIDDEN;

/**
 * Close the lock files.  For sempahores, this is a no-op, but
 * flock() needs it.
 */

ob_retort pool_close_semaphores (pool_hose ph) OB_HIDDEN;

/**
 * Take the deposit lock for this pool.  While this lock is held, no
 * other thread can deposit into this pool.
 */

ob_retort pool_deposit_lock (pool_hose ph) OB_HIDDEN;

/**
 * Drop the deposit lock.
 */

ob_retort pool_deposit_unlock (pool_hose ph) OB_HIDDEN;

/**
 * Take the notification lock.  While this lock is held, no one can
 * notify a pool user of a deposit, and no pool user can set up its
 * notification state.  It is separate from the deposit lock to
 * increase the decoupling of reader and writer performance.
 */

ob_retort pool_notification_lock (pool_hose ph) OB_HIDDEN;

/**
 * Drop the notification lock.
 */

ob_retort pool_notification_unlock (pool_hose ph) OB_HIDDEN;

#ifdef __cplusplus
}
#endif

#endif /* SEM_OPS_RESEARCH */
