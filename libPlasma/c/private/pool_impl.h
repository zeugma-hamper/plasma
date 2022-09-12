
/* (c)  oblong industries */

#ifndef POOL_IMPL_CROMULATION
#define POOL_IMPL_CROMULATION

#include "libLoam/c/ob-sys.h"
#include "libPlasma/c/private/pool-portable.h"

#include <limits.h>

#include "sem_ops.h"
#include "fifo_ops.h"
#include "pool_net.h"

#include "libPlasma/c/pool-time.h"

#ifdef __cplusplus
extern "C" {
#endif

// For internal libPlasma use only!
//
// All pool hose internals are private to the pools implementation.
// If users want to look at them, they have to use a defined interface
// exported by libPlasma.

// POOL_SEM_SET_COUNT must be defined here so that we have
// enough storage in the pool hose structure to hold all
// the semaphore ids for the hose
#define POOL_SEM_DEPOSIT_LOCK_IDX 0
#define POOL_SEM_NOTIFICATION_LOCK_IDX 1
#define POOL_SEM_SET_COUNT 2

// --- internal ob_retorts (should not propagate outside libPlasma) ---

// An error code for being woken in such a way that invalidates the current
// connection to a pool.
#define POOL_AWAIT_WOKEN_DIRTY -(OB_RETORTS_PLASMA_POOLS + 2000)

// An error code indicating that the (mmap) pool was resized while we
// were reading it and we should retry the operation.  This shouldn't
// propagate outside the mmap pool code.
#define POOL_SIZE_CHANGED -(OB_RETORTS_PLASMA_POOLS + 2010)

/**
 * A success code used by pool_config_lock() to indicate that no lock
 * was acquired because there is no pools dir, but that's okay.
 */
#define POOL_LOCK_NO_DIR (OB_RETORTS_PLASMA_POOLS + 2500)

/**
 * A success code returned by pool_mmap_withdraw() to tell
 * pool_withdraw() to try to delete the pool.
 */
#define POOL_DELETE_ME (OB_RETORTS_PLASMA_POOLS + 2510)

/**
 * A success code returned by ob_tls_server_available() to indicate
 * that certificates were not found, so only anonymous ciphersuites
 * will be available.
 */
#define POOL_ANONYMOUS_ONLY (OB_RETORTS_PLASMA_POOLS + 2520)

/**
 * The core pool code handles all the work that is common to all pool
 * types.  The operations that must be provided by each pool type
 * implementation are defined below.  They must be filled in by the
 * pool load methods function.
 *
 * All pool configuration changes (create, dispose, participate, and
 * withdraw) are protected by the configuration lock on the system
 * hosting the pool.  The configuration lock is a mutual exclusion lock
 * which must be held during all reads or changes to pool configuration.
 * It covers all pools using the same OB_POOLS_DIR.  As a result, pool
 * implementations can be guaranteed that no other thread is modifying
 * the pool configuration during these functions - that is, create(),
 * etc. are guaranteed to be single-threaded.  When calling these
 * methods for pools which are not hosted on the local machine (network
 * pools), the configuration lock is NOT held (since operations will be
 * single-threaded on the system hosting the pool).
 */

/**
 * create() sets up all the type-specific persistent state necessary for
 * the pool.  Any parameters needed by the create function are passed
 * through in the options protein.  The generic code will store the
 * pool name and pool type in the main config file.
 *
 * For local pools, the configuration lock is held and the pool is
 * guaranteed not to exist.
 */

typedef ob_retort (*create_func_t) (pool_hose ph, const char *type,
                                    bprotein create_options);

/**
 * participate() creates a connection to the pool, after which we can
 * read, deposit, etc. to the pool.
 *
 * For local pools, the configuration lock is held and the pool is
 * guaranteed to exist.
 */

typedef ob_retort (*participate_func_t) (pool_hose ph);

/**
 * deposit() adds a protein to the pool, at the index following the
 * last protein in the pool (NOT the current index in the pool_hose).
 * This function must notify any awaiters on this pool that a protein
 * has been deposited.  Currently, awaiters may only wait on the next
 * protein to be deposited, so all awaiters will be notified on each
 * deposit.  It must tear down the notification state for each
 * awaiter.
 */

typedef ob_retort (*deposit_func_t) (pool_hose ph, bprotein p, int64 *idx,
                                     pool_timestamp *ret_ts);

/**
 * nth() returns the protein at the specified index if it exists,
 * otherwise it returns an appropriate error.
 */

typedef ob_retort (*nth_func_t) (pool_hose ph, int64 idx, protein *return_prot,
                                 pool_timestamp *ret_ts);

/**
 * newest_index() returns the index of the most recently added
 * protein to the pool.
 */

typedef ob_retort (*newest_index_func_t) (pool_hose ph, int64 *newest_index);

/**
 * oldest_index() returns the index of the oldest protein still
 * available in the pool.
 */

typedef ob_retort (*oldest_index_func_t) (pool_hose ph, int64 *oldest_index);

/**
 * Lookup a pool index according to a given timestamp.
 * More concretely, this function returns the index
 * of the protein closest to the provided timestamp,
 * according to the criterium specified by \a bound.
 */
typedef ob_retort (*index_lookup_func_t) (pool_hose ph, int64 *index,
                                          pool_timestamp ts,
                                          time_comparison bound, bool relative);

/**
 * withdraw() tears down the state set up by participate().
 *
 * The configuration lock is not held.
 */

typedef ob_retort (*withdraw_func_t) (pool_hose ph);

/**
 * dispose() tears down the state set up by create().
 *
 * On entry, the configuration lock is held for local pools.
 */

typedef ob_retort (*dispose_func_t) (pool_hose ph);

/**
 * await_next_single() does not return until either a protein with
 * index > ph->index is deposited or the timeout expires.  If a
 * protein was deposited, it returns OB_OK, fills the protein
 * with the next available protein following ph->index, fills in the
 * protein's timestamp and index if the pointers are non-NULL, and
 * updates ph->index to the index following the returned protein.
 */

typedef ob_retort (*await_next_single_func_t) (pool_hose ph,
                                               pool_timestamp timeout,
                                               protein *ret_prot,
                                               pool_timestamp *ret_ts,
                                               int64 *ret_index);

/**
 * await_next_multi() returns the next available protein if it exists.
 * Otherwise, it sets up the state needed by multi-pool await (that
 * is, creates a socket (stored in ph->notify_fd) that will be written
 * to if a protein is deposited) and returns.  When it returns, one of
 * the following must be true: (a) it returned a protein, or (b) if any
 * protein with index >= ph->index is deposited, something will be
 * written to the socket.  This function must cope with the case of a
 * protein being deposited at any time between function entry and
 * exit.  It updates ph->index to the index following the returned
 * protein.
 */

typedef ob_retort (*multi_add_awaiter_func_t) (pool_hose ph, protein *ret_prot,
                                               pool_timestamp *ret_ts,
                                               int64 *ret_index);

/**
 * cancel_await_multi() tears down any state set up by
 * await_next_multi(), preparatory to a pool_withdraw().  It does not
 * need to be used after a protein deposit as the deposit() function
 * must tear down the await state itself (usually during notification
 * of awaiters).
 */

typedef void (*multi_remove_awaiter_func_t) (pool_hose ph);

/**
 * participate_creatingly() is optional - if the function pointer is
 * not set, the core pools code will implement it using the create()
 * and participate() functions.  Network pools must define this
 * function because otherwise we cannot atomically create and
 * participate in the pool.  On entry, the configuration lock is NOT
 * held.
 */

typedef ob_retort (*participate_creatingly_func_t) (pool_hose ph,
                                                    const char *type,
                                                    bprotein options);

/**
 * next() is an atomic version of pool_next() (usually implemented
 * via nth() and oldest_idx()) for use with remote pools.
 * XXX: there doesn't seem to be anything atomic about it to me,
 * since there's no locking that keeps something from coming between
 * the nth and oldest.  But, it is good for efficiency since it means
 * one round trip instead of two.  And we use this same typedef for
 * prev(), too.
 */

typedef ob_retort (*next_func_t) (pool_hose ph, protein *return_prot,
                                  pool_timestamp *ret_ts, int64 *ret_index);

/**
 * probe_frwd() is an atomic version of pool_probe_frwd() for use
 * with remote pools, which handles the searching on the server side.
 * And ditto for probe_back() in the opposite direction.  This typedef
 * is used for both of those functions.
 */

typedef ob_retort (*probe_func_t) (pool_hose ph, bslaw search,
                                   protein *return_prot, pool_timestamp *ret_ts,
                                   int64 *ret_index);

/**
 * same as above, but also includes a timeout, since it is for
 * implementing pool_await_probe_frwd()
 */

typedef ob_retort (*await_probe_func_t) (pool_hose ph, bslaw search,
                                         pool_timestamp timeout,
                                         protein *return_prot,
                                         pool_timestamp *ret_ts,
                                         int64 *ret_index);

/**
 * Returns a protein with information about a pool.
 * Should always include an ingest "type", which is a string naming the
 * pool type, and "terminal", which is a boolean which is true if this is
 * a terminal pool type like "mmap", or false if this is a transport pool
 * type like "tcp".
 * For mmap pools, should include an ingest "size", which is an integer
 * giving the size of the pool in bytes.
 * For tcp pools, should include an ingest "host", which is a string
 * naming the host the pool is on, and "port" which is an integer
 * giving the port.
 * For other pool types, ingests with other relevant info can be included.
 * If "hops" is 0, means return information about this pool hose.
 * If "hops" is 1, means return information about the pool beyond this
 * hose (assuming this hose is a nonterminal type like TCP).  And higher
 * values of "hops" mean go further down the line, if multiple nonterminal
 * types are chained together.  If "hops" is -1, means return information
 * about the terminal pool, no matter how far it is.
 */
typedef ob_retort (*pool_info_func_t) (pool_hose ph, int64 hops,
                                       protein *return_prot);

/**
 * Returns the names of all the pool on the server, as a slaw list of strings.
 * ph->name is like a pool URI, but without the actual pool name.
 * For example, "tcp://eggplant.local:8421/"
 */
typedef ob_retort (*pool_list_func_t) (pool_hose ph, slaw *ret_list);

/**
 * Sets the name of the pool hose.  This is used for logging and
 * debugging.
 */
typedef ob_retort (*hose_name_func_t) (pool_hose ph, const char *name);

/**
 * Function pointer for implementing the pool_fetch() function.
 * Although pool_fetch() itself does not return a retort, this
 * function pointer does, so it can return POOL_UNSUPPORTED_OPERATION
 * if not supported.  But otherwise, it should return OB_OK and use
 * the individual \a ops entries (and/or \a oldest_idx_out and
 * \a newest_idx_out) to communicate the error.
 */
typedef ob_retort (*fetch_func_t) (pool_hose ph, pool_fetch_op *ops, int64 nops,
                                   int64 *oldest_idx_out, int64 *newest_idx_out,
                                   bool clamp);

/**
 * Function pointer for implementing the pool_rename() function.
 */
typedef ob_retort (*rename_func_t) (pool_hose ph, const char *old_name,
                                    const char *new_name);

typedef ob_retort (*advance_oldest_func_t) (pool_hose ph, int64 idx_in);

typedef ob_retort (*sleep_func_t) (pool_hose ph);

typedef ob_retort (*change_options_func_t) (pool_hose ph, bslaw options);

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
  int mode;
  int uid;
  int gid;
} pool_perms;

