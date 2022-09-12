
/* (c)  oblong industries */

#ifndef POOL_UGLY_CALLBACK_HACK
#define POOL_UGLY_CALLBACK_HACK

/* This should only be used by the tests, and may change or go away
 * in the future.  (In fact, I hope it does.)  It provides a way for
 * the tests to hook into libPlasma locking, to do some signal-related
 * nastiness and some additional assertions.
 *
 * This is not the "right" way to do this, just the most expedient:
 * https://gitlab.oblong.com/platform/docs/-/wikis/plasma-test-failures-(circa-2010)
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  POOL_UGLY_CALLBACK_PRE_ACQUIRE,
  POOL_UGLY_CALLBACK_POST_ACQUIRE,
  POOL_UGLY_CALLBACK_PRE_RELEASE,
  POOL_UGLY_CALLBACK_POST_RELEASE,
  POOL_UGLY_CALLBACK_ASSERT_OWNED
} pool_ugly_callback_when;

typedef enum {
  POOL_UGLY_CALLBACK_CONFIG_LOCK,
  POOL_UGLY_CALLBACK_DEPOSIT_LOCK,
  POOL_UGLY_CALLBACK_NOTIFICATION_LOCK
} pool_ugly_callback_what;

OB_LOAM_API extern void (*pool_ugly_callback) (pool_ugly_callback_what what,
                                               pool_ugly_callback_when when,
                                               const char *file, int line);

#define POOL_UGLY_CALLBACK(what, when)                                         \
  if (pool_ugly_callback)                                                      \
  pool_ugly_callback (what, when, __FILE__, __LINE__)

#ifdef __cplusplus
}
#endif

#endif /* POOL_UGLY_CALLBACK_HACK */
