
/* (c)  oblong industries */

#include <libLoam/c++/Str.h>
#include <libLoam/c++/ObRetort.h>
#include <libLoam/c++/Mutex.h>
#include <libLoam/c++/LoamStreams.h>

#include "libLoam/c/ob-hash.h"

#include <map>
#include <ostream>
#include <errno.h>


namespace oblong {
namespace loam {

namespace {


struct RDesc
{
  Str desc;
  // Whether `desc' was provided via ObRetort::Register (retort
  // registered in Loam++-land) or obtained from the C-side (using
  // ob_error_string_literal). That way, when the C-side asks us for a
  // code's string (using trans_func below), we only return genuinely
  // new ones.
  bool mine;

  RDesc () : mine (false) {}
  RDesc (ob_retort r) : desc (ob_error_string_literal (r)), mine (false)
  {
    if (0 == desc.Length ())
      desc.Sprintf ("ob_retort %" OB_FMT_RETORT "d", r);
  }
  RDesc (const Str &d) : desc (d), mine (true) {}
};

typedef ::std::map<ob_retort, RDesc> RDescs;

RDescs descs_;

struct DescsMutex : Mutex
{
  DescsMutex (ob_translation_func f) : Mutex (true)
  {
    // Register our function to translate our error names.
    // (See ob_add_error_names() Doxygen comment in ob-retorts.h)
    ob_add_error_names (f);
  }
};


extern "C" const char *trans_func (ob_retort err);
DescsMutex mutex_ (trans_func);

// Return the string for any codes registered through ObRetort::Register
// (but not codes that we've merely cached)
extern "C" const char *trans_func (ob_retort err)
{
  MutexLock lock (mutex_);

  RDescs::const_iterator found = descs_.find (err);
  const char *retval = NULL;
  if (found != descs_.end () && found->second.mine)
    retval = found->second.desc.utf8 ();

  return retval;
}
}
}
}  // anonymous namespace, and then loam and oblong


using namespace oblong::loam;


Str ObRetort::Description () const
{
  MutexLock lock (mutex_);

  if (descs_.count (c_retort) == 0)
    descs_.insert (::std::make_pair (c_retort, RDesc (c_retort)));

  Str retval (descs_[c_retort].desc);
  return retval;
}


ObRetort ObRetort::AccreteRetortPod (ObRetortPod *orp)
{
  if (!orp)
    return OB_ARGUMENT_WAS_NULL;

  ObRetortPod *ant_pod = RetortPod ();
  orp->SetAntecedent (ant_pod);
  SetRetortPod (orp);
  return OB_OK;
}


bool ObRetort::Register (ob_retort code, const Str &desc)
{
  MutexLock lock (mutex_);

  bool retval (descs_.count (code) == 0 && !ob_retort_exists (code));
  if (retval)
    descs_.insert (::std::make_pair (code, RDesc (desc)));

  return retval;
}


unt64 ObRetort::Hash () const
{
  return ob_hash_unt64 (c_retort);
}


namespace oblong {
namespace loam {

::std::ostream &operator<< (::std::ostream &os, const ObRetort &ret)
{
  return os << ret.Description ();
}
}
}  // end oblong::loam
