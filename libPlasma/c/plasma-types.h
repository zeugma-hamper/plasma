
/* (c)  oblong industries */

#ifndef PLASMA_TYPES_WITH_ONE_HAND_TIED_BEHIND_ITS_BACK
#define PLASMA_TYPES_WITH_ONE_HAND_TIED_BEHIND_ITS_BACK

/**
 * \defgroup SlawBasicAPI Types, constants and basic slaw handling
 * Basic types and constant codes used by the rest of
 * the API and its clients.
 * \ingroup PlasmaSlawGroup
 */
struct _slaw;

/**
 * slaw is an opaque pointer type. Code holding a value
 * of type slaw is assumed to control its life cycle, including
 * freeing associated resources when no longer needed.
 * \sa bslaw
 * \ingroup SlawBasicAPI
 */
typedef struct _slaw *slaw;

/**
 * A bslaw is an opaque pointer type that represents a slaw
 * whose storage is owned by someone else. Therefore, functions
 * taking a bslaw (rather than slaw) as a parameter won't
 * be able to free it.
 * \sa slaw
 * \ingroup SlawBasicAPI
 */
typedef const struct _slaw *bslaw;

/**
 * A protein is just a slaw with a distinctive type.
 * \ingroup SlawBasicAPI
 */
typedef slaw protein;

/**
 * As with slaw, we mark proteins whose storage we don't own
 * with a different type.
 * \ingroup SlawBasicAPI
 */
typedef bslaw bprotein;

/**
 * \defgroup Slabu Composite slaw builder
 * A slabu allows the incremental creation of a composite
 * slaw. We provide two kinds of composites: lists (\ref SlawList)
 * and maps (\ref SlawMap).
 * lists.
 * \ingroup PlasmaSlawGroup
 */

struct _slaw_bundle;
/**
 * The in-progress list/map build type.
 * \ingroup Slabu
 */
typedef struct _slaw_bundle slabu;

typedef enum { SEARCH_GAP, SEARCH_CONTIG } Protein_Search_Type;

#define OB_KILOBYTE (OB_CONST_U64 (1) << 10)
#define OB_MEGABYTE (OB_CONST_U64 (1) << 20)
#define OB_GIGABYTE (OB_CONST_U64 (1) << 30)
#define OB_TERABYTE (OB_CONST_U64 (1) << 40)

#endif /* PLASMA_TYPES_WITH_ONE_HAND_TIED_BEHIND_ITS_BACK */
