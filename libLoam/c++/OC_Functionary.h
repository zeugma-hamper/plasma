
/* (c)  oblong industries */

#ifndef OC_FUNCTIONARY_BEWILDERMENTS
#define OC_FUNCTIONARY_BEWILDERMENTS



namespace OC_FUNCTIONARY {

template <bool CAR_TO_BE_WRAPPED, bool CDR_TO_BE_WRAPPED, typename CARTYP,
          typename CDRTYP, typename CARACCT, typename CDRACCT, typename CARST,
          typename CDRST>
struct Videlicet___
{
  static inline CARACCT caraccess (CARST &a) { return a; }
  static inline CDRACCT cdraccess (CDRST &d) { return d; }
  static inline CARTYP carnullret () { return CARTYP (); }
  static inline CDRTYP cdrnullret () { return CDRTYP (); }
};

template <bool CAR_TO_BE_WRAPPED, typename CARTYP, typename CDRTYP,
          typename CARACCT, typename CDRACCT, typename CARST, typename CDRST>
struct Videlicet___<CAR_TO_BE_WRAPPED, true, CARTYP, CDRTYP, CARACCT, CDRACCT,
                    CARST, CDRST>
{
  static inline CARACCT caraccess (CARST &a) { return a; }
  static inline CDRACCT cdraccess (const CDRST &d) { return ~(d); }
  static inline CARTYP carnullret () { return CARTYP (); }
  static inline CDRTYP cdrnullret () { return NULL; }
  static inline void __OBERR_Cannot_ObRef_Such_A_Cdr_Type__ () {}
};

template <bool CDR_TO_BE_WRAPPED, typename CARTYP, typename CDRTYP,
          typename CARACCT, typename CDRACCT, typename CARST, typename CDRST>
struct Videlicet___<true, CDR_TO_BE_WRAPPED, CARTYP, CDRTYP, CARACCT, CDRACCT,
                    CARST, CDRST>
{
  static inline CARACCT caraccess (const CARST &a) { return ~(a); }
  static inline CDRACCT cdraccess (CDRST &d) { return d; }
  static inline CARTYP carnullret () { return NULL; }
  static inline CDRTYP cdrnullret () { return CDRTYP (); }
  static inline void __OBERR_Cannot_ObRef_Such_A_Car_Type__ () {}
};

template <typename CARTYP, typename CDRTYP, typename CARACCT, typename CDRACCT,
          typename CARST, typename CDRST>
struct Videlicet___<true, true, CARTYP, CDRTYP, CARACCT, CDRACCT, CARST, CDRST>
{
  static inline CARACCT caraccess (const CARST &a) { return ~(a); }
  static inline CDRACCT cdraccess (const CDRST &d) { return ~(d); }
  static inline CARTYP carnullret () { return NULL; }
  static inline CDRTYP cdrnullret () { return NULL; }
  static inline void __OBERR_Cannot_ObRef_Such_A_Car_Type__ () {}
  static inline void __OBERR_Cannot_ObRef_Such_A_Cdr_Type__ () {}
};

};  // end namespace OC_FUNCTIONARY



#endif
