
/* (c)  oblong industries */

#ifndef _OB_SYS_H_
#define _OB_SYS_H_

#ifdef _MSC_VER

#include "win32/ob-sys-win32.h"

// Not only does Windows not support %e...
//   http://msdn.microsoft.com/en-us/library/fe06s4ak(VS.71).aspx
// ...but strftime() crashes if you use %e!
// Luckily, there is a non-standard alternative that does *almost*
// the same thing, although note that %e on UNIX will pad with a leading
// space where the leading zero would have been, while %#d on Windows
// omits the leading character entirely; thus the day of month is
// variable length.
#define OB_STRFTIME_DAY_OF_MONTH_NO_LEADING_ZERO "%#d"

#define OB_O_BINARY _O_BINARY

#else

#include <limits.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/sem.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <sys/ipc.h>
#include <sys/select.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/dir.h>
#include <sys/utsname.h>

#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <sched.h>

// %e is specified by the Single UNIX Specification
// (yes, most eunuchs are single)
#define OB_STRFTIME_DAY_OF_MONTH_NO_LEADING_ZERO "%e"

#define OB_O_BINARY 0

#define WINAPI  // Placeholder on non-windows platforms

#endif

/* This is platform-independent but depends on the network-related
 * includes above.  Should it go somewhere other than here? */

/**
 * A union that can hold either IPv4 or IPv6 addresses, and allow easy
 * casting between various sockaddr types, while avoiding
 * -Wstrict-aliasing warnings.
 */
typedef union
{
  struct sockaddr_storage ss;
  struct sockaddr sa;
  struct sockaddr_in in;
  struct sockaddr_in6 i6;
} ob_sox;

#endif /* _OB_SYS_H_ */