/* XXX: Is this the correct capitalization?  (bug 2484 comment 10) */
typedef enum {
  Hose_Unused = 252068753,
  Hose_Used = 317901396,
  Hose_Needs_Reconnection = 875387785 /* due to fork() */
} Hose_Status;

typedef struct
{
  pool_hose next;
  int32 /* Hose_Status */ status;
} pool_management_info;

struct pool_hose_struct
{
  pool_management_info mgmt;

  /**
   * Name of the pool.
   */
  char *name;
  /**
   * Name of this particular hose.
   */
  char *hose_name;
  /**
   * Indicate which pool implementation methods to use; e.g., mmap or
   * tcp.
   */
  char *method;
  /**
   * Is this pool remote (hosted on another system) or local
   * (configuration located on this machine)?  Affects configuration
   * locking and multi-pool await notification.
   */
  bool remote;
  /**
   * Index of the next protein to read in this pool.  It can never be
   * negative, but the rest of the API uses signed pool indexes so we
   * match it.  (XXX: actually it can be negative; see bug 313)
   */
  int64 index;

#ifdef _MSC_VER
  /**
   * Windows uses mutexes for lock operations, one mutex per type of
   * lock required by the pool hose (see #defines at the top of this file)
   */
  HANDLE mutex_handles[POOL_SEM_SET_COUNT];

