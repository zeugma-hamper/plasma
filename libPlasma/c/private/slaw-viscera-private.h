
/* (c)  oblong industries */

#ifndef SLAW_VISCERA_PRIVATE_BOILINGS
#define SLAW_VISCERA_PRIVATE_BOILINGS

#include "libLoam/c/ob-types.h"
#include "libPlasma/c/slaw.h"

typedef unt64 slaw_oct;

struct _slaw
{
  slaw_oct o;
};

struct _slaw_bundle
{
  slaw *esses;
  bool *effs; /* for each ess, whether to free it */
  int64 numEsses, maxNumEsses;
  bool obeysMapInvariant; /* all cons, no duplicates */
};

#define SLAW_ILK(s) ((s)->o)

#define SLAW_NIBBLE_SHIFTY 60
#define SLAW_WEE_STRING_LEN_SHIFTY 56
#define SLAW_FULL_STRING_PAD_SHIFTY 56
#define SLAW_WEE_CONTAINER_COUNT_SHIFTY 56

#define SLAW_OCTLEN_MASK OB_CONST_U64 (0x00ffffffffffffff)
#define SLAW_EMBEDDED_OCTLEN(s) (SLAW_ILK (s) & SLAW_OCTLEN_MASK)

#define SLAW_ILK_NIBBLE(s) (SLAW_ILK (s) >> SLAW_NIBBLE_SHIFTY)

#define SLAW_NIB_SWAPPED_PROTEIN 0
#define SLAW_NIB_PROTEIN 1
#define SLAW_NIB_SYMBOL 2
#define SLAW_NIB_WEE_STRING 3
#define SLAW_NIB_LIST 4
#define SLAW_NIB_MAP 5
#define SLAW_NIB_CONS 6
#define SLAW_NIB_FULL_STRING 7
#define SLAW_NIB_SINGL_SINT 8
#define SLAW_NIB_SINGL_UINT 9
#define SLAW_NIB_SINGL_FLOAT 10
#define SLAW_NIB_RESERVED_11 11
#define SLAW_NIB_ARRAY_SINT 12
#define SLAW_NIB_ARRAY_UINT 13
#define SLAW_NIB_ARRAY_FLOAT 14
#define SLAW_NIB_RESERVED_15 15

#define SLAW_PROTEIN_ILK OB_CONST_U64 (0x1000000000000000)
#define SLAW_SWAPPED_ILK OB_CONST_U64 (0x0000000000000010)
#define SLAW_PROTEIN_ILKMASK OB_CONST_U64 (0xf0000000000000f0)
#define SLAW_IS_PROTEIN(s)                                                     \
  ((SLAW_ILK (s) & SLAW_PROTEIN_ILKMASK) == SLAW_PROTEIN_ILK)
#define SLAW_IS_SWAPPED_PROTEIN(s)                                             \
  ((SLAW_ILK (s) & SLAW_PROTEIN_ILKMASK) == SLAW_SWAPPED_ILK)

#define SLAW_STRING_ILK OB_CONST_U64 (0x3000000000000000)
#define SLAW_STRING_ILKMASK OB_CONST_U64 (0xb000000000000000)
#define SLAW_IS_STRING(s)                                                      \
  ((SLAW_ILK (s) & SLAW_STRING_ILKMASK) == SLAW_STRING_ILK)
#define SLAW_WEE_STRING_LEN(s)                                                 \
  ((SLAW_ILK (s) >> SLAW_WEE_STRING_LEN_SHIFTY) & 7)
#define SLAW_FULL_STRING_PAD(s)                                                \
  ((SLAW_ILK (s) >> SLAW_FULL_STRING_PAD_SHIFTY) & 7)

#define SLAW_LISTORMAP_ILK OB_CONST_U64 (0x4000000000000000)
#define SLAW_LISTORMAP_ILKMASK OB_CONST_U64 (0xe000000000000000)
#define SLAW_IS_LISTORMAP(s)                                                   \
  ((SLAW_ILK (s) & SLAW_LISTORMAP_ILKMASK) == SLAW_LISTORMAP_ILK)
#define SLAW_IS_LIST(s) (SLAW_ILK_NIBBLE (s) == SLAW_NIB_LIST)
#define SLAW_IS_MAP(s) (SLAW_ILK_NIBBLE (s) == SLAW_NIB_MAP)

