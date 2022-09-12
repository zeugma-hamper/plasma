
/* (c)  oblong industries */

#ifndef POOL_NET_METERING
#define POOL_NET_METERING

#include "libLoam/c/ob-attrs.h"
#include "libPlasma/c/private/pool-portable.h"
#include "libLoam/c/ob-file.h"

// XXX: eventually don't want to have to include this here
#include "libLoam/c/ob-pthread.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ARGS_KEY "args"
#define OP_KEY "op"

typedef enum outstanding_await_e {
  OB_NO_AWAIT,
  OB_OLD_AWAITING,
  OB_FANCY_PREPARING,
  OB_FANCY_AWAITING,
  OB_FANCY_ARRIVED
} outstanding_await_e;

typedef struct outstanding_await_t
{
  outstanding_await_e status;
  slaw pattern;
  int64 idx;
} outstanding_await_t;

/**
 * Generic support for network pool operations, independent of
 * underlying transport.  Includes support for remote execution of
 * pool operations.
 */

/**
 * A pool_net_data structure contains all the state necessary for
 * steady-state network operations for a single pool hose.
 */

typedef struct
{
  /**
   * connfd is a file descriptor that is passed to the send/recv
   * functions (presumably so that they can use it to send and
   * receive data).  It must also have the property that after await
   * is set up on this connection, data will be written to it when a
   * deposit occurs.  It must be select()-able.
   */
  ob_sock_t connfd;

  /**
   * Windows description:
   * wakeup_event_handle_loc is the location of the event used to
   * wake a hose up from a blocking pool read. its a pointer usually
   * to the wakeup_event_handle of the pool_hose_struct. because
   * wakeup may be enabled and disabled, the HANDLE referenced here
   * may be valid or invalid, so we use a pointer so that we dont
   * have to track the changes that occur due to enable/disable_wakeup
   *
   * UNIX description:
   * wakeup_fd is the location of the read end of a socketpair used to
   * wake up blocking reads, for example to wake up an await on a
   * protein that will never come. it's a pointer because not all pool
   * hoses have a wakeup enabled at all times.
   */
  ob_handle_t *wakeup_handle_loc;

  /**
   * send_nbytes() is a function that sends the data in buf of length
   * len to the other end of the network connection.
   */
  ob_retort (*send_nbytes) (ob_sock_t fd, const void *buf, size_t len,
                            ob_handle_t wake_event_handle);

  /**
   * recv_nbytes() is a function that reads len bytes of data into
   * buf from the other end of the network connection.
   */
  ob_retort (*recv_nbytes) (ob_sock_t fd, void *buf, size_t len,
                            ob_handle_t wake_event_handle);

  /**
   * What version has been negotiated for this connection?
   */
  unt8 net_version;   // version of network protocol
  unt8 slaw_version;  // version of slaw encoding

  /**
   * When await has already been set up on this pool hose, this
   * structure contains information about the outstanding await.
   * This allows useful (though insanely tricky) optimizations.
   */
  outstanding_await_t outstanding;

  /**
   * Bitmask of supported commands.  (See \#defines below, and
   * pool-tcp-protocol.txt.)  This will need to be changed if we
   * ever support more than 64 commands.
   */
  unt64 supported_cmds;

  /**
   * Thread for TLS.  Would eventually like to refactor this somehow.
   */
  pthread_t tls_thread;
} pool_net_data;

static inline bool pool_net_supports_cmd (const pool_net_data *net, int cmd_num)
{
  return (0 != ((OB_CONST_U64 (1) << cmd_num) & net->supported_cmds));
}

/**
 * Network operation identification numbers
 */

