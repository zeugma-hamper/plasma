
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_DETAIL_COMPOSITESLAW_H
#define OBLONG_PLASMA_DETAIL_COMPOSITESLAW_H


#include "SlawRef.h"
#include "slaw-traits.h"

#include <vector>


namespace oblong {
namespace plasma {
namespace detail {


typedef std::vector<SlawRef> SlawRefs;
typedef SlawRefs::iterator SlawIter;
typedef SlawRefs::const_iterator ConstSlawIter;
typedef SlawRefs::size_type SRSize;

class OB_PLASMAXX_API CompositeSlaw
{
 public:
  class OB_PLASMAXX_API RefTraits
  {
   public:
    static CompositeSlaw *Default ();
    static void Destroy (CompositeSlaw *);
  };

  typedef RefCounted<CompositeSlaw *, RefTraits> Ref;

 public:
  // Factory functions
  static Ref Make (SlawRef);
  static Ref Cons (SlawRef, SlawRef);
  static Ref List (const SlawRefs &);
  static Ref Map (const SlawRefs &);
  static Ref Map (Ref, Ref);
  static Ref ExtendMap (Ref, SlawRef, SlawRef);
  static Ref Protein (Ref, Ref);

 public:
  virtual ~CompositeSlaw () = 0;

  virtual bool IsNull () const = 0;
  virtual bool IsCons () const { return false; }
  virtual bool IsList () const { return false; }
  virtual bool IsMap () const { return false; }
  virtual bool IsProtein () const { return false; }
  virtual bool IsAtomic () const { return false; }

  virtual SlawRef AsSlaw () const = 0;

  virtual bool IsEqual (const CompositeSlaw *other) const = 0;

  virtual int64 Count () const = 0;
  virtual SlawRef Nth (int64 idx) const = 0;
  virtual int64 IndexOf (bslaw s, unt64 start) const = 0;
  virtual SlawRefs Slice (int64 begin, int64 end) const = 0;
  virtual SlawRefs KVList () const = 0;
  virtual unt64 KeyCount () const = 0;

  virtual SlawRef Find (bslaw key) const = 0;

  virtual Str ToStr () const = 0;
  virtual void Spew (OStreamReference os) const = 0;

  virtual Ref Descrips () const;
  virtual Ref Ingests () const;
  virtual SlawRefs DescripList () const = 0;
  virtual SlawRefs IngestList () const = 0;

  bool Equals (const CompositeSlaw *other) const;

  SlawRefs Components () const;

  template <typename T>
  bool CanEmit () const;
};

// Utilites
OB_HIDDEN Str RefToStr (const SlawRef &r);

// Emit checks
template <typename T>
struct EmitCheck
{
  static bool CanEmit (const CompositeSlaw &cs)
  {
    if (!cs.IsAtomic ())
      return false;
    T canEmitdummy;
    return slaw_traits<T>::Coerce (cs.Nth (0), canEmitdummy);
  }
};

#define DEFINE_VEMIT_TRAITS(N, T)                                              \
  template <>                                                                  \
  struct EmitCheck<v##N##T>                                                    \
  {                                                                            \
    static bool CanEmit (const CompositeSlaw &s)                               \
    {                                                                          \
      if (s.Count () != N)                                                     \
        return false;                                                          \
      T v;                                                                     \
      for (int64 i = 0; i < N; ++i)                                            \
        if (!slaw_traits<T>::Coerce (s.Nth (i), v))                            \
          return false;                                                        \
      return true;                                                             \
    }                                                                          \
  };

#define DEFINE_VEMIT_TRAITS_T(N)                                               \
  DEFINE_VEMIT_TRAITS (N, int8);                                               \
  DEFINE_VEMIT_TRAITS (N, int16);                                              \
  DEFINE_VEMIT_TRAITS (N, int32);                                              \
  DEFINE_VEMIT_TRAITS (N, int64);                                              \
  DEFINE_VEMIT_TRAITS (N, unt8);                                               \
  DEFINE_VEMIT_TRAITS (N, unt16);                                              \
  DEFINE_VEMIT_TRAITS (N, unt32);                                              \
  DEFINE_VEMIT_TRAITS (N, unt64);                                              \
  DEFINE_VEMIT_TRAITS (N, float32);                                            \
  DEFINE_VEMIT_TRAITS (N, float64)

DEFINE_VEMIT_TRAITS_T (2);
DEFINE_VEMIT_TRAITS_T (3);
DEFINE_VEMIT_TRAITS_T (4);

#undef DEFINE_VEMIT_TRAITS_T
#undef DEFINE_VEMIT_TRAITS

template <>
struct EmitCheck<Vect>
{
  static bool CanEmit (const CompositeSlaw &s)
  {
    return EmitCheck<v3float64>::CanEmit (s);
  }
};

template <>
struct EmitCheck<Vect4>
{
  static bool CanEmit (const CompositeSlaw &s)
  {
    return EmitCheck<v4float64>::CanEmit (s);
  }
};

template <>
struct EmitCheck<ObTrove<Str>>
{
  static bool CanEmit (const CompositeSlaw &cs)
  {
    if (!cs.IsList ())
      return false;
    const int64 cnt = cs.Count ();
    for (int64 i = 0; i < cnt; i++)
      if (!slaw_is_string (cs.Nth (i)))
        return false;
    return true;
  }
};

template <>
struct EmitCheck<Matrix44>
{
  static bool CanEmit (const CompositeSlaw &cs)
  {
    if (cs.IsAtomic ())
      {
        Matrix44 canEmitdummy;
        return slaw_traits<Matrix44>::Coerce (cs.Nth (0), canEmitdummy);
      }
    if (!cs.IsList ())
      return false;
    const int64 cnt = cs.Count ();
    if (cnt == 4 || cnt == 16)
      {
        Matrix44 canEmitdummy;
        return slaw_traits<Matrix44>::Coerce (cs.AsSlaw (), canEmitdummy);
      }
    return false;
  }
};

template <typename T>
bool CompositeSlaw::CanEmit () const
{
  return EmitCheck<T>::CanEmit (*this);
}
}
}
}  // namespace oblong::plasma::detail


#endif  // OBLONG_PLASMA_DETAIL_COMPOSITESLAW_H