#define SLAW_CONS_ILK OB_CONST_U64 (0x6200000000000000)
#define SLAW_CONS_ILKMASK OB_CONST_U64 (0xff00000000000000)
#define SLAW_IS_CONS(s) ((SLAW_ILK (s) & SLAW_CONS_ILKMASK) == SLAW_CONS_ILK)

#define SLAW_BOOL_ILK OB_CONST_U64 (0x2000000000000000)
#define SLAW_BOOL_ILKMASK OB_CONST_U64 (0xfffffffffffffffe)
#define SLAW_IS_BOOL(s) ((SLAW_ILK (s) & SLAW_BOOL_ILKMASK) == SLAW_BOOL_ILK)
#define SLAW_BOOLEAN_VALUE(s) (SLAW_ILK (s) & 1)

#define SLAW_NIL_ILK OB_CONST_U64 (0x2000000000000002)

#define SLAW_MAX_WEE_CONTAINER 15

#define SLAW_NUMERIC_PERSONALITY_BITS OB_CONST_U64 (0xffffc00000000000)
#define SLAW_NUMERIC_SIZE_SHIFTY 58
#define SLAW_NUMERIC_PRIM_BYTES(s)                                             \
  (1 << (3 & (SLAW_ILK (s) >> SLAW_NUMERIC_SIZE_SHIFTY)))
#define SLAW_NUMERIC_ILK OB_CONST_U64 (0x8000000000000000)
#define SLAW_IS_NUMERIC(s)                                                     \
  ((SLAW_ILK (s) & SLAW_NUMERIC_ILK) == SLAW_NUMERIC_ILK)
#define SLAW_NUMERIC_BSIZE_SHIFTY 46
#define SLAW_NUMERIFY(b)                                                       \
  ((((b) -OB_CONST_U64 (1)) << SLAW_NUMERIC_BSIZE_SHIFTY) | SLAW_NUMERIC_ILK)

#define SLAW_NUMERIC_SIZE_ILKMASK OB_CONST_U64 (0x8c00000000000000)
#define SLAW_IS_NUMERIC_SIZE(s, z)                                             \
  ((SLAW_ILK (s) & SLAW_NUMERIC_SIZE_ILKMASK)                                  \
   == (SLAW_NUMERIC_ILK | (((unt64) z) << SLAW_NUMERIC_SIZE_SHIFTY)))
#define SLAW_IS_NUMERIC_8BIT(s) SLAW_IS_NUMERIC_SIZE (s, 0)
#define SLAW_IS_NUMERIC_16BIT(s) SLAW_IS_NUMERIC_SIZE (s, 1)
#define SLAW_IS_NUMERIC_32BIT(s) SLAW_IS_NUMERIC_SIZE (s, 2)
#define SLAW_IS_NUMERIC_64BIT(s) SLAW_IS_NUMERIC_SIZE (s, 3)

#define SLAW_NUMERIC_INT_ILK OB_CONST_U64 (0x8000000000000000)
#define SLAW_NUMERIC_UNT_ILK OB_CONST_U64 (0x9000000000000000)
#define SLAW_NUMERIC_FLOAT_ILK OB_CONST_U64 (0xa000000000000000)
#define SLAW_NUMERIC_TYPE_ILKMASK OB_CONST_U64 (0xb000000000000000)

#define SLAW_IS_NUMERIC_INT(s)                                                 \
  ((SLAW_ILK (s) & SLAW_NUMERIC_TYPE_ILKMASK) == SLAW_NUMERIC_INT_ILK)
#define SLAW_IS_NUMERIC_UNT(s)                                                 \
  ((SLAW_ILK (s) & SLAW_NUMERIC_TYPE_ILKMASK) == SLAW_NUMERIC_UNT_ILK)
#define SLAW_IS_NUMERIC_FLOAT(s)                                               \
  ((SLAW_ILK (s) & SLAW_NUMERIC_TYPE_ILKMASK) == SLAW_NUMERIC_FLOAT_ILK)

