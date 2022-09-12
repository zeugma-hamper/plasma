
/* (c)  oblong industries */

// Derived from ssl/reentrant.c in the sample code:
//
// http://examples.oreilly.com/9780596002701/NSwO-1.3.tar.gz
//
// for the book:
//
// Network Security with OpenSSL by John Viega, Matt Messier, & Pravir Chandra
// Copyright 2002 O'Reilly Media, Inc.  ISBN 978-0-596-00270-1
//
// http://shop.oreilly.com/category/customer-service/faq-examples.do

#include "ossl-common.h"
#include "libLoam/c/ob-sys.h"

#if (OPENSSL_VERSION_NUMBER < 0x10100000L)

#if defined(_WIN32)
#define MUTEX_TYPE HANDLE
#define MUTEX_SETUP(x) (x) = CreateMutex (NULL, FALSE, NULL)
#define MUTEX_CLEANUP(x) CloseHandle (x)
#define MUTEX_LOCK(x) WaitForSingleObject ((x), INFINITE)
#define MUTEX_UNLOCK(x) ReleaseMutex (x)
#define THREAD_ID GetCurrentThreadId ()
#elif defined(_POSIX_THREADS)
/* _POSIX_THREADS is normally defined in unistd.h if pthreads are available
       on your platform. */
#define MUTEX_TYPE pthread_mutex_t
#define MUTEX_SETUP(x) pthread_mutex_init (&(x), NULL)
#define MUTEX_CLEANUP(x) pthread_mutex_destroy (&(x))
#define MUTEX_LOCK(x) pthread_mutex_lock (&(x))
#define MUTEX_UNLOCK(x) pthread_mutex_unlock (&(x))
#define THREAD_ID pthread_self ()
#else
#error You must define mutex operations appropriate for your platform!
#endif

/* This array will store all of the mutexes available to OpenSSL. */
static MUTEX_TYPE *mutex_buf = NULL;

static void locking_function (int mode, int n, const char *file, int line)
{
  if (mode & CRYPTO_LOCK)
    MUTEX_LOCK (mutex_buf[n]);
  else
    MUTEX_UNLOCK (mutex_buf[n]);
}

static unsigned long id_function (void)
{
  return ((unsigned long) THREAD_ID);
}

struct CRYPTO_dynlock_value
{
  MUTEX_TYPE mutex;
};

static struct CRYPTO_dynlock_value *dyn_create_function (const char *file,
                                                         int line)
{
  struct CRYPTO_dynlock_value *value;
  value = (struct CRYPTO_dynlock_value *) malloc (
    sizeof (struct CRYPTO_dynlock_value));
  if (!value)
    return NULL;
  MUTEX_SETUP (value->mutex);
  return value;
}

static void dyn_lock_function (int mode, struct CRYPTO_dynlock_value *l,
                               const char *file, int line)
{
  if (mode & CRYPTO_LOCK)
    MUTEX_LOCK (l->mutex);
  else
    MUTEX_UNLOCK (l->mutex);
}

static void dyn_destroy_function (struct CRYPTO_dynlock_value *l,
                                  const char *file, int line)
{
  MUTEX_CLEANUP (l->mutex);
  free (l);
}

int THREAD_setup (void)
{
  int i;
  mutex_buf = (MUTEX_TYPE *) malloc (CRYPTO_num_locks () * sizeof (MUTEX_TYPE));
  if (!mutex_buf)
    return 0;
  for (i = 0; i < CRYPTO_num_locks (); i++)
    MUTEX_SETUP (mutex_buf[i]);
  CRYPTO_set_id_callback (id_function);
  CRYPTO_set_locking_callback (locking_function);
  /* The following three CRYPTO_... functions are the OpenSSL functions
       for registering the callbacks we implemented above */
  CRYPTO_set_dynlock_create_callback (dyn_create_function);
  CRYPTO_set_dynlock_lock_callback (dyn_lock_function);
  CRYPTO_set_dynlock_destroy_callback (dyn_destroy_function);
  return 1;
}

int THREAD_cleanup (void)
{
  int i;
  if (!mutex_buf)
    return 0;
  CRYPTO_set_id_callback (NULL);
  CRYPTO_set_locking_callback (NULL);
  CRYPTO_set_dynlock_create_callback (NULL);
  CRYPTO_set_dynlock_lock_callback (NULL);
  CRYPTO_set_dynlock_destroy_callback (NULL);
  for (i = 0; i < CRYPTO_num_locks (); i++)
    MUTEX_CLEANUP (mutex_buf[i]);
  free (mutex_buf);
  mutex_buf = NULL;
  return 1;
}

#else

/* OpenSSL 1.1 no longer requires setting up locking */
int THREAD_setup (void)
{
  return 1;
}
int THREAD_cleanup (void)
{
  return 1;
}

#endif