  //this is unused on windows and seems to not be used on linux/osx either??
  int32 sem_key;
#else
  /**
   * Key and id for semaphore-based lock operations.
   */
  key_t sem_key;
  int sem_id;
  /**
   * File descriptors when using flock() locking.  Technically, we either
   * use these, or use the sem_key/sem_id above, but not both.  So, in
   * some alternate universe, we might put them in a union.  In this
   * universe, we chose not to.
   */
  int flock_fds[POOL_SEM_SET_COUNT];
#endif

  wakeup_stuff w;

  /**
   * State used for multi-pool await.
   *
   * Path for the directory containing notification fifos for this
   * pool.  It is possible this can be sped up in the no-waiters case
   * by caching the directory mtime, but it goes pretty fast right
   * now.
   */
  char notify_dir_path[PATH_MAX];
  /**
   * Pathname of our notification fifo.  When we begin await, we
   * create a fifo with this name and wait for someone to write to
   * it.
   */
  char fifo_path[PATH_MAX];

  /**
   * Windows description:
   * this is the handle of the event we should wait on if we want
   * notification for a deposit in this pool.  For local pools, this
   * is the fifo specified by fifo_path. Unlike linux network pools,
   * it is NOT possible on windows to wait on the network socket via
   * notify_fd. This is something that will be addressed in the future.
   *
   * UNIX description:
   * fd of the socket we should wait on if we want notification for a
   * deposit in this pool.  For local pools, this is the fifo
   * specified by fifo_path.  For network pools, this is the network
   * socket.  In particular, the funny (and seemingly undesirable, to
   * me) consequence of this is that for local pools, the file descriptor
   * is "owned" by the type-independent pool code, and needs to be closed
   * by pool_free_pool_hose() if it is not -1.  But for network pools,
   * this file descriptor is either -1 or it is the same as ph->net->connfd,
   * which is "owned" by the TCP pool code, and gets closed in
   * pool_tcp_withdraw().  Therefore, in the network case,
   * pool_free_pool_hose() must not close it, because it has already been
   * closed.  Lovely, huh?
   */
  ob_handle_t notify_handle;

