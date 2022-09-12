
/* (c)  oblong industries */

/**
 * This file includes syslog.h on UNIX, and defines dummy values
 * on Windows.  It is used by both ob-log.c and test-logging.c, but
 * we explicitly don't include it from any public headers (like ob-sys.h)
 * because syslog.h pollutes the namespace rather badly, and we don't
 * want to inflict that upon our users.  In particular, OpenCV also
 * defines LOG_INFO, and therefore OpenCV can't coexist with syslog.h.
 */

#ifndef OB_SYSLOG_SCALE
#define OB_SYSLOG_SCALE

#ifdef _MSC_VER

// syslog not supported
#define LOG_ERR 0
#define LOG_WARNING 0
#define LOG_INFO 0
#define LOG_DEBUG 0

#else

#include <syslog.h>

#endif /* _MSC_VER */

#endif /* OB_SYSLOG_SCALE */
