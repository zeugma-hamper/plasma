
/* (c)  oblong industries */

struct Helpy
{

  enum
  {
    key_is_contructorless =
      std::is_trivially_default_constructible<KEYType>::value
  };

  enum
  {
    val_is_contructorless =
      std::is_trivially_default_constructible<VALType>::value
  };

  enum
  {
    key_is_pointy =
      std::is_pointer<KEYType>::value || std::is_member_pointer<KEYType>::value
  };

  enum
  {
    val_is_pointy =
      std::is_pointer<VALType>::value || std::is_member_pointer<VALType>::value
  };

  enum
  {
    key_to_be_wrapped =
      std::is_pointer<KEYType>::value && !std::is_function<KEYType>::value
      && !std::is_member_function_pointer<KEYType>::value
      && !std::is_function<typename std::remove_pointer<KEYType>::type>::value
      && !std::is_same<KEY_MEM_MGR_TAG<KEYType>, NoMemMgmt<KEYType>>::value
  };

  enum
  {
    val_to_be_wrapped =
      std::is_pointer<VALType>::value && !std::is_function<VALType>::value
      && !std::is_member_function_pointer<VALType>::value
      && !std::is_function<typename std::remove_pointer<VALType>::type>::value
      && !std::is_same<VAL_MEM_MGR_TAG<VALType>, NoMemMgmt<VALType>>::value
  };


  enum
  {
    key_weak_reffed =
      std::is_same<KEY_MEM_MGR_TAG<KEYType>, WeakRef<KEYType>>::value
  };

  enum
  {
    val_weak_reffed =
      std::is_same<VAL_MEM_MGR_TAG<VALType>, WeakRef<VALType>>::value
  };


  typedef
    typename std::conditional<key_to_be_wrapped, KEYType, const KEYType &>::type
      KEYARGTYPE;

  typedef
    typename std::conditional<val_to_be_wrapped, VALType, const VALType &>::type
      VALARGTYPE;


  typedef typename std::conditional<key_to_be_wrapped, KEYType, KEYType &>::type
    KEYACCESSTYPE;

  typedef typename std::conditional<val_to_be_wrapped, VALType, VALType &>::type
    VALACCESSTYPE;


  typedef typename std::conditional
    //    <key_to_be_wrapped, KEYType, KEYType &> :: type
    <key_to_be_wrapped, KEYType, KEYType>::type KEYCRAWLRETTYPE;

  typedef typename std::conditional
    //    <val_to_be_wrapped, VALType, VALType &> :: type
    <val_to_be_wrapped, VALType, VALType>::type VALCRAWLRETTYPE;


  typedef typename std::
    conditional<key_to_be_wrapped,
                typename std::conditional<key_weak_reffed, ObWeakRef<KEYType>,
                                          ObRef<KEYType,
                                                KEY_MEM_MGR_TAG>>::type,
                KEYType>::type KEYSTOREDTYPE;

  typedef typename std::
    conditional<val_to_be_wrapped,
                typename std::conditional<val_weak_reffed, ObWeakRef<VALType>,
                                          ObRef<VALType,
                                                VAL_MEM_MGR_TAG>>::type,
                VALType>::type VALSTOREDTYPE;


};  // the end of ol' Helpy (the struct, not the fungal statistician)



typedef typename Helpy::KEYARGTYPE KeyARGType;
typedef typename Helpy::KEYACCESSTYPE KeyACCESSType;
typedef typename Helpy::KEYCRAWLRETTYPE KeyCRAWLRetType;
typedef typename Helpy::KEYSTOREDTYPE KeySTOREDType;


typedef typename Helpy::VALARGTYPE ValARGType;
typedef typename Helpy::VALACCESSTYPE ValACCESSType;
typedef typename Helpy::VALCRAWLRETTYPE ValCRAWLRetType;
typedef typename Helpy::VALSTOREDTYPE ValSTOREDType;
