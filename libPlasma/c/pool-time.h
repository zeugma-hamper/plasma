
/* (c)  oblong industries */

#ifndef POOL_TIME_H
#define POOL_TIME_H

#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-util.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * pool_timestamp is a floating point number denoting time in seconds
 * (and fractions thereof).  A pool_timestamp is used for two
 * purposes: Recording the time of deposit of a protein in a pool (in
 * seconds since the epoch, Jan. 1, 1970), and specifying the timeout
 * for pool await functions.
 */

typedef float64 pool_timestamp;

/**
 * When seeking by time (i.e., positioning the pool's index pointing to
 * a proteining whose timestamp is close to a given timeout), when
 * have three possible strategies.
 */
typedef enum {
  OB_CLOSEST = 0,   /**< Closest value, either above or below */
  OB_CLOSEST_LOWER, /**< Closest value less or equal to the desired one */
  OB_CLOSEST_HIGHER /**< Closest value greater or equal to the desired one */
} time_comparison;


/**
 * Return a pool_timestamp for "now".
 *
 * Currently the same as ob_current_time(), but you should use
 * pool_timestamp_now if you specifically want a pool timestamp,
 * and use ob_current_time if you want fractional seconds since
 * the epoch for some other reason.  (in case the definition of
 * pool_timestamp ever changes in the future.)
 *
 * pool_timestamp pool_timestamp_now (void);
 */
#define pool_timestamp_now ob_current_time


#ifdef __cplusplus
}
#endif


#endif /* POOL_TIME_H */
