
/* (c)  oblong industries */

#include "libPlasma/c/slaw-ordering.h"
#include "libPlasma/c/slaw.h"
#include "libPlasma/c/protein.h"
#include "libPlasma/c/private/plasma-private.h"
#include "libPlasma/c/private/plasma-util.h"

#include <string.h>
#include <stdlib.h>

static int protein_semantic_compare (bprotein p1, bprotein p2)
{
  bool nonstd1, nonstd2;
  int64 rl1, rl2;
  const void *rude1, *rude2;
  int c;

  nonstd1 = protein_is_nonstandard (p1);
  nonstd2 = protein_is_nonstandard (p2);

  if (nonstd1 && !nonstd2)
    return 1; /* treat nonstandard proteins as greater */
  else if (!nonstd1 && nonstd2)
    return -1;

  if (nonstd1)
    {
      unt64 len1, len2;
      len1 = protein_len (p1);
      len2 = protein_len (p2);

      if (len1 < len2)
        return -1; /* sort nonstandard proteins by size first */
      else if (len1 > len2)
        return 1;

      return memcmp (p1, p2, (size_t) len1); /* compare the bytes */
    }

  c = slaw_semantic_compare (protein_descrips (p1), protein_descrips (p2));
  if (c != 0)
    return c;

  c = slaw_semantic_compare (protein_ingests (p1), protein_ingests (p2));
  if (c != 0)
    return c;

  rude1 = protein_rude (p1, &rl1);
  rude2 = protein_rude (p2, &rl2);

  if (rl1 < rl2)
    return -1; /* sort by size of rude data */
  else if (rl1 > rl2)
    return 1;

  if (rl1 != 0)
    {
      return memcmp (rude1, rude2, (size_t) rl1); /* compare the bytes */
    }

  return 0; /* must be equal */
}

