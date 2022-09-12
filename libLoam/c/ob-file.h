
/* (c)  oblong industries */

#ifndef OB_FILE_SYSTEM
#define OB_FILE_SYSTEM

#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-api.h"
#include "libLoam/c/ob-retorts.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * A typedef for sockets, since on Windows they are not the same thing
 * as file descriptors.
 */
typedef int ob_sock_t;

/**
 * Reads the entire contents of the specified file into a single
 * NUL-terminated string, and returns the string.  (malloced, so the
 * caller must free.)  Returns NULL if an error occurs.
 */
OB_LOAM_API char *ob_read_file (const char *filename);

/**
 * Reads the entire contents of the specified file into a single
 * mallocated byte array.
 */
OB_LOAM_API ob_retort ob_read_binary_file (const char *filename, unt8 **buf,
                                           unt64 *len);

/**
 * Same as UNIX pipe() function: opens two new file descriptors, where you can
 * write data to fildes[1] and then read it from fildes[0].
 * Supported on both Windows and UNIX.
 * Note that the close-on-exec bit is *not* set; use ob_pipe_cloexec()
 * if you want close-on-exec.
 */
OB_LOAM_API ob_retort ob_pipe (int fildes[2]);

/**
 * Identical to the open() system call, but sets the close-on-exec flag
 * for the new file descriptor.  Note than although the third argument
 * to open() is optional, here it is required.  Just pass 0 for the third
 * argument if you're not doing O_CREAT.
 */
OB_LOAM_API int ob_open_cloexec (const char *path, int oflag, int mode);

/**
 * Identical to the fopen() library function, but sets the close-on-exec flag
 * for the new file descriptor.
 */
OB_LOAM_API FILE *ob_fopen_cloexec (const char *filename, const char *mode);

/**
 * Identical to the dup() system call, but sets the close-on-exec flag
 * for the new file descriptor.
 */
OB_LOAM_API int ob_dup_cloexec (int oldfd);

/**
 * Identical to the dup2() system call, but sets the close-on-exec flag
 * for the new file descriptor.
 */
OB_LOAM_API int ob_dup2_cloexec (int oldfd, int newfd);

/**
 * Identical to the socket() system call, but sets the close-on-exec flag
 * for the new file descriptor.
 */
OB_LOAM_API int ob_socket_cloexec (int domain, int type, int protocol);

/**
 * Identical to the socketpair() system call, but sets the close-on-exec flag
 * for the new file descriptors, and the return value is a retort, rather
 * than using a 0/-1 return value and errno.  On Windows, \a domain must
 * be AF_INET.
 */
OB_LOAM_API ob_retort ob_socketpair_cloexec (int domain, int type, int protocol,
                                             int socket_vector[2]);

/**
 * Recommended value of "domain" when calling ob_socketpair_cloexec().
 */
#ifdef _MSC_VER
#define OB_SP_DOMAIN AF_INET
#else
#define OB_SP_DOMAIN AF_UNIX
#endif

/**
 * Like ob_pipe(), but sets the close-on-exec flag for the new
 * file descriptors.  Supported on both Windows and UNIX.  (Although it
 * is identical to ob_pipe() on Windows, since there is no close-on-exec
 * flag there.)
 */
OB_LOAM_API ob_retort ob_pipe_cloexec (int fildes[2]);

/**
 * Returns true if the specified file/directory name is on an NFS filesystem.
 * (Currently only NFS is detected, but the function is named so that it
 * could be expanded to return true for other network file systems if desired.)
 */
OB_LOAM_API bool ob_is_network_path (const char *path);

/**
 * Sets \a real_ret to a malloced string (yes, you need to free it with free)
 * which is \a path made absolute, with all symlinks, etc. removed.
 * Returns OB_OK on success or another retort on error.
 */
OB_LOAM_API ob_retort ob_realpath (const char *path, char **real_ret);

/**
 * Creates a new file in ob_tmp_dir whose filename begins with
 * \a prefix.  (So, overall form of the name will be "ob_tmp_dir/prefixXXXXXX",
 * where XXXXXX is an arbitrary sequence of characters which makes the
 * name unique.)  Allocates a string containing the new filename (which
 * you must free by calling free) and stores it in \a name_ret.  Also,
 * opens a descriptor to the file and stores it in \a fd_ret.  Returns
 * OB_OK on success, or another retort on failure.  This is similar
 * to the standard library function mkstemp(), but is portable (works on
 * Windows), automatically knows where the standard temporary directory
 * is, and has a slightly different interface.
 *
 * The flag \a binary indicates whether the file should be opened in text
 * or binary mode.  Only has an effect on Windows.
 */
OB_LOAM_API ob_retort ob_mkstemp (const char *prefix, char **name_ret,
                                  int *fd_ret, bool binary);

/**
 * Creates a new directory in ob_tmp_dir whose filename begins with
 * \a prefix.  (So, overall form of the name will be "ob_tmp_dir/prefixXXXXXX",
 * where XXXXXX is an arbitrary sequence of characters which makes the
 * name unique.)  Allocates a string containing the new directory name (which
 * you must free by calling free) and stores it in \a name_ret.  Returns
 * OB_OK on success, or another retort on failure.  This is similar
 * to the standard library function mkdtemp(), but is portable (works on
 * Windows), automatically knows where the standard temporary directory
 * is, and has a slightly different interface.
 */
OB_LOAM_API ob_retort ob_mkdtemp (const char *prefix, char **name_ret);

/**
 * Renames \a src to \a dst, failing if \a dst already exists.  This is
 * the behavior rename() already has on Windows, but on UNIX, rename()
 * will normally overwrite \a dst if it exists.
 *
 * Return value is the same as rename().  0 on success, -1 on error with
 * errno set.
 */
OB_LOAM_API int ob_non_overwriting_rename (const char *src, const char *dst);

/**
 * Makes \a fd nonblocking.
 */
OB_LOAM_API ob_retort ob_make_socket_nonblocking (int fd);

/**
 * Makes \a fd blocking.
 */
OB_LOAM_API ob_retort ob_make_socket_blocking (int fd);

/**
 * Closes socket \a sock.
 */
OB_LOAM_API ob_retort ob_close_socket (ob_sock_t sock);

/**
 * Returns true if \a sock represents an invalid socket.
 * (e. g. an error condition)
 */
#ifdef _MSC_VER
#define OB_IS_BAD_SOCK(s) ((s) == INVALID_SOCKET)
#else
#define OB_IS_BAD_SOCK(s) ((s) < 0)
#endif

#ifdef __cplusplus
}
#endif

#endif /* OB_FILE_SYSTEM */