#define POOL_CMD_CREATE 0
#define POOL_CMD_DISPOSE 1
#define POOL_CMD_PARTICIPATE 2
#define POOL_CMD_PARTICIPATE_CREATINGLY 3
#define POOL_CMD_WITHDRAW 4
#define POOL_CMD_DEPOSIT 5
#define POOL_CMD_NTH_PROTEIN 6
#define POOL_CMD_NEXT 7
#define POOL_CMD_PROBE_FRWD 8
#define POOL_CMD_NEWEST_INDEX 9
#define POOL_CMD_OLDEST_INDEX 10
#define POOL_CMD_AWAIT_NEXT_SINGLE 11
#define POOL_CMD_MULTI_ADD_AWAITER 12
// Below should never be used in current code but exists logically
// #define POOL_CMD_MULTI_REMOVE_AWAITER  13
#define POOL_CMD_RESULT 14
#define POOL_CMD_INFO 15
#define POOL_CMD_LIST 16
#define POOL_CMD_INDEX_LOOKUP 17
#define POOL_CMD_PROBE_BACK 18
#define POOL_CMD_PREV 19
#define POOL_CMD_FANCY_ADD_AWAITER 20
#define POOL_CMD_SET_HOSE_NAME 21
#define POOL_CMD_SUB_FETCH 22
#define POOL_CMD_RENAME 23
#define POOL_CMD_ADVANCE_OLDEST 24
#define POOL_CMD_SLEEP 25
// 26 was never used in a publicly released branch, so is safe to reuse
#define POOL_CMD_CHANGE_OPTIONS 27
#define POOL_CMD_LIST_EX 28
#define POOL_CMD_SUB_FETCH_EX 29
#define POOL_CMD_STARTTLS 30
#define POOL_CMD_GREENHOUSE 31
#define POOL_CMD_FANCY_RESULT_1 64
#define POOL_CMD_FANCY_RESULT_2 65
#define POOL_CMD_FANCY_RESULT_3 66

/**
 * Support to automatically pack and unpack operation packets
 * according to the format string specified in the send and receive
 * operation functions.  See pool_net.c for more details on the
 * format.
 */

/**
 * Send an operation to be executed on the remote host.  op_num is
 * the operation number (POOL_CMD_*), format is a string describing
 * the arguments to that operation that should be packed up and sent
 * along, and the following arguments are the values of the specified
 * arguments.
 *
 * Example:
 *     pret = pool_net_send_op (ph->net, POOL_CMD_NTH_PROTEIN, "i", index);
 *
 * Returns POOL_SEND_BADTH if something goes wrong with the
 * connection; otherwise OB_OK.
 *
 * If \a format contains illegal characters, calls abort() by way of
 * OB_FATAL_BUG_CODE().
 *
 * Must be followed by a call to pool_net_recv_result() to get the
 * return value from the sent operation.
 */

OB_PLASMA_API OB_WARN_UNUSED_RESULT ob_retort
pool_net_send_op (pool_net_data *net, int op_num, const char *format, ...);

/**
 * Receive an operation from the remote host.  The operation is
 * returned in packed form in op_prot, which will be later unpacked
 * into the proper arguments by pool_net_unpack_op().
 *
 * Example:
 *
 *    pret = pool_net_recv_op (net, &op_num, &results);
 *
 * Must be followed by a call to pool_net_send_result() to send the
 * return value after executing this operation.
 */

OB_PLASMA_API OB_WARN_UNUSED_RESULT ob_retort
pool_net_recv_op (pool_net_data *net, int *op_num, protein *op_prot);

/**
 * Based on the operation number, unpack a received operation
 * according to the corresponding format.  Put the unpacked arguments
 * into the appropriate variables following fmt.
 *
 * Example:
 *     pret = pool_net_unpack_op (op_protein, "ssp", &pool_name, &type,
 *                                &options);
 */

OB_PLASMA_API OB_WARN_UNUSED_RESULT ob_retort
pool_net_unpack_op (bprotein op_prot, unt32 net_version, const char *fmt, ...);

/**
 * Based on the operation number, unpack a received operation
 * according to the corresponding format.  Put the unpacked arguments
 * into the appropriate variables following fmt.
 *
 * Example:
 *     pret = pool_net_unpack_op (op_protein, "ssp", &pool_name, &type,
 *                                &options);
 *
 * Frees op_prot on success.
 */

OB_PLASMA_API OB_WARN_UNUSED_RESULT ob_retort
pool_net_unpack_op_f (protein op_prot, unt32 net_version, const char *fmt, ...);

/**
 * Send the return code from an operation to the sender.
 */

OB_PLASMA_API OB_WARN_UNUSED_RESULT ob_retort
pool_net_send_result (pool_net_data *net, const char *fmt, ...);

