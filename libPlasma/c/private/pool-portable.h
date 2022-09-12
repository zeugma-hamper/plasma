
/* (c)  oblong industries */

#ifndef POOL_PORTABLE_HOLE
#define POOL_PORTABLE_HOLE

#include "libLoam/c/ob-attrs.h"
#include "libLoam/c/ob-retorts.h"
#include "libLoam/c/ob-sys.h"

#include "libPlasma/c/pool-time.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
typedef HANDLE ob_handle_t;
#define OB_NULL_HANDLE 0
#else
typedef int ob_handle_t;
#define OB_NULL_HANDLE -1
#endif

typedef enum ob_select2_dir { OB_SEL2_RECEIVE, OB_SEL2_SEND } ob_select2_dir;

typedef struct ob_select2_t
{
#ifdef _MSC_VER
  HANDLE waitHandles[2];
#else
  fd_set read_fds;
  fd_set write_fds;
  fd_set except_fds;
  int nfds;
  int wake_fd;
#endif
  int sock_fd;
} ob_select2_t;

/**
 * This is an abstraction for waiting on a socket, an event handle,
 * or a timeout.  (On UNIX, the socket and "event handle" are both just
 * file descriptors, but on Windows they are different things.)
 * This function gets ready to do a select, by initializing an
 * ob_select2_t structure, which should be treated as opaque.
 * \a dir indicates whether to wait on the socket for sending or
 * receiving.  \a sock is the file descriptor of the socket.
 * \a event is an event to wait for, which on UNIX is just a file
 * descriptor that we wait to become readable, but on Windows it is
 * a HANDLE.
 */
OB_PLASMA_API ob_retort ob_select2_prepare (ob_select2_t *sel,
                                            ob_select2_dir dir, int sock,
                                            ob_handle_t event);
/**
 * This does the actual waiting.  It can be called multiple times in
 * between the ob_select2_prepare() and ob_select2_finish().
 * \a sel is the structure initialized by ob_select2_prepare().
 * \a timeout is the (floating point) number of seconds to wait before
 * timing out, or the special constants POOL_WAIT_FOREVER or
 * POOL_NO_WAIT.  On UNIX, \a consume means whether to read a byte from
 * the "event" file descriptor, if that is what wakes us up.  It has no
 * meaning on Windows.
 * Returns OB_OK if "sock" woke us up, POOL_AWAIT_WOKEN if "event" woke
 * us up, POOL_AWAIT_TIMEDOUT if we reached the timeout, or another error
 * code if an error occurred.
 */
OB_PLASMA_API ob_retort ob_select2 (ob_select2_t *sel, pool_timestamp timeout,
                                    bool consume);
/**
 * Cleans up an ob_select2_t structure that was initialized by
 * ob_select2_prepare().
 */
OB_PLASMA_API ob_retort ob_select2_finish (ob_select2_t *sel);

/**
 * Detects an OS-specific condition that indicates a read or write
 * should be retried.  \a nw is the return value of the send() or recv()
 * call.
 */
#ifdef _MSC_VER
#define SHOULD_TRY_AGAIN(nw)                                                   \
  (((nw) == 0 || (nw) == SOCKET_ERROR) && WSAGetLastError () == WSAEWOULDBLOCK)
#else
#define SHOULD_TRY_AGAIN(nw) ((nw) < 0 && errno == EINTR)
#endif

/* ---------------------------------------------------------------------- */

typedef struct
{
#ifdef _MSC_VER
  /**
   * A manual reset event used to cancel blocking reads.
   */
  HANDLE wakeup_event_handle;
#else
  /**
   * A socketpair used to cancel blocking reads.
   */
  int wakeup_write_fd;
  int wakeup_read_fd;
#endif
} wakeup_stuff;

/**
 * Wake up a wakeup_stuff struct.  This is used to factor the common
 * code in pool_hose_wake_up() and pool_gang_wake_up().
 */
OB_HIDDEN ob_retort ob_wake_up (const wakeup_stuff *w);

/**
 * Initialize a wakeup_stuff structure to a neutral value, but
 * does not enable it.
 */
OB_HIDDEN void ob_initialize_wakeup (wakeup_stuff *w);

/**
 * Enable a wakeup_stuff structure.  This is used to factor the
 * common code in pool_gang_init_wakeup() and pool_hose_enable_wakeup().
 * Must have been initialized with ob_initialize_wakeup().
 */
OB_HIDDEN ob_retort ob_enable_wakeup (wakeup_stuff *w);

/**
 * Decommission a wakeup_stuff structure.  This is used to factor the
 * common code in pool_gang_cleanup_wakeup() and
 * hose_close_descriptors().
 */
OB_HIDDEN ob_retort ob_cleanup_wakeup (wakeup_stuff *w);

#ifdef __cplusplus
}
#endif

#endif /* POOL_PORTABLE_HOLE */
