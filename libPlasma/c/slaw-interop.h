
/* (c)  oblong industries */

#ifndef SLAW_INTEROP_PTERODACTYL
#define SLAW_INTEROP_PTERODACTYL

#include "libPlasma/c/slaw.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum slaw_endian {
  SLAW_ENDIAN_BIG = 100,
  SLAW_ENDIAN_LITTLE,
  SLAW_ENDIAN_UNKNOWN /* only works for proteins */
} slaw_endian;

#ifdef __BIG_ENDIAN__
#define SLAW_ENDIAN_CURRENT SLAW_ENDIAN_BIG
#define SLAW_ENDIAN_OPPOSITE SLAW_ENDIAN_LITTLE
#else
#define SLAW_ENDIAN_CURRENT SLAW_ENDIAN_LITTLE
#define SLAW_ENDIAN_OPPOSITE SLAW_ENDIAN_BIG
#endif

typedef unt8 slaw_version;

#define SLAW_VERSION_CURRENT 2

/**
 * Converts a slaw from a potentially foreign endianness and/or
 * a potentially old version, to a slaw of the current version
 * and the current endianness.  You must supply a pointer to the
 * variable that holds the slaw.  Depending on the transformation
 * to be performed, the slaw might be modified in place, or it
 * might be freed and replaced by a newly allocated slaw.
 * You must always specify the version of the input slaw.
 * You must specify the endianness of the input slaw, unless
 * you know it is a protein, in which case you can specify
 * SLAW_ENDIAN_UNKNOWN, and let the endianness of the protein
 * be auto-detected.
 */
OB_PLASMA_API ob_retort slaw_convert_from (slaw *s, slaw_endian e,
                                           slaw_version v);

/**
 * Converts a slaw from the current version and endianness to a
 * potentially old version (but still the current endianness;
 * converting to the non-native endianness is not supported).
 * You must supply a pointer to the
 * variable that holds the slaw.  Depending on the transformation
 * to be performed, the slaw might be modified in place, or it
 * might be freed and replaced by a newly allocated slaw.
 * The length of the converted protein is written into "len".
 */
OB_PLASMA_API ob_retort slaw_convert_to (slaw *s, slaw_version v, int64 *len);

#ifdef __cplusplus
}
#endif

#endif /* SLAW_INTEROP_PTERODACTYL */
