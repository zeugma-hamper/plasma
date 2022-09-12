
/* (c)  oblong industries */

#ifndef OB_PATH_OF_LEAST_RESISTANCE
#define OB_PATH_OF_LEAST_RESISTANCE


#include <libLoam/c/ob-api.h>
#include <libLoam/c/ob-dirs.h>

#include <libLoam/c++/ObTrove.h>
#include <libLoam/c++/Str.h>


namespace oblong {
namespace loam {


/**
 * Object-oriented versions of ob_resolve_standard_path()
 */
class OB_LOAMXX_API ObResolvedPath
{
 public:
  /**
   * Specify whether the function should print an error message.
   */
  enum Path_Verbosity
  {
    Verbosity_Taciturn,
    Verbosity_Chatty
  };

  /**
   * Returns the first
   * instance of \a filename in the search path specified by \a dir which
   * meets the criteria specified in \a searchspec.
   *
   * \a searchspec may contain the following characters:
   * 'd' directory specified by path exists
   * 'r' directory specified by path is readable
   * 'w' directory specified by path is writable
   * 'c' directory specified by path can be created
   * 'l' directory specified by path is on a local (non-NFS) filesystem
   * 'F' filename exists as a regular file
   * 'D' filename exists as a directory
   * 'R' filename is readable
   * 'W' filename is writable
   *
   * Concatenating multiple letters together is an "and", meaning that
   * a single component of the path must fulfill all the specified conditions
   * in order to match.  So, "Rw" would mean the file is readable, and
   * exists in a writable directory.
   *
   * The character '|' can be used to mean "or".  "and" binds more tightly
   * than "or".  So, "RF|w" means either the specified filename is readable
   * and is a regular file, or the directory in the path is writable.
   *
   * The character ',' means "start over and go through all the path components
   * again".  '|' binds more tightly than ','.  So, "RF,w,c" means first
   * check all the directories in the path to see if any of them contain
   * a readable, regular file named \a filename.  Then, go through all the
   * directories in the path and see if any of them are writable.  Then,
   * go through all the directories in the path and see if any of them
   * can be created.
   *
   * The default of "FR" (a regular file that is readable) is generally
   * suitable if what you want to do is read an existing file.
   *
   * Symbolic links are not treated specially.  (In other words, they are
   * always followed.)
   *
   * If \a filename is absolute, then you just get it back as-is,
   * without searching the path.
   *
   * If \a filename is not found in the path, a null string is returned.
   * Additionally, a message is printed if \a verbosity is Verbosity_Chatty.
   */
  static oblong::loam::Str
  ForFile (ob_standard_dir dir, const char *filename,
           const char *searchspec = "FR",
           Path_Verbosity verbosity = Verbosity_Chatty);

  /**
   * Same as ForFile(), but returns all matches, rather than
   * just the first.
   */
  static oblong::loam::ObTrove<oblong::loam::Str>
  ForFiles (ob_standard_dir dir, const char *filename,
            const char *searchspec = "FR");

 private:
  ObResolvedPath ();  // do not instantiate me
};
}
}  // insert suitably obtuse comment about closing oblong::loam


#endif /* OB_PATH_OF_LEAST_RESISTANCE */
