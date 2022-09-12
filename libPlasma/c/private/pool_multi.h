
/* (c)  oblong industries */

#ifndef POOL_MULTI_VERSE
#define POOL_MULTI_VERSE

#include "libLoam/c/ob-sys.h"
#include "libPlasma/c/private/pool-portable.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Multi-pool await support - for internal libPlasma use only.
 */

/**
 * Create per-pool multi-pool await state.  Used during pool creation.
 */

OB_HIDDEN ob_retort pool_init_multi_await (pool_hose ph, int mode, int uid,
                                           int gid);

/**
 * Set up per-connection multi-pool await state for this pool hose.
 * Used during participate.
 */

OB_HIDDEN ob_retort pool_load_multi_await (pool_hose ph, int mode, int uid,
                                           int gid);

/**
 * Destroy the multi-pool await scaffolding for a pool.  Used during
 * dispose.
 */

OB_HIDDEN ob_retort pool_destroy_multi_await (pool_hose ph);

/**
 * Wake all pool users doing a multi-pool await on this pool.
 */

ob_retort pool_multi_wake_awaiters (pool_hose ph) OB_HIDDEN;

/**
 * Construct the pathname for the directory containing notification
 * fifos for this pool.  dir_path must be a pointer to an array of at
 * least PATH_MAX bytes.
 */

OB_HIDDEN ob_retort pool_notify_dir_path (char *dir_path, const char *poolName,
                                          unt8 pdv);

/**
 * Called in the child after a fork, to close gang-related file handles.
 */
OB_HIDDEN void pool_fork_gangs (void);

// Below exported solely for testing purposes

/*
 * Multi-pool await functions operate on a cluster of pool hoses,
 * called a pool gang.  Gang members reside in a singly-linked list.
 * Pools are checked for proteins in a round-robin fashion, beginning
 * each time from the pool hose following the pool hose that supplied
 * the last protein.
 */

/**
 * A gang member simply contains the pool hose and a pointer to the
 * next gang member.
 */

typedef struct pool_gang_member_struct
{
  struct pool_gang_member_struct *next;
  pool_hose ph;
} * pool_gang_member;

#ifdef _MSC_VER
typedef struct pool_gang_wait_list_struct_s
{
  HANDLE wait_handle;
  struct pool_gang_wait_list_struct_s *next;
} pool_gang_wait_list_struct;
#endif

/* XXX: Is this the correct capitalization?  (bug 2484 comment 10) */
typedef enum {
  Gang_Unused = 400327467,
  Gang_Used = 606952633,
  Gang_Needs_Reconnection = 919081036 /* due to fork() */
} Gang_Status;

typedef struct
{
  pool_gang next;
  int32 /* Gang_Status */ status;
} gang_management_info;

/**
 * The pool gang struct keeps all information needed to execute a
 * multi-pool await.
 */

struct pool_gang_struct
{
  gang_management_info mgmt;

  /**
   * The leader is a pointer to the first gang member in the list.
   */
  pool_gang_member leader;
  /**
   * last_checked keeps a pointer to the last pool we found a protein
   * in so that we can fairly distribute reads to the pools.
   */
  pool_gang_member last_checked;

#ifdef _MSC_VER
  /**
   * on Win32 we put together a list of HANDLES which are from either
   * ph->notify_handle or a WSACreateEvent/WSAEventSelect
   */
  struct pool_gang_wait_list_struct *wait_list;
#else
  /**
   * wait_fds is a cached fd_set of all the notification fds
   * associated with the pool hoses in the gang.  select() operates
   * on a copy of this fd_set.
   */
  fd_set wait_fds;
  /**
   * maxfd is the largest file descriptor in our gang, for select().
   */
  int maxfd;
#endif

  wakeup_stuff w;
};

#ifdef __cplusplus
}
#endif

#endif /* POOL_MULTI_VERSE */
