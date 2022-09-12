
/* (c)  oblong industries */

#ifndef PLASMA_PRIVATE_PRACTICE
#define PLASMA_PRIVATE_PRACTICE

#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-api.h"
#include "libPlasma/c/slaw-io.h"

#include "libPlasma/c/private/slaw-viscera-private.h"
#include "libPlasma/c/private/slaw-numeric-ilk-rumpus-private.h"
#include "libPlasma/c/private/private-versioning.h"

#include <string.h> /* need memcpy */

#ifdef __cplusplus
extern "C" {
#endif

slaw slaw_string_raw (int64 len) OB_HIDDEN;
slaw slaw_numeric_array_raw (slaw_oct unit_ilk, unt32 unit_blen,
                             unt64 breadth) OB_HIDDEN;
const void *slaw_numeric_nonarray_emit (bslaw s) OB_HIDDEN;

// need to cast away const to use slaw_string_raw
#define SLAW_STRING_EMIT_WRITABLE(s) ((char *) slaw_string_emit (s))

//
//
//

slaw slaw_alloc (unt64 quadlen) OB_HIDDEN;
unt64 slaw_octlen (bslaw s) OB_HIDDEN;

static inline OB_ALWAYS_INLINE void
slaw_copy_octs_from_to (bslaw fromS, slaw toS, unt64 octlen)
{
  if (octlen != 0)
    memcpy (toS, fromS, 8 * octlen);
}

//
//
//

/**                 Swaps the endianness of the given slaw, in place,
 *                  unconditionally.
 *
 * \note            You can't use this function to swap from native
 *                  byte order to non-native order, because it expects
 *                  the words to make sense after it swaps them.  You can
 *                  only swap from non-native to native byte order.
 *
 * \param[in,out]   s is the slaw to swap
 *
 * \return          OB_OK if successful
 * \return          SLAW_CORRUPT_PROTEIN if an embedded protein is invalid
 * \return          SLAW_CORRUPT_SLAW if the slaw is invalid
 * \return          SLAW_UNIDENTIFIED_SLAW if an unknown slaw is encountered
 */
ob_retort slaw_swap (slaw s, slaw stop) OB_HIDDEN;

/**                 Swaps the endianness of multiple consecutive slawx,
 *                  such as would be contained inside the payload of a
 *                  slaw list, or in the descrips or ingests of a protein.
 *
 * \note            This function is mostly for internal use.  You probably
 *                  just want to call slaw_swap unless you're doing
 *                  something weird.
 *
 * \param[in,out]   s is the beginning of the slawx to swap
 *
 * \param[in]       stop points to the word after the slawx to swap
 *
 * \return          OB_OK if successful
 * \return          SLAW_CORRUPT_PROTEIN if an embedded protein is invalid
 * \return          SLAW_CORRUPT_SLAW if the slaw is invalid
 * \return          SLAW_UNIDENTIFIED_SLAW if an unknown slaw is encountered
 */
ob_retort slaw_swap_sequence (slaw s, slaw stop) OB_HIDDEN;

// a function for categorizing slawx (handy for switch statements)

slaw_type slaw_gettype (bslaw s) OB_HIDDEN;

// Sorts the slawx in the slabu.  For now this is private, but in the
// future we could make this public if it's considered useful.
void slabu_sort (slabu *sb) OB_HIDDEN;

typedef struct
{
  const void *descrips;
  int64 descrips_len;
  const void *ingests;
  int64 ingests_len;
  const void *rude;
  int64 rude_len;
} opaque_protein_info;

OB_HIDDEN protein protein_from_opaque (const opaque_protein_info *opaque);

// I/O-related stuff

typedef ob_retort (*slaw_close_func) (void *data);
typedef ob_retort (*slaw_read_func) (slaw_input f, slaw *s);
typedef ob_retort (*slaw_write_func) (slaw_output f, bslaw s);

struct slaw_input_struct
{
  slaw_read_func rfunc;
  slaw_close_func cfunc;
  void *data;
};

struct slaw_output_struct
{
  slaw_write_func wfunc;
  slaw_close_func cfunc;
  void *data;
};

OB_HIDDEN slaw_input new_slaw_input (void);
OB_HIDDEN slaw_output new_slaw_output (void);

OB_HIDDEN extern const byte ob_binary_header[4];
OB_HIDDEN OB_CONST bool ob_i_am_big_endian (void);

/* values for "typ" */
#define PLASMA_BINARY_FILE_TYPE_SLAW 1
#define PLASMA_BINARY_FILE_TYPE_POOL 2

/* values for "flags" */
#define PLASMA_BINARY_FILE_FLAG_BIG_ENDIAN_SLAW (1 << 0)

/* To prevent various sort of attacks or bugs, it seems wise to have a
 * maximum protein (and slaw) size.  We don't want to set this too small,
 * which could prevent legitimate uses.  Note that there is also a maximum
 * pool size set in pool_mmap.h, which is certainly a related concept,
 * but we are going to set the max protein size to be smaller.
 *
 * For 32-bit systems, set the max size to 1G, on the basis that since plasma
 * (unfortunately) works by generally copying whole slawx and proteins
 * multiple times as you build them up, it probably makes no sense to have
 * a protein larger than 1/4 of your address space.
 *
 * For 64-bit systems, this is just set on the basis "I can't imagine
 * anyone wanting a protein larger than this, for now."  (When someone does,
 * the number can simply be increased.)
 */
#define MAX_SLAW_SIZE                                                          \
  ((sizeof (void *) < 8) ? (OB_GIGABYTE) : (256 * OB_GIGABYTE))


#ifdef __cplusplus
}
#endif

#endif /* PLASMA_PRIVATE_PRACTICE */
