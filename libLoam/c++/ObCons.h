
/* (c)  oblong industries */

#ifndef OB_CONS_TIPATION
#define OB_CONS_TIPATION


#include <libLoam/c++/ObRef.h>


namespace oblong {
namespace loam {


//
// the following dwells here and not in some more logical place as
// is not possible to have partial specializations inside the class
// scope, where they'd actually make much more sense. great!
//
#include "OC_Functionary.h"

/**
 * ObCons is similar in spirit to std::pair, but with lispy nomenclature,
 * and Oblongy memory management, courtesy of AnkleObject.
 * In other words, it holds two things.
 */
template <typename CARType, typename CDRType,
          template <typename DUM1> class CAR_MEM_MGR = UnspecifiedMemMgmt,
          template <typename DUM2> class CDR_MEM_MGR = UnspecifiedMemMgmt>
class ObCons
{
#include "OC_Helpy.h"

  typedef OC_FUNCTIONARY::Videlicet___<Helpy::car_to_be_wrapped,
                                       Helpy::cdr_to_be_wrapped, CARType,
                                       CDRType, CarACCESSType, CdrACCESSType,
                                       CarSTOREDType, CdrSTOREDType>
    Viz;

 OB_PRIVATE:
  /**
   * the first item of the pair
   */
  mutable CarSTOREDType car;
  /**
   * the second item of the pair
   */
  mutable CdrSTOREDType cdr;

 public:
  /**
   * a new no-argument cons; not much in there, but then what do you expect?
   */
  ObCons () : car (Viz::carnullret ()), cdr (Viz::cdrnullret ()) {}

  /**
   * Constructs a new cons with the specified car and cdr.
   */
  ObCons (CarARGType a, CdrARGType d) : car (a), cdr (d) {}

  /**
   * Constructs a new cons with argument convertible to car's type and cdr
   */
  template <typename CAR_ASSOCIATED_TYPE>
  ObCons (CAR_ASSOCIATED_TYPE a, CdrARGType d) : car (a), cdr (d)
  {
  }

  /**
   * Constructs a new cons with car and argument convertible to cdr's type
   */
  template <typename CDR_ASSOCIATED_TYPE>
  ObCons (CarARGType a, CDR_ASSOCIATED_TYPE d) : car (a), cdr (d)
  {
  }

  /**
   * Constructs a new cons with argument convertible to car's type and
   * argument convertible to cdr's type.
   */
  template <typename CAR_ASSOCIATED_TYPE, typename CDR_ASSOCIATED_TYPE>
  ObCons (CAR_ASSOCIATED_TYPE a, CDR_ASSOCIATED_TYPE d) : car (a), cdr (d)
  {
  }

  ~ObCons () {}

  void Delete () { delete this; }


  /**
   * Mutates the car.
   */
  void SetCar (CarARGType a) { car = a; }

  /**
   * Mutates the car with argument convertible to car's type.
   */
  template <typename CAR_ASSOCIATED_TYPE>
  void SetCar (CAR_ASSOCIATED_TYPE a)
  {
    car = a;
  }


  /**
   * Mutates the cdr.
   */
  void SetCdr (CdrARGType d) { cdr = d; }

  /**
   * Mutates the cdr with argument convertible to cdr's type.
   */
  template <typename CDR_ASSOCIATED_TYPE>
  void SetCdr (CDR_ASSOCIATED_TYPE d)
  {
    cdr = d;
  }


  /**
   * Returns the car.
   */
  CarACCESSType Car () const { return Viz::caraccess (car); }

  /**
   * Returns the cdr.
   */
  CdrACCESSType Cdr () const { return Viz::cdraccess (cdr); }


  /**
   * Returns a reference (an ObRef) to the car; will not compile
   * for non-pointer types.
   */
  CarSTOREDType CarObRef () const
  {
    Viz::__OBERR_Cannot_ObRef_Such_A_Car_Type__ ();
    return car;
  }

  /**
   * Returns a reference (an ObRef) to the cdr; will not compile
   * for non-pointer types.
   */
  CdrSTOREDType CdrObRef () const
  {
    Viz::__OBERR_Cannot_ObRef_Such_A_Cdr_Type__ ();
    return cdr;
  }

  bool operator== (const ObCons &otha) const
  {
    return (Viz::caraccess (car) == otha.Car ()
            && Viz::cdraccess (cdr) == otha.Cdr ());
  }
};
}
}  // end namespaces loam, oblong...


#endif
