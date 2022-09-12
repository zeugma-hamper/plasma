
/* (c)  oblong industries */

/* The pool's "table of contents" is an optional data structure, which
 * keeps track of the location of proteins in a pool for faster access.
 *
 * It used to be called an "index", but that was confusing, because it
 * conflicted with the use of "index" to mean "sequence number".
 */

#ifndef OBLONG_POOL_TOC_H
#define OBLONG_POOL_TOC_H

#include "libLoam/c/ob-attrs.h"
#include "libPlasma/c/pool-time.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef volatile struct pool_toc_s pool_toc_t;
typedef struct pool_toc_entry
{
  int64 idx;
  unt64 offset;
  pool_timestamp stamp;
} pool_toc_entry;

#define POOL_TOC_UNKNOWN_OFFSET ((unt64) (-1))
#define POOL_TOC_UNKNOWN_TIME ((pool_timestamp) (-1))
#define POOL_TOC_UNKNOWN_IDX ((int64) (-1))

// Note: OB_PLASMA_API is only needed because some of these functions
// are called by TocUnitTest.exe.  They are not part of the API, though.

OB_PLASMA_API extern const pool_toc_entry POOL_TOC_NULL_ENTRY;

#define POOL_TOC_ENTRY_NULL_P(e)                                               \
  ((POOL_TOC_UNKNOWN_IDX == (e).idx)                                           \
   || (POOL_TOC_UNKNOWN_OFFSET == (e).offset)                                  \
   || (POOL_TOC_UNKNOWN_TIME == (e).stamp))

OB_PLASMA_API pool_toc_t *pool_toc_read (const byte *buffer);
OB_PLASMA_API pool_toc_t *pool_toc_init (byte *buffer, unt64 capacity);

OB_PLASMA_API OB_CONST unt64 pool_toc_room (unt64 capacity);

OB_PLASMA_API unt64 pool_toc_capacity (const pool_toc_t *pi);
OB_PLASMA_API unt64 pool_toc_count (const pool_toc_t *pi);
OB_HIDDEN unt64 pool_toc_step (const pool_toc_t *pi);

OB_PLASMA_API
bool pool_toc_find_idx (const pool_toc_t *pi, int64 idx, pool_toc_entry *lower,
                        pool_toc_entry *upper);

OB_PLASMA_API
bool pool_toc_find_timestamp (const pool_toc_t *pi, pool_timestamp ts,
                              pool_toc_entry *lower, pool_toc_entry *upper);

OB_PLASMA_API pool_timestamp pool_toc_max_timestamp (const pool_toc_t *pi);
OB_PLASMA_API pool_timestamp pool_toc_min_timestamp (const pool_toc_t *pi);

// Precondition: idx and ts are greater than the last appended ones.
OB_PLASMA_API
bool pool_toc_append (pool_toc_t *pi, pool_toc_entry entry,
                      unt64 oldest_offset);

#ifdef __cplusplus
}
#endif

#endif  // OBLONG_POOL_TOC_H
