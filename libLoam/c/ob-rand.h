
/* (c)  oblong industries */

#ifndef OB_RAND_CORPORATION
#define OB_RAND_CORPORATION


#include "libLoam/c/ob-api.h"
#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-retorts.h"
#include <stddef.h>


#ifdef __cplusplus
extern "C" {
#endif


// This trick lets these C functions have default arguments when called
// from C++.  (But you must specify all arguments when calling from C.)
#ifdef __cplusplus
#define OB_DEFARG(x) = x
#else
#define OB_DEFARG(x)
#endif

// forward declaration so we don't need to include dSFMT.h
struct DSFMT_T;

/**
 * Type which holds random number generator state.
 */
typedef struct DSFMT_T ob_rand_t;

/**
 * Seeds the random-number generator using ob_truly_random().
 */
#define OB_RAND_COMPLETELY_RANDOM_PLEASE -1

/**
 * Allocate a new ob_rand_t and seed it.
 */
OB_LOAM_API ob_rand_t *ob_rand_allocate_state (
  int32 seedval OB_DEFARG (OB_RAND_COMPLETELY_RANDOM_PLEASE));

/**
 * Free an ob_rand_t that was allocated with ob_rand_allocate_state()
 */
OB_LOAM_API void ob_rand_free_state (ob_rand_t *rand_state);

/**
 * Seed explicit Mersenne Twister state
 */
OB_LOAM_API void ob_rand_state_seed_int32 (
  ob_rand_t *rand_state,
  int32 seedval OB_DEFARG (OB_RAND_COMPLETELY_RANDOM_PLEASE));

/**
 * use Mersenne Twister with explicit state.
 * \return uniformly distributed float64 x : low <= x < high
 */
OB_LOAM_API float64 ob_rand_state_float64 (float64 low, float64 high,
                                           ob_rand_t *rand_state);

/**
 * use Mersenne Twister with explicit state.
 * \return uniformly distributed int32 x : low <= x < high
 */
OB_LOAM_API int32 ob_rand_state_int32 (int32 low, int32 high,
                                       ob_rand_t *rand_state);

/**
 * Produces all unsigned 32-bit integers with equal probability.
 * Uses Mersenne Twister with explicit state.
 */
OB_LOAM_API unt32 ob_rand_state_unt32 (ob_rand_t *rand_state);

/**
 * Produces all unsigned 64-bit integers with equal probability.
 * Uses Mersenne Twister with explicit state.
 */
OB_LOAM_API unt64 ob_rand_state_unt64 (ob_rand_t *rand_state);

/**
 * Seed implicit Mersenne Twister state
 */
OB_LOAM_API void
ob_rand_seed_int32 (int32 seedval OB_DEFARG (OB_RAND_COMPLETELY_RANDOM_PLEASE));

/**
 * use Mersenne Twister with implicit state.
 * \return uniformly distributed float64 x : low <= x < high
 */
OB_LOAM_API float64 ob_rand_float64 (float64 low OB_DEFARG (0.0),
                                     float64 high OB_DEFARG (1.0));

/**
 * use Mersenne Twister with implicit state.
 * \return uniformly distributed int32 x : low <= x < high
 */
OB_LOAM_API int32 ob_rand_int32 (int32 low, int32 high);

/**
 * Produces all unsigned 32-bit integers with equal probability.
 * Uses Mersenne Twister with implicit, non-thread-safe state.
 */
OB_LOAM_API unt32 ob_rand_unt32 (void);

/**
 * Produces all unsigned 64-bit integers with equal probability.
 * Uses Mersenne Twister with implicit, non-thread-safe state.
 */
OB_LOAM_API unt64 ob_rand_unt64 (void);

/**
 * Generate a standard normal variate; the algorithm generates two
 * variates at once: the first is the return value and the second can
 * be saved by passing a non-NULL float64 pointer.  \note Uses
 * ob_global_rand_state
 */
OB_LOAM_API float64 ob_rand_normal (float64 *second OB_DEFARG (NULL));

/**
 * Generate a standard normal variate; the algorithm generates two
 * variates at once: the first is the return value and the second can
 * be saved by passing a non-NULL float64 pointer.
 */
OB_LOAM_API float64 ob_rand_normal_state (ob_rand_t *rand_state,
                                          float64 *second OB_DEFARG (NULL));


/**
 * Generate random bytes
 * \note Uses ob_global_rand_state
 */
OB_LOAM_API void ob_random_bytes (unt8 *uu, size_t n);

/**
 * Generate random bytes using explicit state.
 */
OB_LOAM_API void ob_random_bytes_state (ob_rand_t *rand_state, unt8 *uu,
                                        size_t n);

/**
 * Generates cryptographic-quality random bytes in a system-dependent way.
 * Writes \a len random bytes into \a dst.  Returns OB_OK if successful.
 * Slower than the pseudo-random functions, so this is best used for
 * generating a seed for a pseudo-random generator.  (Which is exactly
 * what OB_RAND_COMPLETELY_RANDOM_PLEASE does.)
 */
OB_LOAM_API ob_retort ob_truly_random (void *dst, size_t len);

#ifdef __cplusplus
}
#endif


#endif /* OB_RAND_CORPORATION */
