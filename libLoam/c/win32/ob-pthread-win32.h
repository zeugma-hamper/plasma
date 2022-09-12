
/* (c)  oblong industries */

#ifndef _PTHREAD_WIN32_H
#define _PTHREAD_WIN32_H

#include <stdio.h>
#include <malloc.h>
#include <process.h>

#include "libLoam/c/ob-api.h"
#include "libLoam/c/ob-sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * These are functions intended to provide enough pthread
 * compatibility to run tests and apps that require pthread.h.
 */

//this is the way pthread's want to enter
typedef void *(*pthread_func_t) (void *args);

//we use HANDLES as pthread_t
typedef HANDLE pthread_t;

// GetCurrentThread() returns a pseudohandle, so I don't think it will
// generally behave the way you want pthread_self() to behave:
// http://msdn.microsoft.com/en-us/library/ms683182(VS.85).aspx
// #define pthread_self GetCurrentThread

typedef CRITICAL_SECTION pthread_mutex_t;
#define pthread_mutex_init(x, y) InitializeCriticalSection (x)
#define pthread_mutex_destroy(x) DeleteCriticalSection (x)
#define pthread_mutex_lock(x) EnterCriticalSection (x)
#define pthread_mutex_unlock(x) LeaveCriticalSection (x)
#define pthread_mutex_trylock(x) TryEnterCriticalSection (x)

OB_LOAM_API int pthread_create (pthread_t *out_handle, void *attr_ignored,
                                pthread_func_t thread_func, void *thread_args);

/**
 * This is similar in spirit to Linux's pthread_timedjoin_np(), but
 * the third argument has a different type.  Rather than try to
 * emulate struct timespec and clock_gettime(), we just accept a
 * dwMilliseconds argument that will be passed directly to
 * WaitForSingleObject().  But hey, _np does stand for non-portable!
 * Returns 0 on success and EBUSY if timed out.
 */
OB_LOAM_API int pthread_timedjoin_np (pthread_t thread, void **value_ptr,
                                      DWORD dwMilliseconds);

static inline int pthread_join (pthread_t thread, void **value_ptr)
{
  return pthread_timedjoin_np (thread, value_ptr, INFINITE);
}

OB_LOAM_API int pthread_detach (pthread_t thread);

/**
 * pthread conditional variables
 */
typedef struct pthread_condattr pthread_condattr_t;
enum
{
  OB_PTHREAD_SIGNAL = 0,
  OB_PTHREAD_BROADCAST = 1,
  OB_PTHREAD_MAX_EVENTS = 2
};
typedef struct
{
  HANDLE events[OB_PTHREAD_MAX_EVENTS];
  // Signal and broadcast event HANDLEs.
} pthread_cond_t;

static inline int pthread_cond_init (pthread_cond_t *cv,
                                     const pthread_condattr_t *attr)
{
  // Create an auto-reset event.
  cv->events[OB_PTHREAD_SIGNAL] = CreateEvent (NULL,   // no security
                                               FALSE,  // auto-reset event
                                               FALSE,  // non-signaled initially
                                               NULL);  // unnamed

  // Create a manual-reset event.
  cv->events[OB_PTHREAD_BROADCAST] =
    CreateEvent (NULL,   // no security
                 TRUE,   // manual-reset
                 FALSE,  // non-signaled initially
                 NULL);  // unnamed

  (void) attr;  // unused.  Leaving out parameter name yields error C2055.
  return 0;
}

static inline int pthread_cond_wait (pthread_cond_t *cv,
                                     pthread_mutex_t *external_mutex)
{
  // Release the <external_mutex> here and wait for either event
  // to become signaled, due to <pthread_cond_signal> being
  // called or <pthread_cond_broadcast> being called.
  LeaveCriticalSection (external_mutex);
  WaitForMultipleObjects (2,  // Wait on both events
                          cv->events,
                          FALSE,      // Wait for either event to be signaled
                          INFINITE);  // Wait "forever"

  // Reacquire the mutex before returning.
  EnterCriticalSection (external_mutex);
  return 0;
}

static inline int pthread_cond_signal (pthread_cond_t *cv)
{
  // Try to release one waiting thread.
  PulseEvent (cv->events[OB_PTHREAD_SIGNAL]);
  return 0;
}

static inline int pthread_cond_destroy (pthread_cond_t *cv)
{
  CloseHandle (cv->events[OB_PTHREAD_SIGNAL]);
  CloseHandle (cv->events[OB_PTHREAD_BROADCAST]);
  return 0;
}

/**
 * pthread_once is to ensure that a piece of initialization code is
 * executed at most once.
 */
typedef bool pthread_once_t;
#define PTHREAD_ONCE_INIT false
static pthread_mutex_t win32_pthread_once_lock;

#define pthread_once(once, fn)                                                 \
  do                                                                           \
    {                                                                          \
      if (!*(once))                                                            \
        {                                                                      \
          pthread_mutex_init (&win32_pthread_once_lock, NULL);                 \
          pthread_mutex_lock (&win32_pthread_once_lock);                       \
          if (!*once)                                                          \
            {                                                                  \
              *once = true;                                                    \
              fn ();                                                           \
            }                                                                  \
          pthread_mutex_unlock (&win32_pthread_once_lock);                     \
        }                                                                      \
    }                                                                          \
  while (0)

/**
 * This will emulate the pthread_key functions which handles thread-
 * specific data key visible to all threads in the process.
 */
typedef DWORD pthread_key_t;

static inline void pthread_key_create (pthread_key_t *key,
                                       void (*destructor) (void *))
{
  *(key) = TlsAlloc ();
  (void) (destructor);
}

#define pthread_getspecific(key) TlsGetValue ((key))

#define pthread_setspecific(key, value) TlsSetValue ((key), (value))

/* No fork() in windows - so ignore this */
#define pthread_atfork(F1, F2, F3) 0

#ifdef __cplusplus
}
#endif

#endif
