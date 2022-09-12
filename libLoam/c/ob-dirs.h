
/* (c)  oblong industries */

#ifndef OB_DIRS_HIERARCHY
#define OB_DIRS_HIERARCHY

#include "libLoam/c/ob-types.h"
#include "libLoam/c/ob-api.h"
#include "libLoam/c/ob-retorts.h"
#include "libLoam/c/ob-attrs.h"

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
/**
 * Character that separates directories in a filesystem path.
 */
#define OB_DIR_CHAR '\\'
/**
 * Character that separates elements in a search path.
 */
#define OB_PATH_CHAR ';'
#else
/**
 * Character that separates directories in a filesystem path.
 */
#define OB_DIR_CHAR '/'
/**
 * Character that separates elements in a search path.
 */
#define OB_PATH_CHAR ':'
#endif

/**
 * Argument to ob_search_standard_path() and friends.  Ones that end
 * in "path" are colon (semi on Windows) separated paths of
 * directories, but ones that end in "dir" are just single
 * directories.
 */
typedef enum ob_standard_dir {
  /**
   * non-changing "resource" files like fonts, images, videos
   */
  ob_share_path,
  /**
   * configuration files that the user might edit (layout.V)
   */
  ob_etc_path,
  /**
   * files created automatically to hold state (pids, pools)
   */
  ob_var_path,
  /**
   * directory for creating temporary/scratch files
   */
  ob_tmp_dir,
  /**
   * override the location for pools
   */
  ob_pools_dir,
  /**
   * shouldn't be needed at runtime, but yobuild is here
   */
  ob_yobuild_dir,
  /**
   * the --prefix specified to "configure"
   */
  ob_prefix_dir
} ob_standard_dir;

/**
 * Returns the pathname for one of the standard directories enumerated
 * in ob_standard_dir.  These are determined by checking various
 * environment variables and then falling back to a hardcoded path.
 * The returned string is a constant and should not be freed.
 */
OB_LOAM_API OB_CONST const char *ob_get_standard_path (ob_standard_dir dir);

/**
 * Calls ob_get_standard_path() and then concatenates "subdir" onto
 * the end of it, with a slash in between.  The return value is a
 * malloced string and should be freed.  Returns NULL if malloc fails.
 * \note We should use ob_resolve_standard_path() instead.
 */
OB_LOAM_API char *ob_concat_standard_path (ob_standard_dir dir,
                                           const char *subdir);

/**
 * Function pointer accepted by ob_search_path() and
 * ob_search_standard_path().
 */
typedef ob_retort (*ob_path_callback) (const char *file, va_list vargies);

/**
 * This is the function which underlies ob_search_standard_path().
 * You can use it to search a path of your own choosing, rather than a
 * standard path.  It's also useful if you want to pass the "cookies"
 * for the callback function as a va_list instead of as "..."
 */
OB_LOAM_API ob_retort ob_search_path (const char *path, const char *filename,
                                      const char *searchspec,
                                      ob_path_callback func, va_list vargies);

/**
 * Similar to ob_resolve_standard_path(), but uses a callback
 * interface so that multiple matches can be returned.  For each
 * match, calls func() with the matching filename, and a va_list which
 * contains any "..." arguments that were specified.  func() will keep
 * being called until there are no more matches, or until it returns a
 * value other than OB_OK.  If func() returns an error code, the
 * search will stop, and the retort will be returned by
 * ob_search_standard_path().  If you want to stop the search without
 * indicating an error, you can return the success code OB_STOP.
 */
OB_LOAM_API ob_retort ob_search_standard_path (ob_standard_dir dir,
                                               const char *filename,
                                               const char *searchspec,
                                               ob_path_callback func, ...);

/**
 * Returns (as an allocated string, that you need to free), the first
 * instance of \a filename in the search path specified by \a dir
 * which meets the criteria specified in \a searchspec.
 *
 * \a searchspec may contain the following characters:
 * 'd' directory specified by path exists
 * 'r' directory specified by path is readable
 * 'w' directory specified by path is writable
 * 'c' directory specified by path can be created
 * 'l' directory specified by path is on a local (non-NFS) filesystem
 * 'E' filename exists (as anything)
 * 'F' filename exists as a regular file
 * 'D' filename exists as a directory
 * 'R' filename is readable
 * 'W' filename is writable
 *
 * Concatenating multiple letters together is an "and", meaning that a
 * single component of the path must fulfill all the specified
 * conditions in order to match.  So, "Rw" would mean the file is
 * readable, and exists in a writable directory.
 *
 * The character '|' can be used to mean "or".  "and" binds more
 * tightly than "or".  So, "RF|w" means either the specified filename
 * is readable and is a regular file, or the directory in the path is
 * writable.
 *
 * The character ',' means "start over and go through all the path
 * components again".  '|' binds more tightly than ','.  So, "RF,w,c"
 * means first check all the directories in the path to see if any of
 * them contain a readable, regular file named \a filename.  Then, go
 * through all the directories in the path and see if any of them are
 * writable.  Then, go through all the directories in the path and see
 * if any of them can be created.
 *
 * Symbolic links are not treated specially.  (in other words, they
 * are always followed.)
 *
 * If \a filename is absolute, then you just get it back as-is,
 * without searching the path.
 *
 * Returns NULL if \a filename is not found in the path.
 */
