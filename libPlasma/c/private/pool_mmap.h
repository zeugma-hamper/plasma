
/* (c)  oblong industries */

#ifndef POOL_MMAP_QUEST
#define POOL_MMAP_QUEST

#include "libLoam/c/ob-attrs.h"
#include "libLoam/c/ob-atomic.h"
#include "libPlasma/c/pool.h"
#include "libPlasma/c/private/pool-toc.h"
#include "libPlasma/c/private/pool_impl.h"

#ifdef __cplusplus
extern "C" {
#endif

OB_HIDDEN ob_retort pool_mmap_newest_index (pool_hose ph, int64 *idx);
OB_HIDDEN ob_retort pool_mmap_oldest_index (pool_hose ph, int64 *idx);

OB_HIDDEN ob_retort pool_mmap_deposit (pool_hose ph, bprotein p, int64 *idx,
                                       pool_timestamp *ret_ts);
OB_HIDDEN ob_retort pool_mmap_nth_protein (pool_hose ph, int64 idx,
                                           protein *return_prot,
                                           pool_timestamp *ret_ts);

OB_HIDDEN ob_retort pool_mmap_create (pool_hose ph, const char *type,
                                      bprotein create_options);
OB_HIDDEN ob_retort pool_mmap_dispose (pool_hose ph);

OB_HIDDEN ob_retort pool_mmap_participate (pool_hose ph);
OB_HIDDEN ob_retort pool_mmap_withdraw (pool_hose ph);

/**
 * We're using a style of error handling here similar to what ICU uses:
 * http://userguide.icu-project.org/dev/codingguidelines#TOC-Details-about-ICU-Error-Codes
 *
 * (Basically, each function takes a pointer to a retort, and if the
 * retort already indicates failure, the function does nothing.  If the
 * function fails, it sets the retort.)
 *
 * Compared to the "retort is return value" convention used elsewhere
 * in yovo, this convention makes it a bit easier to handle errors in
 * deeply nested call stacks, especially when a lot of the functions
 * already have their own return values.
 *
 * Anyway, this function encapsulates the check for "already an error"
 * that we want to put at the top of each function that uses this
 * convention.
 */
static inline bool already_failed (const ob_retort *errp)
{
  return (*errp < OB_OK);
}

/**
 * The pool header is kept at the beginning of the pool backing file,
 * and keeps the location of the oldest and newest entry in the pool.
 */

typedef struct
{
  // These are offsets to the correct part of memory, relative to
  // d->mem, and continuously increasing, so must be interpreted
  // modulo d->size.
  int64 oldest_entry;
  int64 newest_entry;
} pool_mmap_oldnew;

typedef struct
{
  unt64 sig;
  unt64 len;
} pool_chunk_header;

#define POOL_CHUNK_SIG(a, b, c, d)                                             \
  (OB_CONST_U64 (0x1badd00d00000000)                                           \
   | (OB_CONST_U64 (0xffffffff)                                                \
      & (((a) << 24) | ((b) << 16) | ((c) << 8) | (d))))

typedef struct
{
  pool_chunk_header hdr;
  int64 mmap_version;  // if we're using this header, it's at least 1
  int64 file_size;     // in bytes
  int64 header_size;   // in bytes
  int64 sem_key;       /* unused if POOL_FLAG_FLOCK is set */
  /**
   * In order to make backwards compatibility work better, there are
   * two possible things we can do if we see a flag we don't support:
   * a) ignore it and continue
   * b) error
   * Bits 0-31 will cause an error if not recognized, and bits 32-63
   * will be ignored if not recognized.  That way, we can pick the
   * backwards-compatibility behavior we want when we allocate
   * future flags.
   */
  int64 flags;
  /**
   * This is *only* needed in the case where the pool is empty.
   * In all other cases, we can determine the next index by adding one
   * to the index of the newest protein.
   */
  int64 next_index;
} pool_chunk_conf;

#define POOL_CHUNK_CONF POOL_CHUNK_SIG ('c', 'o', 'n', 'f')

#define POOL_FLAG_STOP_WHEN_FULL (OB_CONST_U64 (1) << 0)
#define POOL_FLAG_FROZEN (OB_CONST_U64 (1) << 1)
#define POOL_FLAG_AUTO_DISPOSE (OB_CONST_U64 (1) << 2)
#define POOL_FLAG_CHECKSUM (OB_CONST_U64 (1) << 3)
#define POOL_FLAG_FLOCK (OB_CONST_U64 (1) << 4)
#define POOL_FLAG_SYNC (OB_CONST_U64 (1) << 32)

#ifdef __APPLE__ /* see bug 3770 for explanation */
#define POOL_DEFAULT_FLAGS POOL_FLAG_FLOCK
#else
#define POOL_DEFAULT_FLAGS 0
#endif

/**
 * The reason we need to store the permissions is because
 * pool_fifo_multi_add_awaiter() needs to create new fifos, and
 * it needs to know what permissions to create them with.
 * This assumes that if we move this pool to another machine, the
 * numeric users and groups are still valid.
 * -1 in any of the three fields means "unspecified".
 */
typedef struct
{
  pool_chunk_header hdr;
  int64 mode;
  int64 uid;
  int64 gid;
} pool_chunk_perm;

#define POOL_CHUNK_PERM POOL_CHUNK_SIG ('p', 'e', 'r', 'm')

typedef struct
{
  pool_chunk_header hdr;
  pool_mmap_oldnew ptrs;
} pool_chunk_ptrs;

#define POOL_CHUNK_PTRS POOL_CHUNK_SIG ('p', 't', 'r', 's')

/**
 * The "table of contents" was originally known as the "index",
 * which is why its signature is "indx", in order to maintain
 * backward compatibility.
 */
#define POOL_CHUNK_TOC POOL_CHUNK_SIG ('i', 'n', 'd', 'x')

typedef struct
{
  byte magic[8];
  pool_chunk_conf conf;
  pool_chunk_ptrs ptrs;
  pool_chunk_perm perm;
} default_v1_header;

typedef struct pool_mmap_data pool_mmap_data;

typedef struct
{
  unt64 (*size_of_header) (unt64 index_capacity);
  void (*initialize_header) (unt8 slaw_vers, byte *mem, unt64 idx_cap);
  ob_retort (*read_header) (pool_mmap_data *d);
  ob_retort (*bootstrap) (pool_mmap_data *d);
  ob_retort (*write_config_file) (const char *name, pool_perms perms,
                                  unt64 file_size, unt64 header_size,
                                  unt64 idx_cap);
  ob_retort (*delete_config_file) (const char *name);
} mmap_version_funcs;

OB_HIDDEN OB_CONST mmap_version_funcs
pool_mmap_get_version_funcs (unt8 mmap_vers);

/**
 * Data associated with each pool hose.
 */

struct pool_mmap_data
{
/**
   * File handle for the backing file.
   */
#ifdef _MSC_VER
  HANDLE file;
  HANDLE file_mapping_object;
#else
  FILE *file;
#endif

  /**
   * Address in memory that the file has been mmap'd to.
   */
  byte *mem;
  /**
   * Total backing file size.
   */
  unt64 mapped_size;
  /**
   * We cache the last entry we read in order to speed up sequential
   * reading (as in pool_next()).  Otherwise we have to navigate back
   * to some fixed point in the pool (first_entry or oldest_entry) and
   * jump forward entry-by-entry to find anything other than the most
   * recent entry.
   */
  int64 cached_index;
  unt64 cached_entry;
  /**
   * Location in memory of the pool header - currently identical to
   * the mem field in this same struct.
   */
  pool_mmap_oldnew *oldnew;
  /**
   * Location of the pool_toc, when present, or NULL if this
   * pool has no table of contents.
   * When present, the pool table of contents is located right after the
   * magic constant, and its size depends on the pool's configuration.
   */
  pool_toc_t *ptoc;
  /**
   * Auxiliary counters to estimate index performance
   */
  unt64 index_success_count;
  unt64 index_failure_count;
  double index_failure_rate;

  /**
   * A pointer back to the hose that contains us, to make the
   * pname() and hname() functions easier to implement.  (Since
   * pool_mmap_data gets passed around everywhere, but the hose
   * doesn't.)
   */
  pool_hose ph;

  /**
   * Pointer to function that gets the length of a protein,
   * for whatever slaw version is used in this pool (since
   * that might be an old version).
   */
  int64 (*vprotein_len) (bprotein p);

  /**
   * Version number for the slaw encoding
   */
  byte slaw_version;

  /** Points to permissions, or NULL if none. */
  pool_chunk_perm *perm_chunk;

  /** Points to configuration; can't be NULL. */
  pool_chunk_conf *conf_chunk;

  /**
   * For old, non-chunked pools, conf_chunk points here instead of
   * into the backing file.
   */
  pool_chunk_conf legacy_conf;
};

/**
 * Minimum pool size.  It must be large enough to store the pool
 * header; it would be nice if it were also large enough to store a
 * protein.
 */

#define POOL_MMAP_MIN_SIZE (1000)

/**
 * Maximum pool size.  This notion was introduced in commit
 * aab222dcce347e763374748c94d5f270a65dabae, and was originally
 * 32 gigabytes, although no one seems to remember why.
 *
 * 32 gigabytes seems like too small a limit for 64-bit machines,
 * while obviously being way too large a limit for 32 bit machines.
 *
 * Therefore, the new limit is 2 gigabytes for 32-bit machines,
 * since it's unlikely that more than 2 gigabytes of contiguous
 * space will be available in the 4 gigabyte address space:
 *
 * http://msdn.microsoft.com/en-us/library/bb613473(VS.85).aspx
 *
 * (Although this documentation is for Windows, it's unlikely
 * more than 2 gigabytes of contiguous address space will be
 * available on 32-bit Linux or Mac, either.)
 *
 * For 64-bit machines, the limit is now 8 terabytes.  This is
 * slightly more than just something I pulled out of /dev/random,
 * in that it's the maximum virtual address space supported by
 * 64-bit Windows:
 *
 * http://msdn.microsoft.com/en-us/library/aa366778(VS.85).aspx
 *
 * Of course, we don't currently support 64-bit Windows.  But
 * it also seems like a reasonable sanity check for Linux, that
 * we're unlikely to run into at the present time (i. e. 2010).
 * We can easily increase this limit when the time comes that
 * I can go to Fry's and buy a drive larger than 8 terabytes.
 */
#define POOL_MMAP_MAX_SIZE                                                     \
  ((sizeof (void *) < 8) ? (2 * GIGABYTE) : (8 * TERABYTE))

/**
 * Helper function to pull out our private data.
 */

static inline pool_mmap_data *pool_mmap_get_data (pool_hose ph)
{
  return (pool_mmap_data *) ph->ext;
}

/**
 * Returns pool name
 */
static inline const char *pname (const pool_mmap_data *d)
{
  return d->ph->name;
}

/**
 * Returns hose name
 */
static inline const char *hname (const pool_mmap_data *d)
{
  return d->ph->hose_name;
}

// These remain constant for the life of the pool, so we don't need to
// use atomic ops.
static inline byte get_mmap_version (const pool_mmap_data *d)
{
  return d->conf_chunk->mmap_version;
}

static inline byte get_slaw_version (const pool_mmap_data *d)
{
  return d->slaw_version;
}

static inline unt64 get_header_size (const pool_mmap_data *d)
{
  return d->conf_chunk->header_size;
}

// These can change while a pool is in use, so get them atomically.
static inline unt64 get_file_size (const pool_mmap_data *d)
{
  return (unt64) ob_atomic_int64_ref (&d->conf_chunk->file_size);
}

static inline unt64 get_flags (const pool_mmap_data *d)
{
  return (unt64) ob_atomic_int64_ref (&d->conf_chunk->flags);
}

// See file libPlasma/c/versions.txt for the dirty details.
OB_HIDDEN OB_CONST unt8
pool_mmap_version_from_directory_version (unt8 pool_directory_version);

typedef ob_retort (*backing_file_func) (const char *path, va_list vargs);

OB_HIDDEN ob_retort pool_mmap_call_with_backing_file (
  const char *poolName, unt8 pool_directory_version, bool locked,
  backing_file_func bff, ...);

OB_HIDDEN ob_retort pool_mmap_load_config (pool_hose ph);

#ifdef __cplusplus
}
#endif

#endif /* POOL_MMAP_QUEST */
