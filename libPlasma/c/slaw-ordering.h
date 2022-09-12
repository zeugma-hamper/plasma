
/* (c)  oblong industries */

#ifndef SLAW_ORDERING_INFORMATION
#define SLAW_ORDERING_INFORMATION

#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-api.h"

#include "libPlasma/c/slaw.h"

#ifdef __cplusplus
extern "C" {
#endif

/**                 Compares two slawx to determine whether they are
 *                  equal, less, or greater.  This has two advantages over
 *                  slawx_equal().  First, slaw_semantic_compare() ignores the
 *                  value of padding bytes, while slawx_equal() can think
 *                  slawx are different if the padding bytes (like for an
 *                  unt8) are different.  Second, if the slawx are unequal,
 *                  slaw_semantic_compare() determines one to be less and the
 *                  other to be greater, allowing you to do things like sort
 *                  slawx if you are so inclined.  For slawx of the same type,
 *                  the ordering is what you'd expect: lexicographical for
 *                  strings, numerical for numbers, etc.  For slawx of
 *                  different types, there is some ordering amongst the types.
 *
 * \param[in]       s1 is the slaw to compare to s2
 *
 * \param[in]       s2 is the slaw to compare to s1
 *
 * \return          -1 if s1 is less than s2
 * \return          0 if s1 is equal to s2
 * \return          1 if s1 is greater than s2
 */
OB_PLASMA_API int slaw_semantic_compare (bslaw s1, bslaw s2);

#ifdef __cplusplus
}
#endif

#endif /* SLAW_ORDERING_INFORMATION */