#define SLAW_NUMERIC_COMPLEX_ILK OB_CONST_U64 (0x8200000000000000)
#define SLAW_NUMERIC_COMPLEX_ILKMASK OB_CONST_U64 (0x8300000000000000)
#define SLAW_IS_NUMERIC_COMPLEX(s)                                             \
  ((SLAW_ILK (s) & SLAW_NUMERIC_COMPLEX_ILKMASK) == SLAW_NUMERIC_COMPLEX_ILK)

#define SLAW_NUMERIC_ARRAY_ILK OB_CONST_U64 (0xc000000000000000)
#define SLAW_IS_NUMERIC_ARRAY(s)                                               \
  ((SLAW_ILK (s) & SLAW_NUMERIC_ARRAY_ILK) == SLAW_NUMERIC_ARRAY_ILK)

#define SLAW_NUMERIC_MVEC_ILK OB_CONST_U64 (0x8100000000000000)
#define SLAW_NUMERIC_MVEC_ILKMASK OB_CONST_U64 (0x8300000000000000)
#define SLAW_IS_NUMERIC_MVEC(s)                                                \
  ((SLAW_ILK (s) & SLAW_NUMERIC_MVEC_ILKMASK) == SLAW_NUMERIC_MVEC_ILK)

#define SLAW_NUMERIC_VECTOR_ILK OB_CONST_U64 (0x8000000000000000)
#define SLAW_NUMERIC_VECTOR_ILKMASK OB_CONST_U64 (0x8100000000000000)
#define SLAW_NUMERIC_VECTOR_SIZEMASK OB_CONST_U64 (0x00c0000000000000)
#define SLAW_IS_NUMERIC_VECTOR(s)                                              \
  ((SLAW_ILK (s) & SLAW_NUMERIC_VECTOR_ILKMASK) == SLAW_NUMERIC_VECTOR_ILK     \
   && (SLAW_ILK (s) & SLAW_NUMERIC_VECTOR_SIZEMASK) != 0)

#define SLAW_NUMERIC_UNIT_BSIZE_MASK 0xff
#define SLAW_NUMERIC_MAX_UNIT_BSIZE SLAW_NUMERIC_UNIT_BSIZE_MASK
#define SLAW_NUMERIC_UNIT_BSIZE(s)                                             \
  (1 + ((SLAW_ILK (s) >> SLAW_NUMERIC_BSIZE_SHIFTY)                            \
        & SLAW_NUMERIC_UNIT_BSIZE_MASK))

#define SLAW_NUMERIC_VECTOR_SHIFTY 54
#define SLAW_N_VECTOR_WIDTH(s)                                                 \
  (7 & (054324321 >> (3 * ((SLAW_ILK (s) >> SLAW_NUMERIC_VECTOR_SHIFTY) & 7))))
#define SLAW_NUMERIC_VECTOR_WIDTH(s)                                           \
  (SLAW_IS_NUMERIC (s) ? SLAW_N_VECTOR_WIDTH (s) : -1)

#define SLAW_NUMERIC_ARRAY_BREADTH_MASK OB_CONST_U64 (0x00003fffffffffff)
#define SLAW_N_ARRAY_BREADTH(s) (SLAW_ILK (s) & SLAW_NUMERIC_ARRAY_BREADTH_MASK)
#define SLAW_N_ARRAY_FIRST_ELEM(s) ((void *) (s + 1))

#define SLAW_NUMERIC_ARRAY_SHIFTY 62
#define SLAW_IS_NUMERIC_ILK(s, ilk, array)                                     \
  ((SLAW_ILK (s) & SLAW_NUMERIC_PERSONALITY_BITS)                              \
   == ((((unt64) (array)) << SLAW_NUMERIC_ARRAY_SHIFTY) | SLAW_##ilk           \
       | SLAW_NUMERIFY (sizeof (ilk))))

#define SLAW_NUMERIC_FLOAT_FLAG OB_CONST_U64 (0x2000000000000000)
#define SLAW_NUMERIC_COMPLEX_FLAG OB_CONST_U64 (0x0200000000000000)
#define SLAW_NUMERIC_UNSIGNED_FLAG OB_CONST_U64 (0x1000000000000000)

#define SLAW_NUMERIC_MVEC_FLAG OB_CONST_U64 (0x0100000000000000)

#define SLAW_NUMERIC_VEC_SHIFTY 54

