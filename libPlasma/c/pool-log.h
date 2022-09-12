
/* (c)  oblong industries */

#ifndef POOL_LOG_CABIN
#define POOL_LOG_CABIN

#include "libLoam/c/ob-api.h"
#include "libLoam/c/ob-attrs.h"
#include "libPlasma/c/pool.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Calling this function causes logging to be redirected to a pool.
 * \a dest is the pool to log to (takes ownership of this hose, so you
 * shouldn't free it or do anything with it after passing it to this
 * function; it will be automatically freed on program exit),
 * \a disableTerminal is true if you want to stop the log output from
 * going to the terminal (stderr) in addition to the pool.  The rest
 * of the arguments are the log levels you want this to apply to,
 * terminated by NULL.
 * \note You can only call this function once, and it stays in effect
 * until program exit.
 * \note Log messages that occur while logging to the pool are still
 * sent to stderr.  Some messages in global destructors or atexit()
 * functions may also go to stderr.
 * \note disableTerminal should be a bool, but varargs and type promotion
 * do not mix well; we have to specify a wider type here to avoid -Wvarargs.
 */
OB_PLASMA_API OB_SENTINEL ob_retort ob_log_to_pool (pool_hose dest,
                                                    int disableTerminal,
                                                    /* ob_log_level* */...);

#ifdef __cplusplus
}
#endif

#endif /* POOL_LOG_CABIN */
