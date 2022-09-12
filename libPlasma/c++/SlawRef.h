
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_DETAIL_SLAWREF_H
#define OBLONG_PLASMA_DETAIL_SLAWREF_H


#include "RefCounted.h"

#include <libPlasma/c/slaw.h>

#include <libLoam/c++/LoamForward.h>
#include <libPlasma/c++/PlasmaForward.h>



namespace oblong {
namespace plasma {


/** Convenience null slaw values */
extern OB_PLASMAXX_API const slaw NULL_SLAW;
extern OB_PLASMAXX_API const bslaw NULL_BSLAW;


namespace detail {


/**
 * Self-managed slaw instance
 */
class OB_PLASMAXX_API SlawRef
{
 public:
  static SlawRef NilRef ();

 public:
  SlawRef ();
  explicit SlawRef (slaw s);
  SlawRef (bslaw s, SlawRef parent);

  operator bslaw () const { return slaw_; }

  void SwapIfNull (SlawRef &other);

  bool IsNull () const { return slaw_ == NULL; }
  bool operator== (const SlawRef &other) const
  {
    return slawx_equal (slaw_, other.slaw_);
  }
  bool operator!= (const SlawRef &other) const
  {
    return !slawx_equal (slaw_, other.slaw_);
  }
  bool Equals (bslaw s) const { return slawx_equal (slaw_, s); }

  bool IsArray () const { return slaw_is_numeric_array (slaw_); }
  bool IsBoolean () const { return slaw_is_boolean (slaw_); }
  bool IsCons () const { return slaw_is_cons (slaw_); }
  bool IsList () const { return slaw_is_list (slaw_); }
  bool IsMap () const { return slaw_is_map (slaw_); }
  bool IsProtein () const { return slaw_is_protein (slaw_); }
  bool IsAtomic () const;

  loam::Str ToStr () const;
  void Spew (OStreamReference os) const;

 private:
  class slaw_traits
  {
   public:
    static slaw Default () { return NULL; }
    static void Destroy (slaw s)
    {
      if (s)
        slaw_free (s);
    }
  };

  typedef RefCounted<slaw, slaw_traits> slaw_ref;

  static const slaw_ref NULL_REF;

  slaw_ref parent_;
  bslaw slaw_;
};
}
}
}  // namespace oblong::plasma::detail


#endif  // OBLONG_PLASMA_DETAIL_SLAWREF_H