#define SLAW_NUMERIC_VEC2_BITS (OB_CONST_U64 (1) << SLAW_NUMERIC_VEC_SHIFTY)
#define SLAW_NUMERIC_VEC3_BITS (OB_CONST_U64 (2) << SLAW_NUMERIC_VEC_SHIFTY)
#define SLAW_NUMERIC_VEC4_BITS (OB_CONST_U64 (3) << SLAW_NUMERIC_VEC_SHIFTY)

#define SLAW_NUMERIC_MVEC2_BITS (OB_CONST_U64 (4) << SLAW_NUMERIC_VEC_SHIFTY)
#define SLAW_NUMERIC_MVEC3_BITS (OB_CONST_U64 (5) << SLAW_NUMERIC_VEC_SHIFTY)
#define SLAW_NUMERIC_MVEC4_BITS (OB_CONST_U64 (6) << SLAW_NUMERIC_VEC_SHIFTY)
#define SLAW_NUMERIC_MVEC5_BITS (OB_CONST_U64 (7) << SLAW_NUMERIC_VEC_SHIFTY)

#define SLAW_N_ARRAY_NTH_ELEM(s, N)                                            \
  ((void *) (((unt8 *) SLAW_N_ARRAY_FIRST_ELEM (s))                            \
             + (N) *SLAW_NUMERIC_UNIT_BSIZE (s)))

// The "special" bytes are "len" contiguous bytes in the
// least-significant part of an oct.  For little endian
// machines, that will be the first "len" bytes of the
// oct, while for big endian machines, it will be the last
// "len" bytes of oct.  Either way, this macro returns
// an unt8 pointer to the first of the special bytes.
#ifdef __BIG_ENDIAN__
#define SLAW_SPECIAL_BYTES(s, len) ((8 - (len)) + (unt8 *) (s))
#else
#define SLAW_SPECIAL_BYTES(s, len) ((unt8 *) (s))
#endif

// These apply to the *second* oct of the protein, not the ilk
#define SLAW_PROTEIN_NONSTD_FLAG OB_CONST_U64 (0x8000000000000000)
#define SLAW_PROTEIN_DESCRIPS_FLAG OB_CONST_U64 (0x4000000000000000)
#define SLAW_PROTEIN_INGESTS_FLAG OB_CONST_U64 (0x2000000000000000)
/// This flag is reserved for some future use; ignored now (and set to 0)
#define SLAW_PROTEIN_FUTURE_FLAG OB_CONST_U64 (0x1000000000000000)
#define SLAW_PROTEIN_VERY_RUDE_FLAG OB_CONST_U64 (0x0800000000000000)
#define SLAW_PROTEIN_RUDE_PRESENT_MASK OB_CONST_U64 (0x0f00000000000000)
#define SLAW_PROTEIN_WEE_RUDE_MASK OB_CONST_U64 (0x0700000000000000)
#define SLAW_PROTEIN_WEE_RUDE_SHIFTY 56
#define SLAW_PROTEIN_VERY_RUDE_MASK OB_CONST_U64 (0x07ffffffffffffff)

#define SLAW_PROTEIN_OCT2(s) ((s)[1].o)

#define PROTEIN_IS_NONSTANDARD(p)                                              \
  ((SLAW_PROTEIN_OCT2 (p) & SLAW_PROTEIN_NONSTD_FLAG) != 0)
#define PROTEIN_HAS_DESCRIPS(p)                                                \
  ((SLAW_PROTEIN_OCT2 (p) & SLAW_PROTEIN_DESCRIPS_FLAG) != 0)
#define PROTEIN_HAS_INGESTS(p)                                                 \
  ((SLAW_PROTEIN_OCT2 (p) & SLAW_PROTEIN_INGESTS_FLAG) != 0)
#define PROTEIN_HAS_RUDE(p)                                                    \
  ((SLAW_PROTEIN_OCT2 (p) & SLAW_PROTEIN_RUDE_PRESENT_MASK) != 0)
#define PROTEIN_IS_VERY_RUDE(p)                                                \
  ((SLAW_PROTEIN_OCT2 (p) & SLAW_PROTEIN_VERY_RUDE_FLAG) != 0)

#endif /* SLAW_VISCERA_PRIVATE_BOILINGS */