OB_LOAM_API char *ob_resolve_standard_path (ob_standard_dir dir,
                                            const char *filename,
                                            const char *searchspec);

/**
 * Creates the specified directory, including any directories leading
 * up to it if necessary.  (Like "mkdir -p", hence the name.)  It is
 * *not* considered an error for the directory to already exist, so
 * OB_OK will be returned if the directory exists on return,
 * regardless of whether it was created or already existed.  A
 * non-OB_OK error code will be returned if the directory could not be
 * created.
 */
OB_LOAM_API ob_retort ob_mkdir_p (const char *dir);

/**
 * Removes the specified directory, if it is empty, and then proceeds
 * up the directory tree, removing each parent directory only if it is
 * now empty.  Thus, it is like "rmdir -p".  Non-OB_OK retorts are
 * only returned for serious/unusual errors.  In particular, OB_OK is
 * returned, and the directory is silently not deleted, if the
 * directory is in use, not empty, or the user doesn't have permission
 * to remove it.
 */
OB_LOAM_API ob_retort ob_rmdir_p (const char *dir);

/**
 * Same as ob_rmdir_p(), but takes an optional second argument, which
 * is the directory to stop at.  \a stop will not be deleted, even if
 * it is empty, and the user has permission to delete it.  If \a stop
 * is NULL, or not an ancestor of \a dir, then ob_rmdir_p_ex() behaves
 * the same as ob_rmdir_p().
 */
OB_LOAM_API ob_retort ob_rmdir_p_ex (const char *dir, const char *stop);

/**
 * Sets the mode, owner, and/or group of a file or directory.  Any of
 * mode, uid, or gid may be -1, in which case that attribute is left
 * unchanged.  Otherwise, it is changed to the specified value.
 * Returns OB_OK if no error occurs, or some other retort, probably an
 * errno wrapped by ob_errno_to_retort(), if an error occurs.
 */
OB_LOAM_API ob_retort ob_permissionize (const char *path, int mode, int uid,
                                        int gid);

/**
 * Same as ob_mkdir_p(), but applies ob_permissionize() to each newly
 * created directory, with the specified mode, uid, and gid.
 */
OB_LOAM_API ob_retort ob_mkdir_p_with_permissions (const char *dir, int mode,
                                                   int uid, int gid);

/**
 * An alternate version of the C library basename() function.  Returns
 * a pointer to inside the supplied string.  (Points to either the
 * character after the last slash, or to the beginning of the string
 * if there is no slash.)
 *
 * The basename() function in the C library is allowed to modify its
 * argument.  This is because the C library version strips trailing
 * slashes; when given "/foo/bar/", it returns "bar".  However, our
 * version does not do that, and would return "" when given
 * "/foo/bar/".  The advantage is that our version does not modify its
 * argument, and therefore can operate on a const string.
 */
OB_LOAM_API const char *ob_basename (const char *longname);

/**
 * Returns true if \a name is absolute.  (Begins with a slash, or on
 * Windows a backslash or drive letter.)
 */
OB_LOAM_API bool ob_is_absolute (const char *name);

/**
 * Returns true if \a name is explicit.  (Either absolute, or begins
 * with "./" or "../")
 */
OB_LOAM_API bool ob_is_explicit (const char *name);

struct dirent;
#ifndef _MSC_VER /* on Windows, ob_scandir is in ob-sys.h */
typedef int (*ob_sd_filt_func) (const struct dirent *);
typedef int (*ob_sd_comp_func) (const struct dirent **, const struct dirent **);

/**
 * Like the library function scandir(), but avoids the problem of
 * d_type being DT_UNKNOWN, which happens on ReiserFS.
 */
OB_LOAM_API int ob_scandir (const char *dirp, struct dirent ***namelist,
                            ob_sd_filt_func filter, ob_sd_comp_func compar);
#endif

/**
 * Frees the namelist allocated by ob_scandir().
 */
OB_LOAM_API void ob_scandir_free (struct dirent **namelist, int nentries);

#ifdef __cplusplus
}
#endif

#endif /* OB_DIRS_HIERARCHY */
