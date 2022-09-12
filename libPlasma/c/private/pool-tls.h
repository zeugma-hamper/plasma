
/* (c)  oblong industries */

#ifndef POOL_TLS_ELEPHANT
#define POOL_TLS_ELEPHANT

#include "libLoam/c/ob-api.h"
#include "libLoam/c/ob-retorts.h"
#include "libLoam/c/ob-pthread.h"

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Prints ob_banner() information, but also appends another line
 * about the TLS implementation.  Also, returns OB_OK or POOL_NO_TLS
 * to indicate the presence or absence of TLS support.
 */
OB_PLASMA_API ob_retort ob_tls_banner (FILE *where);

/**
 * Returns OB_OK if TLS is supported, or an error retort if it is not.
 * Or, returns the success code POOL_ANONYMOUS_ONLY if TLS is supported,
 * but certificates are not available.
 */
OB_PLASMA_API ob_retort ob_tls_server_available (void);

/**
 * Launches a thread which bidirectionally copies data between the
 * file descriptors \a clear_sock and \a cipher_sock, encrypting and
 * decrypting via TLS (you can guess which is which).
 *
 * The thread id is stored in \a thr_out, which is then suitable for
 * passing to ob_tls_server_join_thread().
 *
 * \a anon_ok indicates whether anonymous ciphersuites (vulnerable to
 * man-in-the-middle attacks, but no certificate required) should be
 * allowed.
 *
 * \a client_auth_reqd indicates whether the server should require
 * (and validate) a certificate from the client.
 */
OB_PLASMA_API ob_retort ob_tls_server_launch_thread (int clear_sock,
                                                     int cipher_sock,
                                                     pthread_t *thr_out,
                                                     bool anon_ok,
                                                     bool client_auth_reqd);

/**
 * Waits for a thread created by ob_tls_server_launch_thread() to finish.
 */
OB_PLASMA_API ob_retort ob_tls_server_join_thread (pthread_t thr);

/**
 * Returns OB_OK if TLS is supported, or an error retort if it is not.
 * Or, returns the success code POOL_ANONYMOUS_ONLY if TLS is supported,
 * but certificates are not available.
 */
ob_retort ob_tls_client_available (void);

/**
 * Launches a thread which bidirectionally copies data between the
 * file descriptors \a clear_sock and \a cipher_sock, encrypting and
 * decrypting via TLS (you can guess which is which).
 *
 * The thread id is stored in \a thr_out, which is then suitable for
 * passing to ob_tls_client_join_thread().
 *
 * \a anon_ok indicates whether anonymous ciphersuites (vulnerable to
 * man-in-the-middle attacks, but no certificate required) should be
 * allowed.
 *
 * \a certificate and \a private_key should either both be specified,
 * or both be NULL.  If specified, the client will present this certificate
 * to the server, rather than using the certificate found on ob_etc_path.
 * \a certificate and \a private_key should be strings containing the
 * entire certificate and private key, respectively, encoded in PEM
 * format.
 */
ob_retort ob_tls_client_launch_thread (int clear_sock, int cipher_sock,
                                       pthread_t *thr_out, const char *host,
                                       bool anon_ok, const char *certificate,
                                       const char *private_key);

/**
 * Waits for a thread created by ob_tls_client_launch_thread() to finish.
 */
ob_retort ob_tls_client_join_thread (pthread_t thr);

#ifdef __cplusplus
}
#endif

#endif /* POOL_TLS_ELEPHANT */
