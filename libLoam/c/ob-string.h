
/* (c)  oblong industries */

#ifndef OB_STRING_THEORY
#define OB_STRING_THEORY

#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-api.h"

#include <stddef.h>  // need size_t

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Copies str into buf, ensuring the total length (with terminating
 * NUL) does not exceed capacity.  Returns the new length (not
 * including terminating NUL).  Implemented without the standard
 * library, so guaranteed safe to use in a signal handler.  Similar to
 * OS X's strlcpy or Windows' strcpy_s (or snprintf with a "%s" format
 * string), but different than the portable-but-inconvenient strncpy.
 */
OB_LOAM_API size_t ob_safe_copy_string (char *buf, size_t capacity,
                                        const char *str);

/**
 * Appends str to buf, ensuring the total length (with terminating
 * NUL) does not exceed capacity.  Returns the new length (not
 * including terminating NUL).  Implemented without the standard
 * library, so guaranteed safe to use in a signal handler.  Similar to
 * OS X's strlcat or Windows' strcat_s, but different than the
 * portable-but-inconvenient strncat.
 */
OB_LOAM_API size_t ob_safe_append_string (char *buf, size_t capacity,
                                          const char *str);

/**
 * Appends a base 10 representation of n to buf, ensuring the total
 * length (with terminating NUL) does not exceed capacity.  Returns
 * the new length (not including terminating NUL).  Implemented
 * without the standard library, so guaranteed safe to use in a signal
 * handler.
 */
OB_LOAM_API size_t ob_safe_append_int64 (char *buf, size_t capacity, int64 n);

/**
 * Appends a string representation of \a f to \a buf, ensuring the
 * total length (with terminating NUL) does not exceed \a capacity.
 * There will be exactly \a digits_after_decimal digits after the
 * decimal point.  Returns the new length (not including terminating
 * NUL).  Implemented without the standard library, so guaranteed safe
 * to use in a signal handler.
 */
OB_LOAM_API size_t ob_safe_append_float64 (char *buf, size_t capacity,
                                           float64 f, int digits_after_decimal);

/**
 * If the last character of \a s is a newline, replaces the newline
 * with a NUL terminator.  (Or, if the last two characters are CRLF,
 * removes both of them.)
 */
OB_LOAM_API void ob_chomp (char *s);

/**
 * Returns true if the string \a str matches the glob pattern \a p.
 * The current implementation does not understand UTF-8.
 *
 * \code
 *
 * glob patterns:
 *      *       matches zero or more characters
 *      ?       matches any single character
 *      [set]   matches any character in the set
 *      [^set]  matches any character NOT in the set
 *              where a set is a group of characters or ranges. a range
 *              is written as two characters seperated with a hyphen: a-z
 *              denotes all characters between a to z inclusive.
 *      [-set]  set matches a literal hypen and any character in the set
 *      []set]  matches a literal close bracket and any character in the set
 *
 *      char    matches itself except where char is '*' or '?' or '['
 *      \char   matches char, including any pattern character
 *
 * examples:
 *      a*c             ac abc abbc ...
 *      a?c             acc abc aXc ...
 *      a[a-z]c         aac abc acc ...
 *      a[-a-z]c        a-c aac abc ...
 *
 * \endcode
 */
OB_LOAM_API bool ob_match_glob (const char *str, const char *p);

#ifdef __cplusplus
}
#endif

#endif /* OB_STRING_THEORY */
