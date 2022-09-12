
/* (c)  oblong industries */

/* So, here's the thing:
 * libPlasma/c/fifo_ops.c - only used on non-Windows
 * libPlasma/c/win32/fifo_ops_win32.c - only used on Windows
 * libPlasma/c/pool-fifo.c - used on both!
 * libPlasma/c/private/fifo_ops.h - prototypes for all of the above .c files
 */

#ifndef FIFO_OPS_BLACK
#define FIFO_OPS_BLACK

#include "libPlasma/c/pool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Fifo-based pool operations
 */

/**
 * Final clean up on hose-specific fifo.  It can be called before
 * notification locks are setup, so be careful.
 * Doesn't return a value because if it fails there's
 * really nothing to do.
 */
void pool_fifo_multi_destroy_awaiter (pool_hose ph);

/**
 * Add an awaiter for a multi-pool await.  Returns OB_OK if a
 * protein was found during the set up.  Returns POOL_NO_SUCH_PROTEIN
 * if the set up succeeded but no protein was found.
 * Otherwise, returns an error.
 */

ob_retort pool_fifo_multi_add_awaiter (pool_hose ph, protein *ret_prot,
                                       pool_timestamp *ret_ts,
                                       int64 *ret_index);

/**
 * Tear down await state for multi-pool await.
 */

void pool_fifo_multi_remove_awaiter (pool_hose ph);

/**
 * Wake anyone waiting for a deposit in this pool.  Called by
 * depositors.
 */

ob_retort pool_fifo_wake_awaiters (pool_hose ph) OB_HIDDEN;

/**
 * Add an awaiter for a single-pool await.  Returns OB_OK if a
 * protein was found during the set up.  Returns POOL_NO_SUCH_PROTEIN
 * if the set up succeeded but no protein was found.
 * Otherwise, returns an error.
 */

ob_retort pool_fifo_await_next_single (pool_hose ph, pool_timestamp timeout,
                                       protein *ret_prot,
                                       pool_timestamp *ret_ts,
                                       int64 *ret_index);

/**
 * Generate a probably unique fifo name and cache it in the pool hose.
 * If later on we discover that we're colliding with someone else's
 * fifo name, we'll call this function again to get a new fifo name.
 */

OB_HIDDEN OB_WARN_UNUSED_RESULT ob_retort pool_create_fifo_path (pool_hose ph);

/**
 * Just the random-filename code extracted from above function.
 * \a out_buf should be at least 13 chars, and will be given a 12-char
 * name plus NUL terminator.
 */
OB_HIDDEN OB_WARN_UNUSED_RESULT ob_retort pool_random_name (char *out_buf);



#ifdef _MSC_VER

/**
 * Convert a pool timestamp into the corresponding DWORD that will
 * be passed to WaitForSingleObject / WaitForMultipleObjects
 */
DWORD pool_timeout_to_wait_millis (pool_timestamp timeout);

#else

/**
 * Convert a pool timestamp into the corresponding timeval so that it
 * can be passed to select().  The odd structure in which we pass in
 * the address of the struct timeval and then return an address is
 * because we have to pass a NULL address to select() to signal
 * "block forever".
 */
struct timeval *pool_timeout_to_timeval (pool_timestamp timeout,
                                         struct timeval *ret_tv) OB_HIDDEN;
#endif

/**
 * Maximum number of bytes to read when sucking all the pending
 * requests out of a wakeup fifo.
 */
#define PRETTY_DARN_LARGE 1024

#ifdef __cplusplus
}
#endif

#endif /* FIFO_OPS_BLACK */
