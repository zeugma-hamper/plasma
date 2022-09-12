
/* (c)  oblong industries */

#ifndef OM_FUNCTIONARY_SMATTERINGS
#define OM_FUNCTIONARY_SMATTERINGS



namespace OM_FUNCTIONARY {

template <bool TO_BE_WRAPPED, bool IS_CONSTRUCTORLESS, typename T>
struct NullaryKnowHow
{
  static inline T nullret () { return T (); }
};

template <bool IS_CONSTRUCTORLESS, typename T>
struct NullaryKnowHow<true, IS_CONSTRUCTORLESS, T>
{
  static inline T nullret () { return NULL; }
};

template <typename T>
struct NullaryKnowHow<false, true, T>
{
  static inline T nullret ()
  {
    T x;
    memset (&x, 0, sizeof (x));
    return x;
  }
};

template <bool KEY_TO_BE_WRAPPED, bool VAL_TO_BE_WRAPPED,
          bool KEY_IS_CONSTRUCTORLESS, bool VAL_IS_CONSTRUCTORLESS,
          typename KEYT, typename VALT>
struct Videlicet___
{
  typedef NullaryKnowHow<KEY_TO_BE_WRAPPED, KEY_IS_CONSTRUCTORLESS, KEYT> Key;
  typedef NullaryKnowHow<VAL_TO_BE_WRAPPED, VAL_IS_CONSTRUCTORLESS, VALT> Val;

  static inline KEYT keynullret () { return Key::nullret (); }
  static inline VALT valnullret () { return Val::nullret (); }
};
};  // end namespace OM_FUNCTIONARY



#endif