int slaw_semantic_compare (bslaw s1, bslaw s2)
{
  slaw_type t1, t2;

  t1 = slaw_gettype (s1);
  t2 = slaw_gettype (s2);

  if (t1 < t2)
    return -1;
  else if (t1 > t2)
    return 1;

  /* slawx are the same type, so do type-specific comparison */
  switch (t1)
    {
      case SLAW_TYPE_NULL:
      case SLAW_TYPE_NIL:
        return 0; /* nil always equals nil */
      case SLAW_TYPE_BOOLEAN:
        {
          int b1, b2;
          b1 = *slaw_boolean_emit (s1);
          b2 = *slaw_boolean_emit (s2);
          if (b1 < b2)
            return -1;
          else if (b1 > b2)
            return 1;
          else
            return 0;
        }
      case SLAW_TYPE_STRING:
        return strcmp (slaw_string_emit (s1), slaw_string_emit (s2));
      case SLAW_TYPE_NUMERIC:
        {
          slaw_oct pers1, pers2;
          bool isArray1, isArray2;
          unt32 primBytes;
          unt64 nprims, i;
          const void *e1, *e2;
          bool isUnsigned, isFloat;
          int bsize;

          pers1 = (SLAW_ILK (s1) & SLAW_NUMERIC_PERSONALITY_BITS);
          pers2 = (SLAW_ILK (s2) & SLAW_NUMERIC_PERSONALITY_BITS);
          if (pers1 < pers2)
            return -1;
          else if (pers1 > pers2)
            return 1;

          isArray1 = slaw_is_numeric_array (s1);
          isArray2 = slaw_is_numeric_array (s2);

          if (isArray1 && !isArray2)
            return 1; /* treat arrays as greater than non-arrays */
          if (!isArray1 && isArray2)
            return -1;

          primBytes = SLAW_NUMERIC_PRIM_BYTES (s1);

          nprims = (bsize = slaw_numeric_unit_bsize (s1)) / primBytes;
          e1 = (bsize > 4 ? (void *) (s1 + 1)
                          : (void *) (SLAW_SPECIAL_BYTES (s1, bsize)));
          e2 = (bsize > 4 ? (void *) (s2 + 1)
                          : (void *) (SLAW_SPECIAL_BYTES (s2, bsize)));

          if (isArray1)
            {
              unt64 breadth1 = slaw_numeric_array_count (s1);
              unt64 breadth2 = slaw_numeric_array_count (s2);

              if (breadth1 < breadth2)
                return -1; /* treat smaller arrays as less */
              if (breadth1 > breadth2)
                return 1;

              nprims *= breadth1;

              e1 = slaw_numeric_array_emit (s1);
              e2 = slaw_numeric_array_emit (s2);
            }

          isUnsigned = slaw_is_numeric_unt (s1);
          isFloat = slaw_is_numeric_float (s1);

          for (i = 0; i < nprims; i++)
            {
              if (isFloat)
                {
                  int c;
                  if (primBytes == 8)
                    c = private_float64_compare (*(unt64 *) e1, *(unt64 *) e2);
                  else
                    c = private_float32_compare (*(unt32 *) e1, *(unt32 *) e2);

                  if (c != 0)
                    return c;
                }
              else if (isUnsigned)
                {
                  switch (primBytes)
                    {
                      case 1:
                        if (*(unt8 *) e1 < *(unt8 *) e2)
                          return -1;
                        else if (*(unt8 *) e1 > *(unt8 *) e2)
                          return 1;
                        else
                          break;
                      case 2:
                        if (*(unt16 *) e1 < *(unt16 *) e2)
                          return -1;
                        else if (*(unt16 *) e1 > *(unt16 *) e2)
                          return 1;
                        else
                          break;
                      case 4:
                        if (*(unt32 *) e1 < *(unt32 *) e2)
                          return -1;
                        else if (*(unt32 *) e1 > *(unt32 *) e2)
                          return 1;
                        else
                          break;
                      case 8:
                        if (*(unt64 *) e1 < *(unt64 *) e2)
                          return -1;
                        else if (*(unt64 *) e1 > *(unt64 *) e2)
                          return 1;
                        else
                          break;
                    }
                }
              else
                {
                  switch (primBytes)
                    {
                      case 1:
                        if (*(int8 *) e1 < *(int8 *) e2)
                          return -1;
                        else if (*(int8 *) e1 > *(int8 *) e2)
                          return 1;
                        else
                          break;
                      case 2:
                        if (*(int16 *) e1 < *(int16 *) e2)
                          return -1;
                        else if (*(int16 *) e1 > *(int16 *) e2)
                          return 1;
                        else
                          break;
                      case 4:
                        if (*(int32 *) e1 < *(int32 *) e2)
                          return -1;
                        else if (*(int32 *) e1 > *(int32 *) e2)
                          return 1;
                        else
                          break;
                      case 8:
                        if (*(int64 *) e1 < *(int64 *) e2)
                          return -1;
                        else if (*(int64 *) e1 > *(int64 *) e2)
                          return 1;
                        else
                          break;
                    }
                }

              e1 = primBytes + (byte *) e1;
              e2 = primBytes + (byte *) e2;
            }

          return 0; /* every element is equal */
        }
      case SLAW_TYPE_CONS:
        {
          int c;
          c = slaw_semantic_compare (slaw_cons_emit_car (s1),
                                     slaw_cons_emit_car (s2));
          if (c != 0)
            return c;
          return slaw_semantic_compare (slaw_cons_emit_cdr (s1),
                                        slaw_cons_emit_cdr (s2));
        }
      case SLAW_TYPE_LIST:
        {
          bslaw e1, e2;
          bool m1, m2;

          // first need to disambiguate a list from a map
          m1 = slaw_is_map (s1);
          m2 = slaw_is_map (s2);

          if (m1 < m2)
            return -1;
          else if (m1 > m2)
            return 1;

          for (e1 =
                 (slaw_list_count (s1) > 0 ? slaw_list_emit_first (s1) : NULL),
              e2 =
                 (slaw_list_count (s2) > 0 ? slaw_list_emit_first (s2) : NULL);
               e1 != NULL && e2 != NULL; e1 = slaw_list_emit_next (s1, e1),
              e2 = slaw_list_emit_next (s2, e2))
            {
              int c = slaw_semantic_compare (e1, e2);
              if (c != 0)
                return c;
            }

          if (e1)
            return 1; /* first list is longer, therefore greater */
          else if (e2)
            return -1; /* first list is shorter, therefore less */

          return 0; /* lists ended together, therefore equal */
        }
      case SLAW_TYPE_PROTEIN:
        return protein_semantic_compare (s1, s2);
      default:
        return 0; /* let's say unknown slawx are equal */
    }
}
