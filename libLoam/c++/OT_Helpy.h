
/* (c)  oblong industries */

struct Helpy
{

  enum
  {
    is_constructorless =
      std::is_trivially_default_constructible<ELEMType>::value
  };

  enum
  {
    is_pointy = std::is_pointer<ELEMType>::value
                || std::is_member_pointer<ELEMType>::value
  };

  enum
  {
    to_be_wrapped =
      std::is_pointer<ELEMType>::value && !std::is_function<ELEMType>::value
      && !std::is_member_function_pointer<ELEMType>::value
      && !std::is_function<typename std::remove_pointer<ELEMType>::type>::value
#ifdef WITH_MEM_MANAGEMENT_SENSITIVITY
      && !std::is_same<MEM_MGR_CLASS<ELEMType>, NoMemMgmt<ELEMType>>::value
#endif
  };

#ifdef WITH_MEM_MANAGEMENT_SENSITIVITY
  enum
  {
    weak_reffed =
      std::is_same<MEM_MGR_CLASS<ELEMType>, WeakRef<ELEMType>>::value
  };
#endif


  typedef
    typename std::conditional<to_be_wrapped, ELEMType, const ELEMType &>::type
      ARGTYPE;

  typedef typename std::conditional<to_be_wrapped, ELEMType, ELEMType &>::type
    ACCESSTYPE;

  typedef typename std::conditional
    //    < to_be_wrapped, ELEMType, ELEMType & > :: type
    <to_be_wrapped, ELEMType, ELEMType>::type CRAWLRETTYPE;

#ifdef WITH_MEM_MANAGEMENT_SENSITIVITY
  typedef typename std::
    conditional<to_be_wrapped,
                typename std::conditional<weak_reffed, ObWeakRef<ELEMType>,
                                          ObRef<ELEMType, MEM_MGR_CLASS>>::type,
                ELEMType>::type STOREDTYPE;
#endif

};  // the end of ol' Helpy (the struct, not the clown)



typedef typename Helpy::ARGTYPE ARGType;
typedef typename Helpy::ACCESSTYPE ACCESSType;
typedef typename Helpy::CRAWLRETTYPE CRAWLRetType;

#ifdef WITH_MEM_MANAGEMENT_SENSITIVITY
typedef typename Helpy::STOREDTYPE STOREDType;
#endif
