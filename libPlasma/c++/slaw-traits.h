
/* (c)  oblong industries */

#ifndef OBLONG_PLASMA_SLAW_TRAITS_H
#define OBLONG_PLASMA_SLAW_TRAITS_H


#include <libPlasma/c/slaw.h>
#include <libPlasma/c/slaw-coerce.h>

#include <libLoam/c++/ObRetort.h>
#include <libLoam/c++/ObTrove.h>
#include <libLoam/c++/Vect.h>
#include <libLoam/c++/Vect4.h>
#include <libLoam/c++/Quat.h>
#include <libLoam/c++/Matrix44.h>
#include <libLoam/c++/Str.h>
#include <libLoam/c++/ObColor.h>
#include <libPlasma/c++/PlasmaForward.h>

#include <vector>
#include <limits>


#define DEFINE_UNEQU(V)                                                        \
  inline bool operator!= (const V &l, const V &r) { return !(l == r); }

#define DEFINE_V2_EQU(T)                                                       \
  inline bool operator== (const v2##T &l, const v2##T &r)                      \
  {                                                                            \
    return l.x == r.x && l.y == r.y;                                           \
  }                                                                            \
  DEFINE_UNEQU (v2##T)

#define DEFINE_V3_EQU(T)                                                       \
  inline bool operator== (const v3##T &l, const v3##T &r)                      \
  {                                                                            \
    return l.x == r.x && l.y == r.y && l.z == r.z;                             \
  }                                                                            \
  DEFINE_UNEQU (v3##T)

#define DEFINE_V4_EQU(T)                                                       \
  inline bool operator== (const v4##T &l, const v4##T &r)                      \
  {                                                                            \
    return l.x == r.x && l.y == r.y && l.z == r.z && l.w == r.w;               \
  }                                                                            \
  DEFINE_UNEQU (v4##T)

#define DEFINE_V_EQU(T)                                                        \
  DEFINE_V2_EQU (T)                                                            \
  DEFINE_V3_EQU (T)                                                            \
  DEFINE_V4_EQU (T)

OB_FOR_ALL_NUMERIC_TYPES (DEFINE_V_EQU);

#undef DEFINE_V_EQU
#undef DEFINE_V4_EQU
#undef DEFINE_V3_EQU
#undef DEFINE_V2_EQU
#undef DEFINE_UNEQU


namespace oblong {
namespace plasma {
using namespace loam;

// convenience constructors
#define DEFINE_VCONSTRUCTORS(T)                                                \
  inline v2##T make_v2##T (T x, T y)                                           \
  {                                                                            \
    v2##T v;                                                                   \
    v.x = x;                                                                   \
    v.y = y;                                                                   \
    return v;                                                                  \
  }                                                                            \
                                                                               \
  inline v3##T make_v3##T (T x, T y, T z)                                      \
  {                                                                            \
    v3##T v;                                                                   \
    v.x = x;                                                                   \
    v.y = y;                                                                   \
    v.z = z;                                                                   \
    return v;                                                                  \
  }                                                                            \
                                                                               \
  inline v4##T make_v4##T (T x, T y, T z, T w)                                 \
  {                                                                            \
    v4##T v;                                                                   \
    v.x = x;                                                                   \
    v.y = y;                                                                   \
    v.z = z;                                                                   \
    v.w = w;                                                                   \
    return v;                                                                  \
  }

OB_FOR_ALL_NUMERIC_TYPES (DEFINE_VCONSTRUCTORS);

#undef DEFINE_VCONSTRUCTORS

/**
 * Type information for slawx wrapping values of type T.
 * Specializations of this template for a given type @a T provide the
 * required functions for conversions and type checking between slaw
 * and @a T values.  The default template definition provides no
 * definitions, to avoid undesired instantiations.
 * @ingroup PlasmaTypesPlumbing
 */
template <typename T>
struct slaw_traits
{
  /**
   * Return a default value for initializing slawx of type @a T. For
   * numerical T types, this is typically @c T(0).
   */
  // static T Null ();
  /**
   * Check whether @a s is of type @c T (typically using C-level
   * functions like, e.g., slaw_is_string()).
   */
  // static bool TypeP (bslaw s);
  /**
   * Try to coerce the given slaw to a value of type T. The return
   * value indicates whether the coercion was possible.
   */
  // static bool Coerce (bslaw s, T &v);
  /**
   * Create a new slaw with the provided type and value.
   */
  // static slaw Make (T v);
};

/**
 * Uses slaw_traits to cast a slaw to a value whose traits are
 * defined. Just syntactic sugar for @c slaw_traits<T>::Cast(s).
 * @ingroup PlasmaTypesPlumbing
 */
template <typename T>
T slaw_cast (bslaw s)
{
  T value;
  return slaw_traits<T>::Coerce (s, value) ? value : slaw_traits<T>::Null ();
}

/**
 * Convenience slaw creation overloads. Makes it a bit easier to
 * create new slawx:
 * @code
 *   const char* foo = bar ();
 *   // ...
 *   slaw s = make_slaw (foo);  // instead of slaw_int8 ();
 *   slaw f = make_slaw (get_a_float64 ()); // instead of slaw_string ();
 * @endcode
 * When using literal values, it might be advisable to be type-wise
 * explicit:
 * @code
 *    slaw n = make_slaw (int64 (42));
 *    assert (slaw_cast<int64> (n) == 42);
 *    // or, equivalenty:
 *    slaw m = make_slaw<int64> (42);
 *    assert (slaw_cast<int64> (m) == 42);
 * @endcode
 * @ingroup PlasmaTypesPlumbing
 */
template <typename T>
inline slaw make_slaw (T v)
{
  return slaw_traits<T>::Make (v);
}

// Traits specialisations

// - Numeric and v-types

#define DEFINE_SLAW_TRAITS(T, N)                                               \
  template <>                                                                  \
  class slaw_traits<T>                                                         \
  {                                                                            \
   public:                                                                     \
    typedef T type;                                                            \
    static type Null ()                                                        \
    {                                                                          \
      type value = N;                                                          \
      return value;                                                            \
    }                                                                          \
    static bool TypeP (bslaw s) { return slaw_is_##T (s); }                    \
    static bool Coerce (bslaw s, T &value)                                     \
    {                                                                          \
      if (TypeP (s))                                                           \
        {                                                                      \
          value = *slaw_##T##_emit (s);                                        \
          return true;                                                         \
        }                                                                      \
      return slaw_to_##T (s, &value) == OB_OK;                                 \
    }                                                                          \
    static slaw Make (type v) { return slaw_##T (v); }                         \
  }

#define OBLONG_PLASMA_Z2                                                       \
  {                                                                            \
    0, 0                                                                       \
  }
#define OBLONG_PLASMA_Z3                                                       \
  {                                                                            \
    0, 0, 0                                                                    \
  }
#define OBLONG_PLASMA_Z4                                                       \
  {                                                                            \
    0, 0, 0, 0                                                                 \
  }

#define DEFINE_SLAW_TRAITS_T(T)                                                \
  DEFINE_SLAW_TRAITS (T, 0);                                                   \
  DEFINE_SLAW_TRAITS (v2##T, OBLONG_PLASMA_Z2);                                \
  DEFINE_SLAW_TRAITS (v3##T, OBLONG_PLASMA_Z3);                                \
  DEFINE_SLAW_TRAITS (v4##T, OBLONG_PLASMA_Z4)

//OB_FOR_ALL_NUMERIC_TYPES (DEFINE_SLAW_TRAITS_T);
/* Below we expand the OB_FOR_ALL_NUMERIC_TYPES macro to accommodate
certain old versions of Doxygen (e.g. 1.5.5 that "goes with" Ubuntu
8.04). Such versions would otherwise choke on the very long lines that
would result. */
DEFINE_SLAW_TRAITS_T (int8);
DEFINE_SLAW_TRAITS_T (int16);
DEFINE_SLAW_TRAITS_T (int32);
DEFINE_SLAW_TRAITS_T (int64);
DEFINE_SLAW_TRAITS_T (unt8);
DEFINE_SLAW_TRAITS_T (unt16);
DEFINE_SLAW_TRAITS_T (unt32);
DEFINE_SLAW_TRAITS_T (unt64);
DEFINE_SLAW_TRAITS_T (float32);
DEFINE_SLAW_TRAITS_T (float64);

#undef DEFINE_SLAW_TRAITS
#undef OBLONG_PLASMA_Z2
#undef OBLONG_PLASMA_Z3
#undef OBLONG_PLASMA_Z4
#undef DEFINE_SLAW_TRAITS_T

// - Arrays

#define DEFINE_ARRAY_TRAITS_T(T)                                               \
  template <>                                                                  \
  class slaw_traits<const T *>                                                 \
  {                                                                            \
   public:                                                                     \
    typedef const T *type;                                                     \
    static type Null () { return NULL; }                                       \
    static bool TypeP (bslaw s) { return slaw_is_##T##_array (s); }            \
    static bool Coerce (bslaw s, type &value)                                  \
    {                                                                          \
      if (TypeP (s))                                                           \
        {                                                                      \
          value = slaw_##T##_array_emit (s);                                   \
          return true;                                                         \
        }                                                                      \
      T *v = NULL;                                                             \
      if (slaw_to_##T (s, v) == OB_OK)                                         \
        {                                                                      \
          value = v;                                                           \
          return true;                                                         \
        }                                                                      \
      return false;                                                            \
    }                                                                          \
    static slaw Make (type v, int64 c = 1) { return slaw_##T##_array (v, c); } \
  }

#define DEFINE_ARRAY_TRAITS(T)                                                 \
  DEFINE_ARRAY_TRAITS_T (T);                                                   \
  DEFINE_ARRAY_TRAITS_T (v2##T);                                               \
  DEFINE_ARRAY_TRAITS_T (v3##T);                                               \
  DEFINE_ARRAY_TRAITS_T (v4##T)

//OB_FOR_ALL_NUMERIC_TYPES(DEFINE_ARRAY_TRAITS);
/* Below we expand the OB_FOR_ALL_NUMERIC_TYPES macro to accommodate
certain old versions of Doxygen (e.g. 1.5.5 that "goes with" Ubuntu
8.04). Such versions would otherwise choke on the very long lines that
would result. */
DEFINE_ARRAY_TRAITS (int8);
DEFINE_ARRAY_TRAITS (int16);
DEFINE_ARRAY_TRAITS (int32);
DEFINE_ARRAY_TRAITS (int64);
DEFINE_ARRAY_TRAITS (unt8);
DEFINE_ARRAY_TRAITS (unt16);
DEFINE_ARRAY_TRAITS (unt32);
DEFINE_ARRAY_TRAITS (unt64);
DEFINE_ARRAY_TRAITS (float32);
DEFINE_ARRAY_TRAITS (float64);

#undef DEFINE_ARRAY_TRAITS

#define DEFINE_WRAPPED_ARRAY_TRAITS_T(T)                                       \
  template <>                                                                  \
  class slaw_traits<std::vector<T>>                                            \
  {                                                                            \
   public:                                                                     \
    typedef std::vector<T> type;                                               \
    static type Null () { return type (); }                                    \
    static bool TypeP (bslaw s) { return slaw_is_##T##_array (s); }            \
    static bool Coerce (bslaw s, type &value)                                  \
    {                                                                          \
      const int64 breadth = slaw_numeric_array_count (s);                      \
      if (breadth > 0)                                                         \
        {                                                                      \
          const T *data = slaw_##T##_array_emit (s);                           \
          value = data ? type (data, data + breadth) : type ();                \
          return true;                                                         \
        }                                                                      \
      return false;                                                            \
    }                                                                          \
    static slaw Make (type v)                                                  \
    {                                                                          \
      return slaw_##T##_array (&(v[0]), v.size ());                            \
    }                                                                          \
  };                                                                           \
                                                                               \
  template <>                                                                  \
  class slaw_traits<ObTrove<T>>                                                \
  {                                                                            \
   public:                                                                     \
    typedef ObTrove<T> type;                                                   \
    static type Null () { return type (); }                                    \
    static bool TypeP (bslaw s) { return slaw_is_##T##_array (s); }            \
    static bool Coerce (bslaw s, type &value)                                  \
    {                                                                          \
      const int64 breadth = slaw_numeric_array_count (s);                      \
      if (breadth > 0)                                                         \
        {                                                                      \
          const T *data = slaw_##T##_array_emit (s);                           \
          value = data ? type (data, breadth) : type ();                       \
          return true;                                                         \
        }                                                                      \
      return false;                                                            \
    }                                                                          \
    static slaw Make (type v)                                                  \
    {                                                                          \
      return slaw_##T##_array (v._NaughtyXMXPVMFLNF_Pees_ForSlawTraits (),     \
                               v.Count ());                                    \
    }                                                                          \
  }

#define DEFINE_WRAPPED_ARRAY_TRAITS(T)                                         \
  DEFINE_WRAPPED_ARRAY_TRAITS_T (T);                                           \
  DEFINE_WRAPPED_ARRAY_TRAITS_T (v2##T);                                       \
  DEFINE_WRAPPED_ARRAY_TRAITS_T (v3##T);                                       \
  DEFINE_WRAPPED_ARRAY_TRAITS_T (v4##T)


template <>
class slaw_traits<ObTrove<Vect>>
{
 public:
  typedef ObTrove<Vect> type;
  static type Null () { return type (); }
  static bool TypeP (bslaw s) { return slaw_is_v3float64_array (s); }
  static bool Coerce (bslaw s, type &value)
  {
    const int64 breadth = slaw_numeric_array_count (s);
    if (breadth > 0)
      {
        const Vect *data = (const Vect *) slaw_v3float64_array_emit (s);
        value = data ? type (data, breadth) : type ();
        return true;
      }
    return false;
  }
  static slaw Make (type v)
  {
    return slaw_v3float64_array (v._NaughtyXMXPVMFLNF_Pees_ForSlawTraits (),
                                 v.Count ());
  }
};



//OB_FOR_ALL_NUMERIC_TYPES(DEFINE_WRAPPED_ARRAY_TRAITS);
/* Below we expand the OB_FOR_ALL_NUMERIC_TYPES macro to accommodate
certain old versions of Doxygen (e.g. 1.5.5 that "goes with" Ubuntu
8.04). Such versions would otherwise choke on the very long lines that
would result. */
DEFINE_WRAPPED_ARRAY_TRAITS (int8);
DEFINE_WRAPPED_ARRAY_TRAITS (int16);
DEFINE_WRAPPED_ARRAY_TRAITS (int32);
DEFINE_WRAPPED_ARRAY_TRAITS (int64);
DEFINE_WRAPPED_ARRAY_TRAITS (unt8);
DEFINE_WRAPPED_ARRAY_TRAITS (unt16);
DEFINE_WRAPPED_ARRAY_TRAITS (unt32);
DEFINE_WRAPPED_ARRAY_TRAITS (unt64);
DEFINE_WRAPPED_ARRAY_TRAITS (float32);
DEFINE_WRAPPED_ARRAY_TRAITS (float64);

#undef DEFINE_WRAPPED_ARRAY_TRAITS

// - Additional C++ numeric types (literal integers)

#ifndef OB_INT64_IS_LONG
template <>
class slaw_traits<unsigned long>
{
 public:
  typedef unsigned long type;
  static const bool is_wide = (std::numeric_limits<unsigned long>::digits) > 32;
  static type Null () { return 0; }
  static bool TypeP (bslaw s)
  {
    return is_wide ? slaw_is_unt64 (s) : slaw_is_unt32 (s);
  }
  static bool Coerce (bslaw s, type &value)
  {
    unt64 v;
    bool result = false;
    if (is_wide)
      {
        result = slaw_traits<unt64>::Coerce (s, v);
      }
    else
      {
        unt32 vv;
        result = slaw_traits<unt32>::Coerce (s, vv);
        v = vv;
      }
    if (result)
      value = static_cast<type> (v);
    return result;
  }
  static slaw Make (type v)
  {
    return is_wide ? slaw_unt64 (v) : slaw_unt32 (v);
  }
};

template <>
class slaw_traits<long int>
{
 public:
  typedef long int type;
  static const bool is_wide = (std::numeric_limits<long int>::digits) > 32;
  static type Null () { return 0; }
  static bool TypeP (bslaw s)
  {
    return is_wide ? slaw_is_int64 (s) : slaw_is_int32 (s);
  }
  static bool Coerce (bslaw s, type &value)
  {
    int64 v;
    bool result = false;
    if (is_wide)
      {
        result = slaw_traits<int64>::Coerce (s, v);
      }
    else
      {
        int32 vv;
        result = slaw_traits<int32>::Coerce (s, vv);
        v = vv;
      }
    if (result)
      value = static_cast<type> (v);
    return result;
  }
  static slaw Make (type v)
  {
    return is_wide ? slaw_int64 (v) : slaw_int32 (v);
  }
};
#endif

// - Booleans

template <>
class slaw_traits<bool>
{
 public:
  typedef bool type;
  static type Null () { return false; }
  static bool TypeP (bslaw s) { return slaw_is_boolean (s); }
  static bool Coerce (bslaw s, type &value)
  {
    if (TypeP (s))
      {
        value = *slaw_boolean_emit (s);
        return true;
      }
    return (slaw_to_boolean (s, &value) == OB_OK);
  }
  static slaw Make (type s) { return slaw_boolean (s); }
};

// - Strings

template <>
class slaw_traits<const char *>
{
 public:
  typedef const char *type;
  static type Null () { return NULL; }
  static bool TypeP (bslaw s) { return slaw_is_string (s); }
  static bool Coerce (bslaw s, type &value)
  {
    if (TypeP (s))
      {
        value = slaw_string_emit (s);
        return true;
      }
    return false;
  }
  static slaw Make (type s) { return slaw_string (s ? s : ""); }
};

template <>
class slaw_traits<Str>
{
 public:
  typedef Str type;
  static type Null () { return Str (); }
  static bool TypeP (bslaw s) { return slaw_is_string (s); }
  static bool Coerce (bslaw s, type &value)
  {
    if (TypeP (s))
      {
        value.Set (slaw_string_emit (s));
        return true;
      }
    return false;
  }
  static slaw Make (const type &s) { return slaw_string (s.utf8 ()); }
};

template <>
class slaw_traits<ObTrove<Str>>
{
 public:
  typedef ObTrove<Str> type;
  static type Null () { return type (); }
  static bool TypeP (bslaw s)
  {
    if (!slaw_is_list (s))
      return false;
    for (bslaw cole = slaw_list_emit_first (s); cole != NULL;
         cole = slaw_list_emit_next (s, cole))
      if (!slaw_is_string (cole))
        return false;
    return true;
  }
  static bool Coerce (bslaw s, type &value)
  {
    if (TypeP (s))
      {
        value.Empty ();
        value.EnsureRoomFor (slaw_list_count (s));
        for (bslaw cole = slaw_list_emit_first (s); cole != NULL;
             cole = slaw_list_emit_next (s, cole))
          value.Append (Str (slaw_string_emit (cole)));
        return true;
      }
    return false;
  }
  static slaw Make (const type &s)
  {
    slabu *sb = slabu_new ();
    const int64 cnt = s.Count ();
    for (int64 i = 0; i < cnt; i++)
      slabu_list_add_c (sb, s.Nth (i).utf8 ());
    return slaw_list_f (sb);
  }
};

// - Vect and Vect4

template <>
class slaw_traits<Vect>
{
 public:
  typedef Vect type;
  static type Null () { return type (); }
  static bool TypeP (bslaw s)
  {
    return (slaw_is_v3float64 (s) || (slaw_is_float64_array (s)
                                      && slaw_numeric_array_count (s) == 3));
  }
  static bool Coerce (bslaw s, type &value)
  {
    v3float64 v;
    bool result = slaw_traits<v3float64>::Coerce (s, v);
    if (result)
      value = type (v);
    return result;
  }
  static slaw Make (const type &s)
  {
    v3float64 v = {s.x, s.y, s.z};
    return slaw_v3float64 (v);
  }
};

template <>
class slaw_traits<Vect4>
{
 public:
  typedef Vect4 type;
  static type Null () { return type (); }
  static bool TypeP (bslaw s)
  {
    return (slaw_is_v4float64 (s) || (slaw_is_float64_array (s)
                                      && slaw_numeric_array_count (s) == 4));
  }
  static bool Coerce (bslaw s, type &value)
  {
    v4float64 v;
    bool result = slaw_traits<v4float64>::Coerce (s, v);
    if (result)
      value = type (v);
    return result;
  }
  static slaw Make (const type &s)
  {
    v4float64 v = {s.x, s.y, s.z, s.w};
    return slaw_v4float64 (v);
  }
};


template <>
class slaw_traits<ObColor>
{
 public:
  typedef ObColor type;

  static type Null () { return type (); }

  static bool TypeP (bslaw s)
  {
    float64 v[4];
    int len;
    ObRetort err = slaw_to_vn (s, v, 4, &len);
    return (err == OB_OK);
  }

  static bool Coerce (bslaw s, type &value)
  {
    float64 v[4];
    int len;

    ObRetort err = slaw_to_vn (s, v, 4, &len);
    if (err != OB_OK)
      return false;

    if (len == 4)
      value.Set (v[0], v[1], v[2], v[3]);
    else if (len == 3)
      value.Set (v[0], v[1], v[2], 1.0);
    else if (len == 2)
      value.Set (v[0], v[0], v[0], v[1]);
    else if (len == 1)
      value.Set (v[0], v[0], v[0], 1.0);
    return true;
  }

  static slaw Make (const type &s)
  {
    v4float64 v = {s.r, s.g, s.b, s.a};
    return slaw_v4float64 (v);
  }
};


// - Quat and Matrix44


template <>
class slaw_traits<Quat>
{
 public:
  typedef Quat type;

  static type Null () { return type (); }

  static bool TypeP (bslaw s)
  {
    return (slaw_is_v4float64 (s) || (slaw_is_float64_array (s)
                                      && slaw_numeric_array_count (s) == 4));
  }

  static bool Coerce (bslaw s, type &value)
  {
    v4float64 v;
    bool result = slaw_traits<v4float64>::Coerce (s, v);
    if (result)
      value = type (v);
    return result;
  }

  static slaw Make (const type &s) { return slaw_v4float64 (s); }
};


template <>
class slaw_traits<Matrix44>
{
 public:
  typedef Matrix44 type;

  static const int FMT_NOT_A_MATRIX = 0;
  static const int FMT_v4float64_ARRAY = 1;
  static const int FMT_float64_ARRAY = 2;
  static const int FMT_v4float64_LIST = 3;
  static const int FMT_float64_LIST = 4;
  static const int FMT_float64_ARRAY_LIST = 5;

  static type Null () { return type (); }

  static int GetSlawFormat (bslaw s)
  {
    if (slaw_is_v4float64_array (s) && slaw_numeric_array_count (s) == 4)
      {
        return FMT_v4float64_ARRAY;
      }
    if (slaw_is_float64_array (s) && slaw_numeric_array_count (s) == 16)
      {
        return FMT_float64_ARRAY;
      }
    if (slaw_is_list (s))
      {
        if (slaw_list_count (s) == 16)
          {
            bool isFloat64 = true;
            for (bslaw v = slaw_list_emit_first (s); v != NULL;
                 v = slaw_list_emit_next (s, v))
              if (!slaw_is_float64 (v))
                {
                  isFloat64 = false;
                  break;
                }
            if (isFloat64)
              {
                return FMT_float64_LIST;
              }
          }
        else if (slaw_list_count (s) == 4)
          {
            bslaw v = slaw_list_emit_first (s);
            if (slaw_is_v4float64 (v))
              {
                bool isV4float64array = true;
                for (; v != NULL; v = slaw_list_emit_next (s, v))
                  if (!slaw_is_v4float64 (v))
                    {
                      isV4float64array = false;
                      break;
                    }
                if (isV4float64array)
                  {
                    return FMT_v4float64_LIST;
                  }
              }
            else if (slaw_is_float64_array (v)
                     && slaw_numeric_array_count (v) == 4)
              {
                bool isFloat64array = true;
                for (; v != NULL; v = slaw_list_emit_next (s, v))
                  if (!slaw_is_float64_array (v)
                      || slaw_numeric_array_count (v) != 4)
                    {
                      isFloat64array = false;
                      break;
                    }
                if (isFloat64array)
                  {
                    return FMT_float64_ARRAY_LIST;
                  }
              }
          }
      }
    return FMT_NOT_A_MATRIX;
  }

  static bool TypeP (bslaw s)
  {
    return (GetSlawFormat (s) != FMT_NOT_A_MATRIX);
  }

  static bool Coerce (bslaw s, type &value)
  {
    int fmt = GetSlawFormat (s);
    switch (fmt)
      {
        case FMT_NOT_A_MATRIX:
          {
            return false;
          }
        case FMT_v4float64_ARRAY:
          {
            const v4float64 *array = slaw_v4float64_array_emit (s);
            memcpy (value.m, array, sizeof (float64) * 16);
            return true;
          }
        case FMT_float64_ARRAY:
          {
            const float64 *array = slaw_float64_array_emit (s);
            memcpy (value.m, array, sizeof (float64) * 16);
            return true;
          }
        case FMT_v4float64_LIST:
          {
            bslaw v = NULL;
            for (int i = 0; i < 4; i++)
              {
                v = slaw_list_emit_nth (s, i);
                slaw_to_v4 (v, value.v_ + i);
              }
            return true;
          }
        case FMT_float64_LIST:
          {
            bslaw v = NULL;
            float64 *mat = value.f_;
            for (int i = 0; i < 16; i++)
              {
                v = slaw_list_emit_nth (s, i);
                slaw_to_float64 (v, mat + i);
              }
            return true;
          }
        case FMT_float64_ARRAY_LIST:
          {
            bslaw v = NULL;
            float64 *mat = value.f_;
            for (int i = 0; i < 4; i++)
              {
                v = slaw_list_emit_nth (s, i);
                const float64 *arr = slaw_float64_array_emit (v);
                memcpy (mat + i * 4, arr, sizeof (float64) * 4);
              }
            return true;
          }
      }
    return false;
  }

  static slaw Make (const type &s) { return slaw_v4float64_array (s.v_, 4); }
};
}
}  // namespace oblong::plasma



#endif  // OBLONG_PLASMA_SLAW_TRAITS_H
