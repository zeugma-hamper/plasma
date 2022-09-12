
/* (c)  oblong industries */

#ifndef PROTEIN_GURGLINGS
#define PROTEIN_GURGLINGS



#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-api.h"
#include "libLoam/c/ob-retorts.h"

#include "libPlasma/c/plasma-types.h"



#ifdef __cplusplus
extern "C" {
#endif


/**
 * protein: read-only assembly of
 *   - slaw list of descrips, elements of which are usually slaw
 *     strings; possible for descrips list to be empty.
 *   - slaw list of ingests, which is usually a map, and the keys of the
 *     map are usually strings; possible for ingests list to be empty.
 *   - an optional pile of uninterprable/arbitrary ("rude") data
 *     which is useful for things like video frames, etc.
 *
 * actually a kind of slaw, under the covers
 */

OB_PLASMA_API protein protein_from (bslaw descrips, bslaw ingests);
OB_PLASMA_API protein protein_from_ff (slaw descrips, slaw ingests);
OB_PLASMA_API protein protein_from_lf (bslaw descrips, slaw ingests);
OB_PLASMA_API protein protein_from_fl (slaw descrips, bslaw ingests);
OB_PLASMA_API protein protein_from_llr (bslaw descrips, bslaw ingests,
                                        const void *rude, int64 rude_len);
OB_PLASMA_API protein protein_from_ffr (slaw descrips, slaw ingests,
                                        const void *rude, int64 rude_len);
OB_PLASMA_API protein protein_from_lfr (bslaw descrips, slaw ingests,
                                        const void *rude, int64 rude_len);
OB_PLASMA_API protein protein_from_flr (slaw descrips, bslaw ingests,
                                        const void *rude, int64 rude_len);

OB_PLASMA_API bslaw protein_descrips (bprotein prot);
OB_PLASMA_API bslaw protein_ingests (bprotein prot);
OB_PLASMA_API const void *protein_rude (bprotein prot, int64 *len);

/**
 * If \a needle is a list, performs a slaw_list_gapsearch() of
 * \a needle in \a haystack.  Otherwise, performs a slaw_list_find()
 * of \a needle in \a haystack.  Returns the index of the match in
 * \a haystack, or negative if no match.
 */
OB_PLASMA_API int64 protein_search (bprotein haystack, bslaw needle);

/**
 * If \a needle is a list, performs a the search specified by \a how of
 * \a needle in \a haystack.  Otherwise, performs a slaw_list_find()
 * of \a needle in \a haystack.  Returns the index of the match in
 * \a haystack, or negative if no match.
 */
OB_PLASMA_API int64 protein_search_ex (bprotein haystack, bslaw needle,
                                       Protein_Search_Type how);

//
//
//

#define protein_dup slaw_dup
#define protein_free slaw_free
#define protein_len slaw_len
#define proteins_equal slawx_equal

#define Free_Protein(p)                                                        \
  do                                                                           \
    {                                                                          \
      protein_free (p);                                                        \
      (p) = NULL;                                                              \
    }                                                                          \
  while (0)


/**
 * An empty protein has no descrips, ingests, or rude data.
 * Create one with protein_from (NULL, NULL).
 */
OB_PLASMA_API bool protein_is_empty (bprotein p);

OB_PLASMA_API bool protein_is_nonstandard (bprotein p);



/**
 *                  Swaps the endianness of the given protein, in place,
 *                  unconditionally.
 *
 * \note            Generally you probably want to call protein_fix_endian
 *                  instead, which only swaps if necessary.
 *
 * \note            You can't use this function to swap from native
 *                  byte order to non-native order, because it expects
 *                  the words to make sense after it swaps them.  You can
 *                  only swap from non-native to native byte order.
 *
 * \param[in,out]   p is the protein to swap
 *
 * \return          OB_OK if successful
 * \return          SLAW_CORRUPT_PROTEIN if the protein is invalid
 * \return          SLAW_CORRUPT_SLAW if a slaw in the protein is invalid
 * \return          SLAW_UNIDENTIFIED_SLAW if an unknown slaw is encountered
 */
OB_PLASMA_API ob_retort protein_swap_endian (protein p);

/**                 If the specified protein has the same endianness as
 *                  the host, does nothing.  If the specified protein has
 *                  the opposite endianness of the host, swaps the endianness
 *                  of the protein, in-place, so that it becomes a protein
 *                  with the same endianness as the host.
 *
 * \note            Rude data and nonstandard proteins are left unswapped.
 *
 * \param[in,out]   p is the protein to (maybe) swap
 *
 * \return          OB_OK if successful
 * \return          SLAW_CORRUPT_PROTEIN if the protein is invalid
 * \return          SLAW_CORRUPT_SLAW if a slaw in the protein is invalid
 * \return          SLAW_UNIDENTIFIED_SLAW if an unknown slaw is encountered
 */
OB_PLASMA_API ob_retort protein_fix_endian (protein p);

#ifdef __cplusplus
}
#endif


#endif
