
/* (c)  oblong industries */

#ifndef POOL_TCP_OFFLOAD
#define POOL_TCP_OFFLOAD

#include <stddef.h>                // for size_t
#include "libLoam/c/ob-retorts.h"  // for ob_retort
#include "libLoam/c/ob-file.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Information shared by TCP client and server.
 */

#ifdef _MSC_VER

/**
 * Send bytes over a TCP connection.
 */
OB_PLASMA_API ob_retort pool_tcp_send_nbytes (ob_sock_t fd, const void *buf,
                                              size_t len,
                                              HANDLE wakeup_event_handle_loc);

/**
 * Receive bytes over a TCP connection.
 */
OB_PLASMA_API ob_retort pool_tcp_recv_nbytes (ob_sock_t fd, void *buf,
                                              size_t len,
                                              HANDLE wakeup_event_handle_loc);

#else

/**
 * Send bytes over a TCP connection.
 */
OB_PLASMA_API ob_retort pool_tcp_send_nbytes (ob_sock_t fd, const void *buf,
                                              size_t len, int wake_fd);

/**
 * Receive bytes over a TCP connection.
 */
OB_PLASMA_API ob_retort pool_tcp_recv_nbytes (ob_sock_t fd, void *buf,
                                              size_t len, int wake_fd);

#endif

/**
 * Default port for a pool TCP server to listen on.
 */

#define POOL_TCP_PORT 65456

/**
 * The default port in string form, for getaddrinfo().
 */

// The below conversion from integer to string is fancy and hard to
// understand, but will never be out of sync with the actual port
// number.  See http://gcc.gnu.org/onlinedocs/cpp/Stringification.html
#define xstr(s) str (s)
#define str(s) #s
#define POOL_TCP_PORT_STR xstr (POOL_TCP_PORT)

#define POOL_TCP_VERSION_CURRENT 3
#define POOL_TCP_VERSION_WITH_NEW_PCREATINGLY_CODES 3

/**
 * Used by pool server to get error string from errno or WSAGetLastError().
 */
OB_PLASMA_API const char *ob_sockmsg (void);

/**
 * Used by pool server to set socket options (Nagle, DSCP) on a socket.
 */
OB_PLASMA_API ob_retort ob_common_sockopts (ob_sock_t sock);

/**
 * Used by pool server to disable SIGPIPE on a socket.  This only
 * does anything on OS X.  On Linux, it is necessary to use the
 * MSG_NOSIGNAL flag on send() instead.
 */
OB_PLASMA_API ob_retort ob_nosigpipe_sockopt (ob_sock_t sock);

/**
 * Like ob_nosigpipe_sockopt(), but does it to two sockets.
 */
OB_PLASMA_API ob_retort ob_nosigpipe_sockopt_x2 (ob_sock_t socks[2]);

#ifdef __cplusplus
#define EVIL_SOCKOPT_CAST(x) reinterpret_cast<const char *> (x)
#else
#define EVIL_SOCKOPT_CAST(x) (x)
#endif

#ifdef __cplusplus
}
#endif

#endif /* POOL_TCP_OFFLOAD */
