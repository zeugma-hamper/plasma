
/* (c)  oblong industries */

#ifndef OB_ATOMIC_STEAMPUNK
#define OB_ATOMIC_STEAMPUNK


#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-api.h"

#ifdef __cplusplus
extern "C" {
#endif

/* To preserve my sanity, and because they are fairly complicated on
 * many platforms, implement the 64-bit operations as non-inline
 * functions on all platforms. */

/**
 * Return *loc after executing a memory barrier.
 */
OB_LOAM_API int64 ob_atomic_int64_ref (const int64 *loc);

/**
 * Set *loc to \a shall_be, and then execute a memory barrier.
 */
OB_LOAM_API void ob_atomic_int64_set (int64 *loc, int64 shall_be);

/**
 * If *loc equals \a was, then set *loc to \a shall_be and return
 * true.  Otherwise, leave *loc unchanged and return false.
 */
OB_LOAM_API bool ob_atomic_int64_compare_and_swap (int64 *loc, int64 was,
                                                   int64 shall_be);

/**
 * Atomically increment *loc by \a addend, and return the new value.
 */
OB_LOAM_API int64 ob_atomic_int64_add (int64 *loc, int64 addend);


/* For the 32-bit and pointer operations, on the other hand,
 * we have different inline implementations for different platforms... */

#if !defined(_MSC_VER)

/**
 * \cond INTERNAL
 */
#define OB_SIGNAL_SAFE_ATOMIC_OPS 1
/** \endcond */

/**
 * Execute a memory barrier.
 */
#define OB_MEMORY_BARRIER() __sync_synchronize ()

/**
 * Return *loc after executing a memory barrier.
 */
static inline int32 ob_atomic_int32_ref (const int32 *loc)
{
  __sync_synchronize ();
  return *loc;
}

/**
 * Set *loc to \a shall_be, and then execute a memory barrier.
 */
static inline void ob_atomic_int32_set (int32 *loc, int32 shall_be)
{
  *loc = shall_be;
  __sync_synchronize ();
}

/**
 * If *loc equals \a was, then set *loc to \a shall_be and return
 * true.  Otherwise, leave *loc unchanged and return false.
 */
static inline bool ob_atomic_int32_compare_and_swap (int32 *loc, int32 was,
                                                     int32 shall_be)
{
  return __sync_bool_compare_and_swap (loc, was, shall_be);
}

/**
 * Atomically increment *loc by \a addend, and return the new value.
 */
static inline int32 ob_atomic_int32_add (int32 *loc, int32 addend)
{
  return __sync_add_and_fetch (loc, addend);
}


/**
 * Return \*loc after executing a memory barrier.
 * void* ob_atomic_pointer_ref (void **loc)
 */
#define ob_atomic_pointer_ref(loc) (__sync_synchronize (), *(loc))

/**
 * Set *loc to \a shall_be, and then execute a memory barrier.
 * void ob_atomic_pointer_set (void **loc, void *shall_be)
 */
#define ob_atomic_pointer_set(loc, shall_be)                                   \
  do                                                                           \
    {                                                                          \
      *(loc) = (shall_be);                                                     \
      __sync_synchronize ();                                                   \
    }                                                                          \
  while (0)

/**
 * If *loc equals \a was, then set *loc to \a shall_be and return
 * true.  Otherwise, leave *loc unchanged and return false.
 * bool ob_atomic_pointer_compare_and_swap (void **loc, void *was, void *shall_be)
 */
#define ob_atomic_pointer_compare_and_swap __sync_bool_compare_and_swap


#else /* _MSC_VER */

/**
 * \cond INTERNAL
 * Well, I don't know if we should say this or not, since there aren't
 * signals on Windows...
 */
#define OB_SIGNAL_SAFE_ATOMIC_OPS 1
/** \endcond */

// this should be enough to get the windows system functions
// needed below and also takes care of undefining some silly
// macros that come with <windows.h> such as max and min
#include <libLoam/c/ob-sys.h>

/**
 * Execute a memory barrier.
 */
#define OB_MEMORY_BARRIER() MemoryBarrier ()

/**
 * Return *loc after executing a memory barrier.
 */
static inline int32 ob_atomic_int32_ref (const int32 *loc)
{
  MemoryBarrier ();
  return *loc;
}

/**
 * Set *loc to \a shall_be, and then execute a memory barrier.
 */
static inline void ob_atomic_int32_set (int32 *loc, int32 shall_be)
{
  *loc = shall_be;
  MemoryBarrier ();
}

/**
 * If *loc equals \a was, then set *loc to \a shall_be and return
 * true.  Otherwise, leave *loc unchanged and return false.
 */
static inline bool ob_atomic_int32_compare_and_swap (int32 *loc, int32 was,
                                                     int32 shall_be)
{
  return (was
          == InterlockedCompareExchange ((volatile LONG *) loc, shall_be, was));
}

/**
 * Atomically increment *loc by \a addend, and return the new value.
 */
static inline int32 ob_atomic_int32_add (int32 *loc, int32 addend)
{
  return addend + InterlockedExchangeAdd ((volatile LONG *) loc, addend);
}


/**
 * Return *loc after executing a memory barrier.
 */
#define ob_atomic_pointer_ref(loc) (MemoryBarrier (), *(loc))

/**
 * Set *loc to \a shall_be, and then execute a memory barrier.
 */
#define ob_atomic_pointer_set(loc, shall_be)                                   \
  do                                                                           \
    {                                                                          \
      *(loc) = (shall_be);                                                     \
      MemoryBarrier ();                                                        \
    }                                                                          \
  while (0)

/**
 * If *loc equals \a was, then set *loc to \a shall_be and return
 * true.  Otherwise, leave *loc unchanged and return false.
 */
#define ob_atomic_pointer_compare_and_swap(loc, was, shall_be)                 \
  ob_private_atomic_pointer_compare_and_swap ((void **) (loc), (was),          \
                                              (shall_be))
static inline bool ob_private_atomic_pointer_compare_and_swap (void **loc,
                                                               void *was,
                                                               void *shall_be)
{
  return (was == InterlockedCompareExchangePointer (loc, shall_be, was));
}

#endif /* _MSC_VER */


#ifdef __cplusplus
}
#endif


#endif /* OB_ATOMIC_STEAMPUNK */