  /**
   * For some pool types, waking them up can leave them in an inconsistent
   * state. If that is the case, this flag will be set, and the next time pool
   * functions are called, the implementation should reset its state and clear
   * this flag.
   */
  bool dirty;

  /**
   * This is permission information for the pool.  We need to keep
   * this around, because we're always creating new notification fifos.
   */
  pool_perms perms;

  /**
   * This flag indicates whether the hose is currently a member
   * of a gang.
   * A pool hose may only be a member of one gang at once, otherwise
   * it may be awaited more than once and that will end in disaster.
   */
  bool is_gang_member;

  /**
   * How the pool directory and files are organized for local pools.
   */
  unt8 pool_directory_version;

  /**
   * True if this pool has been found to be on a different filesystem
   * than the temporary directory.  (Therefore, no moving files between
   * them.)  Starts out false and setting it to true is just an optimization.
   */
  bool different_filesystem_than_tmp;

  /**
   * Generic network pool data.
   */
  pool_net_data *net;

  /**
   * Pointer to pool_mmap_data for mmap pools, or parsed_pseudo_uri
   * for TCP pools.
   */
  void *ext;

  /**
   * Optionally, a context with more information
   */
  pool_context ctx;

  /**
   * Locking functions for local pools.
   */
  const pool_lock_ops *lock_ops;

  /**
   * Type-specific functions (see comments in function typedefs).
   */
  create_func_t create;
  participate_func_t participate;
  deposit_func_t deposit;
  nth_func_t nth_protein;
  newest_index_func_t newest_index;
  oldest_index_func_t oldest_index;
  index_lookup_func_t index_lookup;
  withdraw_func_t withdraw;
  dispose_func_t dispose;
  await_next_single_func_t await_next_single;
  multi_add_awaiter_func_t multi_add_awaiter;
  multi_remove_awaiter_func_t multi_remove_awaiter;
  /**
   * Functions below this line are optional, set to NULL if unneeded
   */
  participate_creatingly_func_t participate_creatingly;
  next_func_t next;
  probe_func_t probe_frwd;
  pool_info_func_t info;
  pool_list_func_t list_pools;
  probe_func_t probe_back;
  next_func_t prev;
  next_func_t opportunistic_next;
  hose_name_func_t set_hose_name;
  await_probe_func_t await_probe_frwd;
  fetch_func_t fetch;
  rename_func_t rename;
  advance_oldest_func_t advance_oldest;
  sleep_func_t psleep;
  dispose_func_t get_semkey;
  change_options_func_t change_options;
  withdraw_func_t hiatus;
};

struct pool_context_struct
{
  unt32 magic; /* double-check to make sure this is a context, not freed mem */
  slaw connection_options;
};

#define POOL_CTX_MAGIC 519965495
#define POOL_CTX_FREED 229425281

/**
 * Check the magic number of a pool context, and abort with an error
 * message (including \a where) if it is not valid.
 */
OB_HIDDEN void pool_validate_context (pool_context ctx, const char *where);

/**
 * Append a string to an existing pathname.
 *
 * Example:
 *
 *    char path[PATH_MAX];
 *    strcpy (path, "/usr");
 *    pool_add_path_element (path, "local")
 *
 * Now "path" contains "/usr/local".
 */

OB_HIDDEN ob_retort pool_add_path_element (char *path, const char *subdir);

