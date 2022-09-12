
/* (c)  oblong industries */

#ifndef OB_THREAD_NEEDLE
#define OB_THREAD_NEEDLE

#include "libLoam/c/ob-pthread.h"
#include "libLoam/c/ob-retorts.h"
#include "libLoam/c/ob-api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * ob_static_mutex is a type which represents a mutex which does not have
 * to be explicitly initialized.  Therefore, it can be used as a static
 * or global variable, without having to worry about C++ initialization
 * order issues.
 */
typedef
#ifdef _MSC_VER
  void *
#else
  pthread_mutex_t
#endif
    ob_static_mutex;

/**
 * OB_STATIC_MUTEX_INITIALIZER is what an instance of ob_static_mutex
 * should be initialized to.  For example:
 *
 * \code
 * ob_static_mutex foobar = OB_STATIC_MUTEX_INITIALIZER;
 * \endcode
 */
#ifdef _MSC_VER
#define OB_STATIC_MUTEX_INITIALIZER NULL
#else
#define OB_STATIC_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#endif

/**
 * Locks an ob_static_mutex.
 */
OB_LOAM_API ob_retort ob_lock_static_mutex (ob_static_mutex *m);

/**
 * Unlocks an ob_static_mutex.
 */
OB_LOAM_API ob_retort ob_unlock_static_mutex (ob_static_mutex *m);

#ifdef _MSC_VER
struct ob_win_once
{
  ob_static_mutex mut;
  bool done;
};
#endif

typedef
#ifdef _MSC_VER
  struct ob_win_once
#else
  pthread_once_t
#endif
    ob_once_t;

#ifdef _MSC_VER
#define OB_ONCE_INIT                                                           \
  {                                                                            \
    OB_STATIC_MUTEX_INITIALIZER, false                                         \
  }
#else
#define OB_ONCE_INIT PTHREAD_ONCE_INIT
#endif

OB_LOAM_API ob_retort ob_once (ob_once_t *o, void (*f) (void));

#ifdef __cplusplus
}
#endif

#endif /* OB_THREAD_NEEDLE */
