
/* (c)  ANIMIST contributors */

#ifndef PLASMA_INFO_DUMP
#define PLASMA_INFO_DUMP

#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-attrs.h"
#include "libLoam/c/ob-api.h"

#include "libPlasma/c/plasma-types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Returns a slaw map containing information about how libPlasma is
 * configured, such as whether libYaml and OpenSSL are enabled, and if
 * so, which versions of those libraries are used.  The exact format
 * is subject to change.
 *
 * Currently, the map returned contains two keys, "YAML" and "TLS".
 * If the corresponding feature is enabled, the value is a string
 * containing the version number of the library.  If the corresponding
 * feature is disabled, the value is the boolean false.
 */
OB_PLASMA_API slaw plasma_info (void);

#ifdef __cplusplus
}
#endif

#endif /* PLASMA_INFO_DUMP */
