
/* (c)  oblong industries */

#ifndef SLAW_COERCE_INTERROGATION
#define SLAW_COERCE_INTERROGATION

#include "libLoam/c/ob-types.h"
#include "libPlasma/c/slaw.h"

#ifdef __cplusplus
extern "C" {
#endif

/**                 Attempts to interpret the given slaw as an unt64 if
 *                  at all possible.  In particular, if s is any atomic
 *                  numeric type, it will be converted to an unt64.
 *                  If s is a string, the string will be
 *                  parsed as an unt64 if possible.
 *                  Is s is a list of one element or an array of one element,
 *                  then that one element shall be treated to the same
 *                  opportunities for coercion as described above.
 *
 * \param[in]       s is the slaw to be coerced
 *
 * \param[out]      result gets integer if return value is OB_OK,
 *                  no guarantee otherwise
 *
 * \return          one of OB_OK, SLAW_RANGE_ERR, or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_unt64 (bslaw s, unt64 *result);

/**                 Attempts to interpret the given slaw as an unt32 if
 *                  at all possible. The conversion is performed by @ref
 *                  slaw_to_unt64, which see, and the result is cast to
 *                  unt32.
 * \return          one of OB_OK, SLAW_RANGE_ERR, or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_unt32 (bslaw s, unt32 *result);

/**                 Attempts to interpret the given slaw as an unt16 if
 *                  at all possible. The conversion is performed by @ref
 *                  slaw_to_unt16, which see, and the result is cast to
 *                  unt16.
 * \return          one of OB_OK, SLAW_RANGE_ERR, or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_unt16 (bslaw s, unt16 *result);

/**                 Attempts to interpret the given slaw as an unt8 if
 *                  at all possible. The conversion is performed by @ref
 *                  slaw_to_unt8, which see, and the result is cast to
 *                  unt8.
 * \return          one of OB_OK, SLAW_RANGE_ERR, or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_unt8 (bslaw s, unt8 *result);

/**                 Attempts to interpret the given slaw as an int64 if
 *                  at all possible.  In particular, if s is any atomic
 *                  numeric type, it will be converted to an int64.
 *                  If s is a string, the string will be
 *                  parsed as an int64 if possible.
 *                  Is s is a list of one element or an array of one element,
 *                  then that one element shall be treated to the same
 *                  opportunities for coercion as described above.
 *
 * \param[in]       s is the slaw to be coerced
 *
 * \param[out]      result gets integer if return value is OB_OK,
 *                  no guarantee otherwise
 *
 * \return          one of OB_OK, SLAW_RANGE_ERR, or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_int64 (bslaw s, int64 *result);

/**                 Attempts to interpret the given slaw as an int32 if
 *                  at all possible. The conversion is performed by @ref
 *                  slaw_to_int64, which see, and the result is cast to
 *                  int32.
 * \return          one of OB_OK, SLAW_RANGE_ERR, or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_int32 (bslaw s, int32 *result);

/**                 Attempts to interpret the given slaw as an int16 if
 *                  at all possible. The conversion is performed by @ref
 *                  slaw_to_int16, which see, and the result is cast to
 *                  int16.
 * \return          one of OB_OK, SLAW_RANGE_ERR, or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_int16 (bslaw s, int16 *result);

/**                 Attempts to interpret the given slaw as an int8 if
 *                  at all possible. The conversion is performed by @ref
 *                  slaw_to_int8, which see, and the result is cast to
 *                  int8.
 * \return          one of OB_OK, SLAW_RANGE_ERR, or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_int8 (bslaw s, int8 *result);

/**                 Attempts to interpret the given slaw as a float64 if
 *                  at all possible.  In particular, if s is any atomic
 *                  numeric type, it will be converted to a float64.
 *                  If s is a string, the string will be
 *                  parsed as a float64 if possible.
 *                  Is s is a list of one element or an array of one element,
 *                  then that one element shall be treated to the same
 *                  opportunities for coercion as described above.
 *
 * \param[in]       s is the slaw to be coerced
 *
 * \param[out]      result gets float if return value is OB_OK,
 *                  no guarantee otherwise
 *
 * \return          one of OB_OK, SLAW_RANGE_ERR, or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_float64 (bslaw s, float64 *result);

/**                 Attempts to interpret the given slaw as an float32 if
 *                  at all possible. The conversion is performed by @ref
 *                  slaw_to_int64, which see, and the result is cast to
 *                  float32.
 * \return          one of OB_OK, SLAW_RANGE_ERR, or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_float32 (bslaw s, float32 *result);

/**                 Attempts to interpret the given slaw as a length-2
 *                  vector if at all possible.  In particular, if s is a
 *                  list of the proper length, then the elements of the
 *                  list will be coerced as by slaw_to_float64.
 *                  If s is either an array or a vector of any real type,
 *                  of the proper length, it will be coerced to a vector
 *                  of float64.  And if s is a string, it will be parsed
 *                  as numbers separated by commas, if possible.
 *
 * \param[in]       s is the slaw to be coerced
 *
 * \param[out]      result gets the vector if return value is OB_OK,
 *                  otherwise unchanged
 *
 * \return          one of OB_OK, SLAW_RANGE_ERR, SLAW_WRONG_LENGTH,
 *                  or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_v2 (bslaw s, v2float64 *result);

/**                 Attempts to interpret the given slaw as a length-3
 *                  vector if at all possible.  In particular, if s is a
 *                  list of the proper length, then the elements of the
 *                  list will be coerced as by slaw_to_float64.
 *                  If s is either an array or a vector of any real type,
 *                  of the proper length, it will be coerced to a vector
 *                  of float64.  And if s is a string, it will be parsed
 *                  as numbers separated by commas, if possible.
 *
 * \param[in]       s is the slaw to be coerced
 *
 * \param[out]      result gets the vector if return value is OB_OK,
 *                  otherwise unchanged
 *
 * \return          one of OB_OK, SLAW_RANGE_ERR, SLAW_WRONG_LENGTH,
 *                  or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_v3 (bslaw s, v3float64 *result);

/**                 Attempts to interpret the given slaw as a length-4
 *                  vector if at all possible.  In particular, if s is a
 *                  list of the proper length, then the elements of the
 *                  list will be coerced as by slaw_to_float64.
 *                  If s is either an array or a vector of any real type,
 *                  of the proper length, it will be coerced to a vector
 *                  of float64.  And if s is a string, it will be parsed
 *                  as numbers separated by commas, if possible.
 *
 * \param[in]       s is the slaw to be coerced
 *
 * \param[out]      result gets the vector if return value is OB_OK,
 *                  otherwise unchanged
 *
 * \return          one of OB_OK, SLAW_RANGE_ERR, SLAW_WRONG_LENGTH,
 *                  or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_v4 (bslaw s, v4float64 *result);

/**                 Implements the same coercion behavior as slaw_to_v2(),
 *                  slaw_to_v3(), and slaw_to_v4(), but for an arbitrary
 *                  length.
 *
 * \param[in]       s is the slaw to be coerced
 *
 * \param[out]      result gets the vector elements if return value is OB_OK,
 *                  otherwise unchanged
 *
 * \param[in]       capacity is the maximum number of float64s which can
 *                  be written to \a result.
 *
 * \param[out]      len is the number of vector elements actually written
 *                  to \a result, which will be less than or equal to
 *                  \a capacity.
 *
 * \return          one of OB_OK, SLAW_RANGE_ERR, SLAW_WRONG_LENGTH,
 *                  or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_vn (bslaw s, float64 *result, int capacity,
                                    int *len);


/**       Attempts to interpret the given slaw as a v ## N ## T, by
 *         first calling slaw_to_v ## N and then casting the
 *         components of the result to T
 */
#define DECLARE_V_COERCION(N, T)                                               \
  OB_PLASMA_API ob_retort slaw_to_v##N##T (bslaw s, v##N##T *result)

#define FOR_ALL_NUMERIC_TYPES(N, M)                                            \
  M (N, int8);                                                                 \
  M (N, int16);                                                                \
  M (N, int32);                                                                \
  M (N, int64);                                                                \
  M (N, unt8);                                                                 \
  M (N, unt16);                                                                \
  M (N, unt32);                                                                \
  M (N, unt64);                                                                \
  M (N, float32);                                                              \
  M (N, float64)

FOR_ALL_NUMERIC_TYPES (2, DECLARE_V_COERCION);
FOR_ALL_NUMERIC_TYPES (3, DECLARE_V_COERCION);
FOR_ALL_NUMERIC_TYPES (4, DECLARE_V_COERCION);

#undef FOR_ALL_NUMERIC_TYPES
#undef DECLARE_V_COERCION

/**                 Attempts to interpret the given slaw as a boolean if
 *                  at all possible.  In particular, if s is any atomic
 *                  numeric type with a value of "0" or "1", it will
 *                  be converted to false or true, respectively.
 *                  If s is a string, it may be either "true" or "false",
 *                  case-insensitively.
 *                  Is s is a list of one element or an array of one element,
 *                  then that one element shall be treated to the same
 *                  opportunities for coercion as described above.
 *
 * \param[in]       s is the slaw to be coerced
 *
 * \param[out]      result gets boolean if return value is OB_OK,
 *                  no guarantee otherwise
 *
 * \return          one of OB_OK, SLAW_RANGE_ERR, or SLAW_NOT_NUMERIC
 */
OB_PLASMA_API ob_retort slaw_to_boolean (bslaw s, bool *result);

#ifdef __cplusplus
}
#endif

#endif /* SLAW_COERCE_INTERROGATION */
