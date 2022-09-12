
/* (c)  oblong industries */

#include <libLoam/c++/ObResolvedPath.h>

#include <libLoam/c/ob-log.h>


using namespace oblong::loam;


Str ObResolvedPath::ForFile (ob_standard_dir dir, const char *filename,
                             const char *searchspec, Path_Verbosity verbosity)
{
  char *s = ob_resolve_standard_path (dir, filename, searchspec);
  if (!s && Verbosity_Chatty == verbosity)
    OB_LOG_ERROR_CODE (0x11090000, "\"%s\" was not found in path:\n%s\n",
                       filename, ob_get_standard_path (dir));
  Str r (s);
  free (s);
  return r;
}


typedef ObTrove<Str> StrTrove;

static ob_retort trover (const char *name, va_list vargies)
{
  StrTrove *t = va_arg (vargies, StrTrove *);
  t->Append (name);
  return OB_OK;
}


StrTrove ObResolvedPath::ForFiles (ob_standard_dir dir, const char *filename,
                                   const char *searchspec)
{
  StrTrove result (2.0);
  ob_search_standard_path (dir, filename, searchspec, trover, &result);
  return result;
}
