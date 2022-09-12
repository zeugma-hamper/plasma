
/* (c)  oblong industries */

#ifndef SLAW_PATH_FINDER
#define SLAW_PATH_FINDER


#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-api.h"

#include "libPlasma/c/slaw.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Calls slaw_path_get_slaw(), and if the resulting slaw is a
 * string, returns that string.  Otherwise, returns dflt.
 */
OB_PLASMA_API const char *slaw_path_get_string (bslaw s, const char *path,
                                                const char *dflt);

/**
 * Combines slaw_path_get_slaw() with slaw_to_int64().
 * Returns dflt if path does not exist or cannot be coerced.
 */
OB_PLASMA_API int64 slaw_path_get_int64 (bslaw s, const char *path, int64 dflt);

/**
 * Combines slaw_path_get_slaw() with slaw_to_unt64().
 * Returns dflt if path does not exist or cannot be coerced.
 */
OB_PLASMA_API unt64 slaw_path_get_unt64 (bslaw s, const char *path, unt64 dflt);

/**
 * Combines slaw_path_get_slaw() with slaw_to_float64().
 * Returns dflt if path does not exist or cannot be coerced.
 */
OB_PLASMA_API float64 slaw_path_get_float64 (bslaw s, const char *path,
                                             float64 dflt);

/**
 * Combines slaw_path_get_slaw() with slaw_to_boolean().
 * Returns dflt if path does not exist or cannot be coerced.
 */
OB_PLASMA_API bool slaw_path_get_bool (bslaw s, const char *path, bool dflt);

/**
 * Splits path into multiple components at the "/" character, and
 * then, assuming s is a nested map, descends one level of map
 * for each key in the path.  So, if path contains no slashes,
 * this is much like slaw_map_find_c().
 */
OB_PLASMA_API bslaw slaw_path_get_slaw (bslaw s, const char *path);

#ifdef __cplusplus
}
#endif

#endif /* SLAW_PATH_FINDER */