/**
 * Create the pathname for a configuration file.
 */

OB_HIDDEN ob_retort pool_config_file_path (char *path, const char *config_name,
                                           const char *poolName);

/**
 * Create the pathname of the directory that contains all the
 * configuration files for this particular pool.
 */

OB_HIDDEN ob_retort pool_build_pool_dir_path (char *dir_path,
                                              const char *poolName);

/**
 * Configuration related functions.
 *
 * Configuration files contain proteins which store configuration
 * data in their ingests.  Each pool can have multiple configuration
 * files, one per unique "config_name" string.  The main pools code
 * uses a config name of NULL, and everything else goes into config
 * files with other config names.  For example, the "mmap" pool type
 * stores its configuration data under the config name "mmap".  This
 * is useful because then different parts of the code can store
 * configuration data without worrying about what other parts of the
 * code stored.
 *
 * The location of the pool configuration files is controlled by the
 * OB_POOLS_DIR environment variable.  This specifies the path of the top
 * level directory of the various pool-related files.  The files
 * associated with a particular pool are located in a directory named:
 * @code
 * ${OB_POOLS_DIR}/<pool_name>
 * @endcode
 * If OB_POOLS_DIR is unset, it defaults to "/var/ob/pools".
 *
 * Several of the helper functions that build pathnames take a "char
 * *path" argument.  This should point to a buffer of at least
 * PATH_MAX bytes (the maximum length of a pathname on a UNIX file
 * system).
 */

/**
 * Read a configuration protein from its config file.
 */

ob_retort pool_read_config_file (const char *config_name, const char *poolName,
                                 protein *conf_p);

/**
 * Write a config protein to a file.  Permissions can be set with
 * mode, uid, and gid, or can be -1 to leave unspecified.
 */
ob_retort pool_write_config_file (const char *config_name, const char *poolName,
                                  bprotein conf, int mode, int uid, int gid);

/**
 * Remove a configuration file.
 */
OB_HIDDEN ob_retort pool_remove_config_file (const char *config_name,
                                             const char *poolName);

/**
 * Save the generic pool configuration.  This is where we save all
 * the information needed to open the pool.  Used by pool_sem.c.
 */
OB_HIDDEN ob_retort pool_save_default_config (pool_hose ph);

/**
 * Is this pool hosted on the local machine?
 */

OB_HIDDEN bool pool_is_local (pool_hose ph);

/**
 * Is this pool hosted on a remote machine?
 */

OB_HIDDEN bool pool_is_remote (pool_hose ph);

/**
 * Assuming options is either a protein or a map, look for ingests
 * "mode", "owner", or "group", and set *mode, *uid, and *gid to the
 * values.  Anything unspecified is set to -1.  If mode is a string,
 * it is parsed as an octal number.  If owner or group is a string,
 * the user or group name is looked up.
 */
OB_HIDDEN ob_retort pool_parse_permissions (bslaw options, int *mode, int *uid,
                                            int *gid);

/**
 * "and" mode with mask, unless it is unspecified (-1)
 */
static inline int pool_combine_modes (int mode, int mask)
{
  return (mode < 0 ? -1 : (mode & mask));
}

OB_HIDDEN pool_timestamp private_incremental_timeout (pool_timestamp timeout,
                                                      pool_timestamp *target);

OB_HIDDEN void private_maybe_fill_index (
  pool_hose ph, ob_retort (*fn) (pool_hose ph, int64 *index_out), int64 *out);

/**
 * Entry points for specific pool implementations, since we no longer
 * use dlopen().  (bug 1005)
 */
OB_HIDDEN ob_retort pool_mmap_load_methods (pool_hose ph);
OB_HIDDEN ob_retort pool_tcp_load_methods (pool_hose ph);

// For more information on versioning, see the text file "versions.txt"
// in the libPlasma/c directory of yovo.

#define POOL_DIRECTORY_VERSION_CONFIG_IN_FILE 4
#define POOL_DIRECTORY_VERSION_CONFIG_IN_MMAP 5
#define POOL_DIRECTORY_VERSION_SINGLE_FILE 6

// round pools to a multiple of this size
#define POOL_SIZE_GRANULARITY 4096

/**
 * Given "foo/bar/baz" and "crud", produces "foo/bar/crud/baz".
 * May reallocate *pathp, hence the indirection.
 *
 * Used from pool-flock-ops.c, as well as pool_multi.c
 */
OB_HIDDEN ob_retort ob_insert_penultimate_directory (char **pathp,
                                                     const char *insert);

#ifdef __cplusplus
}
#endif

#endif  // POOL_IMPL_CROMULATION
