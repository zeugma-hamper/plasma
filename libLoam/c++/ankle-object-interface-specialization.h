
/* (c)  oblong industries */

#ifndef ANKLE_OBJECT_INTERFACE_SPECIALIZATION_HEADER_GUARD
#define ANKLE_OBJECT_INTERFACE_SPECIALIZATION_HEADER_GUARD


/**
 * specialization for very simple types that will be used only as
 * interface declarations attached to AnkleObject-descendant
 * classes. this allows the compiler to do proper obreffish memory
 * management even if it only knows that an object adheres to a
 * particular interface. the cost is that, once you declare an
 * interface to be AnkleObject-specialized, it's an error to define
 * a class that inherits from that interface but not AnkleObject.
 *
 * horrifyingly, you may not call the DECLARE_... macro from inside
 * any namespace block. sorry.
 *
 *   \code
 *   #include <libLoam/c++/ankle-object-interface-specialization.h>
 *   #include <libBasement/KneeObject.h>
 *
 *
 *   using namespace oblong::basement;
 *
 *
 *   class intrfaithA
 *   { public:
 *     virtual ObRetort MethA ()
 *     { printf ("base-a: %p\n", this);
 *       return OB_OK;
 *     }
 *   };
 *   DECLARE_INTERFACE_AS_ANKLE_OBJECT_SPECIALIZED (intrfaithA);
 *
 *   ObTrove <intrfaithA *> atro;
 *   atro . Append (new HappilyA ());
 *   \endcode
 *
 *
 */


#include <libLoam/c++/AnkleObject.h>


#define DECLARE_INTERFACE_AS_ANKLE_OBJECT_SPECIALIZED(SPECIALIZED_T)           \
                                                                               \
                                                                               \
  namespace oblong {                                                           \
  namespace loam {                                                             \
                                                                               \
                                                                               \
  template <typename MEM_MGR_CLASS>                                            \
  class ObRef_Guts<SPECIALIZED_T, MEM_MGR_CLASS>                               \
  {                                                                            \
   public:                                                                     \
    AnkleObject *ao;                                                           \
                                                                               \
    ObRef_Guts () noexcept : ao (NULL) {}                                      \
    ObRef_Guts (SPECIALIZED_T *p)                                              \
    {                                                                          \
      if (!p)                                                                  \
        {                                                                      \
          ao = NULL;                                                           \
          return;                                                              \
        }                                                                      \
                                                                               \
      ao = dynamic_cast<AnkleObject *> (p);                                    \
      if (!ao)                                                                 \
        {                                                                      \
          OB_FATAL_BUG_CODE (                                                  \
            0x11070000,                                                        \
            "TERRIBLE MISTAKE trying to use a biased interface \n"             \
            "in an ObReffish thing, but failing to declare that \n"            \
            "the actual class at issue is AnkleObject-derived, \n"             \
            "too. (It would be nice if this error were a compile-\n"           \
            "time matter, but it's not clear how to implement \n"              \
            "such a thing. \n");                                               \
        }                                                                      \
      if (ao->grip_mnd)                                                        \
        {                                                                      \
          ao->grip_mnd->Grip ();                                               \
          return;                                                              \
        }                                                                      \
      ao->grip_mnd = new MEM_MGR_CLASS (p);                                    \
    }                                                                          \
                                                                               \
    ~ObRef_Guts () { DisownObject (); }                                        \
                                                                               \
    ObRef_Guts (AnkleObject *p, OB_UNUSED GripMinder *m) : ao (p)              \
    {                                                                          \
      /* see AnkleObject_ObRefGuts for discussion */                           \
      if (ao && ao->grip_mnd)                                                  \
        ao->grip_mnd->Grip ();                                                 \
      else if (0)                                                              \
        OB_FATAL_BUG_CODE (0x11070001,                                         \
                           "our expectation that a subclass of a AnkleObject " \
                           "subclass constructed in ObRef_Guts would have a "  \
                           "grip_mnd is sadly unjustified");                   \
    }                                                                          \
                                                                               \
    void Become (SPECIALIZED_T *p, OB_UNUSED GripMinder *m)                    \
    {                                                                          \
      DisownObject ();                                                         \
      ao = dynamic_cast<AnkleObject *> (p);                                    \
      ClaimObject ();                                                          \
    }                                                                          \
    void Switch (ObRef_Guts &r) { std::swap (ao, r.ao); }                      \
                                                                               \
    void ClaimObject ()                                                        \
    {                                                                          \
      if (ao && ao->grip_mnd)                                                  \
        ao->grip_mnd->Grip ();                                                 \
    }                                                                          \
    void DisownObject ()                                                       \
    {                                                                          \
      if (ao && ao->grip_mnd)                                                  \
        {                                                                      \
          if (ao->grip_mnd->QueryIsWeakRefShill ())                            \
            return;                                                            \
          else if (ao->grip_mnd->Ungrip ())                                    \
            {                                                                  \
              delete ao->grip_mnd;                                             \
              ao->grip_mnd = NULL;                                             \
              ao->CleanUp ();                                                  \
            }                                                                  \
        }                                                                      \
    }                                                                          \
                                                                               \
    SPECIALIZED_T *ItsPointer () const                                         \
    {                                                                          \
      return dynamic_cast<SPECIALIZED_T *> (ao);                               \
    }                                                                          \
    GripMinder *ItsGripMinder () const { return (ao ? ao->grip_mnd : NULL); }  \
  };                                                                           \
  }                                                                            \
  }  //   end o' namespace Shamus McOblong O'Loam


#endif
