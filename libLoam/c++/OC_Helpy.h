
/* (c)  oblong industries */

struct Helpy
{

  enum
  {
    car_to_be_wrapped =
      std::is_pointer<CARType>::value && !std::is_function<CARType>::value
      && !std::is_member_function_pointer<CARType>::value
      && !std::is_function<typename std::remove_pointer<CARType>::type>::value
      && !std::is_same<CAR_MEM_MGR<CARType>, NoMemMgmt<CARType>>::value
  };

  enum
  {
    cdr_to_be_wrapped =
      std::is_pointer<CDRType>::value && !std::is_function<CDRType>::value
      && !std::is_member_function_pointer<CDRType>::value
      && !std::is_function<typename std::remove_pointer<CDRType>::type>::value
      && !std::is_same<CDR_MEM_MGR<CDRType>, NoMemMgmt<CDRType>>::value
  };


  enum
  {
    car_weak_reffed =
      std::is_same<CAR_MEM_MGR<CARType>, WeakRef<CARType>>::value
  };

  enum
  {
    cdr_weak_reffed =
      std::is_same<CDR_MEM_MGR<CDRType>, WeakRef<CDRType>>::value
  };


  typedef
    typename std::conditional<car_to_be_wrapped, CARType, const CARType &>::type
      CARARGTYPE;

  typedef
    typename std::conditional<cdr_to_be_wrapped, CDRType, const CDRType &>::type
      CDRARGTYPE;


  typedef typename std::conditional<car_to_be_wrapped, CARType, CARType &>::type
    CARACCESSTYPE;

  typedef typename std::conditional<cdr_to_be_wrapped, CDRType, CDRType &>::type
    CDRACCESSTYPE;


  typedef typename std::conditional
    //    < car_to_be_wrapped, CARType, CARType & > :: type
    <car_to_be_wrapped, CARType, CARType>::type CARCRAWLRETTYPE;

  typedef typename std::conditional
    //    < cdr_to_be_wrapped, CDRType, CDRType & > :: type
    <cdr_to_be_wrapped, CDRType, CDRType>::type CDRCRAWLRETTYPE;


  typedef typename std::
    conditional<car_to_be_wrapped,
                typename std::conditional<car_weak_reffed, ObWeakRef<CARType>,
                                          ObRef<CARType, CAR_MEM_MGR>>::type,
                CARType>::type CARSTOREDTYPE;

  typedef typename std::
    conditional<cdr_to_be_wrapped,
                typename std::conditional<cdr_weak_reffed, ObWeakRef<CDRType>,
                                          ObRef<CDRType, CDR_MEM_MGR>>::type,
                CDRType>::type CDRSTOREDTYPE;


};  // the end of ol' Helpy (the struct, not the fungal statistician)



typedef typename Helpy::CARARGTYPE CarARGType;
typedef typename Helpy::CARACCESSTYPE CarACCESSType;
typedef typename Helpy::CARCRAWLRETTYPE CarCRAWLRetType;
typedef typename Helpy::CARSTOREDTYPE CarSTOREDType;


typedef typename Helpy::CDRARGTYPE CdrARGType;
typedef typename Helpy::CDRACCESSTYPE CdrACCESSType;
typedef typename Helpy::CDRCRAWLRETTYPE CdrCRAWLRetType;
typedef typename Helpy::CDRSTOREDTYPE CdrSTOREDType;
