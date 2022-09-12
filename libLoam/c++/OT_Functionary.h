
/* (c)  oblong industries */

#ifndef OT_FUNCTIONARY_BELEAGUERINGS
#define OT_FUNCTIONARY_BELEAGUERINGS



namespace OT_FUNCTIONARY {

//Operation for objects with constructors
template <bool IS_POINTY, bool TO_BE_WRAPPED, bool IS_CONSTRUCTORLESS,
          typename ELEMT, typename ACCT, typename PEEST, typename ARGT>
struct Videlicet___
{
  static inline ACCT access (PEEST &el) { return el; }
  static inline ELEMT nullret () noexcept (noexcept (ELEMT ()))
  {
    return ELEMT ();
  }
  static inline void assign (PEEST *p, int64 ind, ARGT el)
  {
    // p[ind] = el;  FIX: optimize (here and other places) by doing
    //                    the faster/simpler thing for POD types (but
    //                    only for POD types)
    new (p + ind) PEEST (el);
  }
  static inline PEEST *expand (PEEST *p, int64 n_p, int64 newcap)
  {
    PEEST *noo = (PEEST *) malloc (newcap * sizeof (PEEST));
    if (noo)
      {
        for (int64 i = 0; i < n_p; i++)
          {
            new (noo + i) PEEST (p[i]);
            p[i].~PEEST ();
          }
        free (p);
      }
    return noo;
  }
};

//Operatations for objects without constructors
template <typename ELEMT, typename ACCT, typename PEEST, typename ARGT>
struct Videlicet___<false, false, true, ELEMT, ACCT, PEEST, ARGT>
{
  static inline ACCT access (PEEST &el) { return el; }
  static inline ELEMT nullret () noexcept (noexcept (ELEMT ()))
  {
    ELEMT x;
    memset (&x, 0, sizeof (x));
    return x;
  }
  static inline void assign (PEEST *p, int64 ind, ARGT el)
  {
    // p[ind] = el;  FIX: optimize (here and other places) by doing
    //                    the faster/simpler thing for POD types (but
    //                    only for POD types)
    new (p + ind) PEEST (el);
  }
  static inline PEEST *expand (PEEST *p, int64 n_p, int64 newcap)
  {
    PEEST *noo = (PEEST *) malloc (newcap * sizeof (PEEST));
    if (noo)
      {
        for (int64 i = 0; i < n_p; i++)
          {
            new (noo + i) PEEST (p[i]);
            p[i].~PEEST ();
          }
        free (p);
      }
    return noo;
  }
};

//Operations for pointers wrapped in ObRef
template <bool IS_CONSTRUCTORLESS, typename ELEMT, typename ACCT,
          typename PEEST, typename ARGT>
struct Videlicet___<true, true, IS_CONSTRUCTORLESS, ELEMT, ACCT, PEEST, ARGT>
{
  static inline ACCT access (PEEST &el) { return ~(el); }
  static inline ELEMT nullret () noexcept { return NULL; }
  static inline void assign (PEEST *p, int64 ind, ARGT el)
  {
    new (p + ind) PEEST (el);
  }
  // For intrepid code reviewers, the following implementation is OK
  // as ObRef was written with the following in mind; it only holds
  // onto (non-self-referencing) pointers.
  static inline PEEST *expand (PEEST *p, OB_UNUSED int64 n_p, int64 newcap)
  {
    return (PEEST *) realloc (static_cast<void *> (p), newcap * sizeof (PEEST));
  }
  static inline void __OBERR_Cannot_Use_ObRef_With_Non_Pointer_Types__ () {}
  static inline void __OBERR_Cannot_Use_NullifyNth_With_Non_Pointer_Types__ ()
  {
  }
};

//Operations for bare pointers
template <bool IS_CONSTRUCTORLESS, typename ELEMT, typename ACCT,
          typename PEEST, typename ARGT>
struct Videlicet___<true, false, IS_CONSTRUCTORLESS, ELEMT, ACCT, PEEST, ARGT>
{
  static inline ACCT access (PEEST &el) { return el; }
  static inline ELEMT nullret () noexcept { return NULL; }
  static inline void assign (PEEST *p, int64 ind, ARGT el)
  {
    new (p + ind) PEEST (el);
  }
  static inline PEEST *expand (PEEST *p, OB_UNUSED int64 n_p, int64 newcap)
  {
    return (PEEST *) realloc (static_cast<void *> (p), newcap * sizeof (PEEST));
  }
  static inline void __OBERR_Cannot_Use_ObRef_With_Non_Pointer_Types__ () {}
  static inline void __OBERR_Cannot_Use_NullifyNth_With_Non_Pointer_Types__ ()
  {
  }
};

};  // end namespace OT_FUNCTIONARY



#endif
