
/* (c)  oblong industries */

#ifndef PLASMA_UTIL_KILT
#define PLASMA_UTIL_KILT

#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-attrs.h"
#include "libLoam/c/ob-retorts.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * These two functions compare floats by looking
 * directly at the bits in the float (hence they
 * take their arguments as unsigned integers).
 * This is because C's floating point comparison
 * operators don't result in a reliable ordering
 * (for example, NaNs are neither less than, equal
 * to, nor greater than anything else).  By looking
 * at the bits we can do a comparison that does
 * what you'd expect for actual numbers, but also
 * throws NaNs into the ordering as well.
 */
int private_float64_compare (unt64 f1, unt64 f2) OB_HIDDEN;
int private_float32_compare (unt32 f1, unt32 f2) OB_HIDDEN;

/**
 * Flags used by ob_numeric_builder
 */
//@{
#define OB_ARRAY 1
#define OB_MULTI 2
#define OB_VECTOR 4
#define OB_COMPLEX 8
#define OB_FLOAT 16
#define OB_INT 32
#define OB_UNT 64
//@}

struct ob_numeric_builder;
/**
 * container for incrementally building compund numeric types
 */
typedef struct ob_numeric_builder ob_numeric_builder;

/**
 * Allocate a new ob_numeric_builder.
 */
ob_retort ob_nb_new (ob_numeric_builder **nb_out) OB_HIDDEN;

/**
 * Enter an array, vector, complex, or multivector.
 */
ob_retort ob_nb_enter (ob_numeric_builder *nb_in, unt8 flags) OB_HIDDEN;

/**
 * Leave an array, vector, complex, or multivector.
 */
ob_retort ob_nb_leave (ob_numeric_builder *nb_in, unt8 flags) OB_HIDDEN;

/**
 * Append an integer to the builder.
 */
ob_retort ob_nb_push_integer (ob_numeric_builder **nb_inout, bool isUnsigned,
                              int bits, unt64 val) OB_HIDDEN;

/**
 * Append a floating-point number to the builder.
 */
ob_retort ob_nb_push_float (ob_numeric_builder **nb_inout, int bits,
                            float64 val) OB_HIDDEN;

/**
 * Get a pointer to the builder's data.
 */
const void *ob_nb_pointer (const ob_numeric_builder *nb_in) OB_HIDDEN;

/**
 * Validate the builder and return information about what it contains.
 */
ob_retort ob_nb_dimensions (const ob_numeric_builder *nb_in, unt8 *flags_out,
                            unt8 *vsize_out, unt8 *bits,
                            unt64 *breadth_out) OB_HIDDEN;

/**
 * Returns true if an equal number of "enter" and "leave" have been done.
 */
bool ob_nb_done (const ob_numeric_builder *nb_in) OB_HIDDEN;

/**
 * Free an ob_numeric_builder.
 */
void ob_nb_free (ob_numeric_builder **nb_inout) OB_HIDDEN;

/**
 * Analyzes the given UTF-8 string.  On return, *invalid is true if
 * the supplied string is not valid UTF-8, and *multiline is true if
 * the supplied string contains at least one newline character.
 */
void ob_analyze_utf8 (const char *utf8, int64 len, bool *invalid,
                      bool *multiline) OB_HIDDEN;

/**
 * Converts an older encoding of an ob_retort to its newer encoding.
 */
ob_retort ob_old_retort_to_new (ob_retort ancient_value,
                                unt32 version) OB_HIDDEN;

/**
 * Converts the new encoding of an ob_retort to an older encoding.
 */
ob_retort ob_new_retort_to_old (ob_retort modern_value,
                                unt32 version) OB_HIDDEN;

#ifdef __cplusplus
}
#endif

#endif /* PLASMA_UTIL_KILT */
