
/* (c)  oblong industries */

#ifndef _OB_SYS_WIN32_H_
#define _OB_SYS_WIN32_H_

//Using NOMINMAX is the only not-completely-evil way to include <windows.h>.
#define NOMINMAX

#include "libLoam/c/ob-api.h"  //for import/export macro OB_LOAM_API
#include "libLoam/c/ob-log.h"

#include <WinSock2.h>  //for sockets
#include <direct.h>    //for mkdir
#include <process.h>   //for getpid
#include <ws2tcpip.h>
#include <io.h>  //for _open, etc
#include <share.h>
#include <stdarg.h>  // for va_list used by ob_vsnprintf() below
#include <stdio.h>
#include <malloc.h>
#include <fcntl.h>  // _O_APPEND, etc.

#include <sys/types.h>
#include <sys/stat.h>

#ifdef __cplusplus
extern "C" {
#endif

#define R_OK 4
#define W_OK 2

//replaces any non-alphanumeric character with _
//ensures valid names (typically from filesystem paths)
//are used in creating named object handles via CreateEvent,
//CreateMutex, CreateSemaphore, etc
OB_LOAM_API void prepare_windows_handle_name (char *name);

#define PATH_MAX 260  //taken from from WinDef.h

#define GET_X_LPARAM(lp) ((int) (short) LOWORD (lp))  //taken from Windowsx.h
#define GET_Y_LPARAM(lp) ((int) (short) HIWORD (lp))

#if defined(_WIN64)
typedef __int64 ssize_t;
#else
typedef long ssize_t;
#endif

//windows does not implement the mode_t style permissions
//but this does at least allow code to compile
typedef int mode_t;
#ifndef va_copy
#define va_copy(dest, src) (dest) = (src)
#endif

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

#ifndef STDOUT_FILENO
#define STDOUT_FILENO 1
#endif

#ifndef STDERR_FILENO
#define STDERR_FILENO 2
#endif

// Microsoft insists on prepending standard symbols with an underscore,
// just to make portability more annoying.
#define O_WRONLY _O_WRONLY
#define O_APPEND _O_APPEND
#define O_CREAT _O_CREAT

#define strtok_r strtok_s

// Unfortunately, Windows _snprintf is not exactly the same as
// C99 snprintf.  See detailed comment in ob-sys-win32.c.
#ifndef __cplusplus
#define snprintf ob_snprintf
#endif
#define vsnprintf ob_vsnprintf
#define asprintf ob_asprintf
#define vasprintf ob_vasprintf

#define strcasecmp stricmp
#define strncasecmp strnicmp

#define alloca _alloca

#define localtime_r(a, b) localtime_s (b, a)

OB_LOAM_API int ob_snprintf (char *buf, size_t len, const char *fmt, ...);
OB_LOAM_API int ob_vsnprintf (char *buf, size_t len, const char *fmt,
                              va_list ap);
OB_LOAM_API int ob_asprintf (char **out_ptr, const char *fmt, ...);
OB_LOAM_API int ob_vasprintf (char **out_ptr, const char *fmt, va_list ap);

#define strtoull(str, endptr, base) _strtoui64 (str, endptr, base)
#define strtoll(str, endptr, base) _strtoi64 (str, endptr, base)
#define atoll(nptr) strtoll (nptr, (char **) NULL, 10)

#define getpid _getpid
#define getcwd _getcwd

//fork can't work on windows, will output an error message and return -1
typedef int pid_t;

OB_LOAM_API pid_t fork ();

// Windows emulation of scandir(), from 4.2BSD and POSIX.1-2008

enum
{
  DT_UNKNOWN,
  DT_DIR,
  DT_REG,
  DT_LNK
};

struct dirent
{
  WIN32_FIND_DATA d_win32;
  char *d_name;          // points to d_win32.cFileName
  unsigned char d_type;  // DT_DIR, DT_REG, or DT_LNK
};

OB_LOAM_API int ob_scandir (const char *dirp, struct dirent ***namelist,
                            int (*filter) (const struct dirent *),
                            int (*compar) (const struct dirent **,
                                           const struct dirent **));

//win32 replacement for getopt() and associated extern int's
extern OB_LOAM_API int optind;
extern OB_LOAM_API int opterr;
extern OB_LOAM_API int optopt;
extern OB_LOAM_API char *optarg;

OB_LOAM_API int getopt (int argc, char **argv, char *opts);

//win32 replacement for basename()
OB_LOAM_API char *basename (char *name);

//unixy time functions
OB_LOAM_API void gettimeofday (struct timeval *p, void *tz /* IGNORED */);

OB_LOAM_API int sched_yield (void);
OB_LOAM_API int usleep (unsigned useconds);
OB_LOAM_API int sleep (unsigned seconds);

//#define index(a,b)  strchr((a),(b))
//#define rindex(a,b) strrchr((a),(b))

// strsep() is a non-standard function available on BSD and Linux,
// but we happen to use it in a test.
OB_LOAM_API char *strsep (char **stringp, const char *delim);

//init/shutdown for winsock
OB_LOAM_API void winsock_init ();
OB_LOAM_API void winsock_shutdown ();

//This is a stub function that is called from a number of places in yovo where
//methods or functions have been partially implemented, partially tested, or are
//in general not ready for prime time.
OB_LOAM_API void ms_windows_not_ready ();

/**
 * Windows critical sections seem to have a problem; you have
 * to initialize them with InitializeCriticalSection() before
 * you can use them.  If you want to protect a global object, and
 * are worried about the "initialization order fiasco":
 * http://www.parashift.com/c++-faq-lite/ctors.html#faq-10.12
 * then there's no good time to initialize it.
 *
 * This function solves that problem by using atomic ops to make
 * sure the critical section gets initialized before the first
 * time it is used.  (Unfortunately, DeleteCriticalSection will
 * never be called, but I think that's okay for objects of global
 * scope, because the OS should reclaim the resources on program
 * termination.)
 *
 * To use it, have a global/static variable of type "void *"
 * that represents your critical section.  (Which will start out
 * as NULL.)  Then call ob_fetch_critical() with the address of
 * your void * variable whenever you want to use the critical
 * section.  You'll get back a pointer to the critical section,
 * which will be allocated and initialized if necessary.
 */
OB_LOAM_API CRITICAL_SECTION *ob_fetch_critical (void **vpp);

/**
 * Encapsulates a win32 error, as returned by GetLastError(),
 * in an ob_retort.
 */
OB_LOAM_API ob_retort ob_win32err_to_retort (DWORD w32err);

/**
 * Returns the win32 that was encapsulated in the given retort,
 * or ~0 if the retort does not encapsulate a win32 error.
 */
OB_LOAM_API DWORD ob_retort_to_win32err (ob_retort tort);

typedef struct
{
  char dll[32];  /* name of dll function is in */
  char func[32]; /* name of function you wish for */
  // below is internal to implementation
  void *critical;
  HMODULE module;
  FARPROC proc;
  DWORD misfortune;
} ob_w32_func_wish;

/**
 * Convenience for loading functions at runtime.  Pass a pointer to a
 * static ob_w32_func_wish structure, whose dll and func members have
 * been initialized to the name of the dll and function you want.
 * Returns the address of the function (which is now cached in the
 * structure) if successful, or NULL (and sets LastError) if not.
 */
OB_LOAM_API FARPROC ob_w32_wish_for_func (ob_w32_func_wish *wish);

/**
 * \cond internal
 * ob-retorts.c calls this function to translate Windows-specific retorts.
 * It is not OB_LOAM_API because it is being called from the same DLL.
 * You should not call it; it is private
 */
const char *ob_private_w32_translation (ob_retort t);
/** \endcond */

// C++ Windows alternative for lrint for Visual Studio 2010
// from: http://stackoverflow.com/questions/15319494/c-windows-alternative-to-lrint
// C99 library support is now in Visual Studio 2013 (version 1800)
#if defined(_MSC_VER) && _MSC_VER < 1800
OB_LOAM_API long int lrint (double num);
#endif

// C Windows alternative for strptime for Visual Studio 2010
//
#if defined(_MSC_VER)
OB_LOAM_API char *strptime (const char *buf, const char *format,
                            struct tm *timeptr);
#endif

#ifdef __cplusplus
}
#endif

#endif
