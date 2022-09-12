
/* (c)  oblong industries */

#ifndef OH_FUNCTIONARY_SNEAKERY
#define OH_FUNCTIONARY_SNEAKERY



namespace OH_FUNCTIONARY {

template <bool IS_POINTY, bool TO_BE_WRAPPED, typename ELEMT, typename ACCT,
          typename PEEST, typename ARGT>
struct Videlicet___
{
  static inline ACCT access (PEEST &el) { return el; }
  static inline ELEMT nullret () { return ELEMT (); }
  static inline void assign (PEEST &p, ARGT el) { new (&p) PEEST (el); }
};

template <typename ELEMT, typename ACCT, typename PEEST, typename ARGT>
struct Videlicet___<true, true, ELEMT, ACCT, PEEST, ARGT>
{
  static inline ACCT access (PEEST &el) { return ~el; }
  static inline ELEMT nullret () { return NULL; }
  static inline void assign (PEEST &p, ARGT el) { new (&p) PEEST (el); }
  static inline void __OBERR_Cannot_Use_CompactNulls_With_Non_Pointer_Types__ ()
  {
  }
};

template <typename ELEMT, typename ACCT, typename PEEST, typename ARGT>
struct Videlicet___<true, false, ELEMT, ACCT, PEEST, ARGT>
{
  static inline ACCT access (PEEST &el) { return el; }
  static inline ELEMT nullret () { return NULL; }
  static inline void assign (PEEST &p, ARGT el) { new (&p) PEEST (el); }
  static inline void __OBERR_Cannot_Use_CompactNulls_With_Non_Pointer_Types__ ()
  {
  }
};

};  // end namespace OT_FUNCTIONARY



#endif