/**
 * Receive the return code from the last operation sent.
 */

OB_PLASMA_API OB_WARN_UNUSED_RESULT ob_retort
pool_net_recv_result (pool_net_data *net, const char *fmt, ...);

/**
 * Generic versions of network-based pool operations.  A pool type
 * implementation need only define the send/recv operations and then
 * it can use these routines - just fill out the function pointers in
 * the pool hose struct.
 */

ob_retort pool_net_newest_index (pool_hose ph, int64 *idx) OB_HIDDEN;
ob_retort pool_net_oldest_index (pool_hose ph, int64 *idx) OB_HIDDEN;

ob_retort pool_net_index_lookup (pool_hose ph, int64 *idx, pool_timestamp ts,
                                 time_comparison cmp, bool rel) OB_HIDDEN;

ob_retort pool_net_set_hose_name (pool_hose ph, const char *name) OB_HIDDEN;

ob_retort pool_net_deposit (pool_hose ph, bprotein p, int64 *idx,
                            pool_timestamp *ret_ts) OB_HIDDEN;
ob_retort pool_net_nth_protein (pool_hose ph, int64 idx, protein *return_prot,
                                pool_timestamp *ret_ts) OB_HIDDEN;
ob_retort pool_net_next (pool_hose ph, protein *return_prot,
                         pool_timestamp *ret_ts, int64 *ret_idx) OB_HIDDEN;
ob_retort pool_net_opportunistic_next (pool_hose ph, protein *return_prot,
                                       pool_timestamp *ret_ts,
                                       int64 *ret_idx) OB_HIDDEN;
ob_retort pool_net_prev (pool_hose ph, protein *return_prot,
                         pool_timestamp *ret_ts, int64 *ret_idx) OB_HIDDEN;
ob_retort pool_net_probe_frwd (pool_hose ph, bslaw search, protein *ret_prot,
                               pool_timestamp *ret_ts,
                               int64 *ret_index) OB_HIDDEN;
ob_retort pool_net_probe_back (pool_hose ph, bslaw search, protein *ret_prot,
                               pool_timestamp *ret_ts,
                               int64 *ret_index) OB_HIDDEN;
ob_retort pool_net_await_probe_frwd (pool_hose ph, bslaw search,
                                     pool_timestamp timeout, protein *ret_prot,
                                     pool_timestamp *ret_ts,
                                     int64 *ret_index) OB_HIDDEN;
ob_retort pool_net_fetch (pool_hose ph, pool_fetch_op *ops, int64 nops,
                          int64 *oldest_idx_out, int64 *newest_idx_out,
                          bool clamp) OB_HIDDEN;
ob_retort pool_net_advance_oldest (pool_hose ph, int64 idx_in) OB_HIDDEN;
ob_retort pool_net_change_options (pool_hose ph, bslaw options) OB_HIDDEN;

ob_retort pool_net_participate (pool_hose ph) OB_HIDDEN;
ob_retort pool_net_withdraw (pool_hose ph) OB_HIDDEN;

ob_retort pool_net_await_next_single (pool_hose ph, pool_timestamp timeout,
                                      protein *ret_prot, pool_timestamp *ret_ts,
                                      int64 *ret_index) OB_HIDDEN;

ob_retort pool_net_multi_add_awaiter (pool_hose ph, protein *ret_prot,
                                      pool_timestamp *ret_ts,
                                      int64 *ret_index) OB_HIDDEN;

void pool_net_multi_remove_awaiter (pool_hose ph) OB_HIDDEN;

void pool_net_free (pool_net_data *freeme) OB_HIDDEN;

/**
 * Interruptible await for the server side.  Allows a remote client
 * to set up an await and cancel it by sending any other command.
 */

OB_PLASMA_API ob_retort pool_net_server_await (pool_hose ph, pool_net_data *net,
                                               pool_timestamp timeout,
                                               protein *ret_prot,
                                               pool_timestamp *ret_ts,
                                               int64 *ret_index);

OB_PLASMA_API void
pool_net_adjust_timeout_value_for_version (pool_timestamp *timeout,
                                           unt8 net_version);

#ifdef __cplusplus
}
#endif

#endif /* POOL_NET_METERING */
