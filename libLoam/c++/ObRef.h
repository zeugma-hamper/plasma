
/* (c)  oblong industries */

#ifndef OB_REF_HEADER_GUARD
#define OB_REF_HEADER_GUARD


#include <stdio.h>

#include <libLoam/c/ob-types.h>
#include <libLoam/c/ob-log.h>
#include <libLoam/c++/Preterite.h>


#include <type_traits>
#include <utility>


/* NOT A DOXYGEN COMMENT
 * (For maintainers: here's some foundational theory-of-operation stuff...)
 *
 * The contents following are the hoist point for a collection of
 * related classes and files that together form the basis for
 * yovo's/g-speak's memory management system(s). Central is the class
 * 'ObRef', defined (mostly) here, which is roughly equivalent to
 * other libraries' 'smart pointer' classes. ObRef, however, offers a
 * number of features that render it more desirable than these other
 * alternatives; chief among these is a bifurcative design that allows
 * the ObRef system to work with arbitrary C++ objects but that also
 * specializes around g-speak's own object system (rooted at the class
 * AnkleObject) -- ObRef and AnkleObject are much more tightly
 * integrated, through some explicit knowledge of each other, than
 * would be possible in the general case. In particular, ObRef knows
 * to fetch AnkleObject's internal GripMinder.
 *
 * Files and classes directly implicated in the scheme:
 * ObRef.h; ObWeakRef; AnkleObject.{h,cpp};
 * ankle-object-interface-specialization.h.
 *
 * To the details: ObRef itself is an approachable veneer around two
 * other progressively nested components: ObRef_Guts and GripMinder.
 * From bottom up, GripMinder is a 'memory management scheme'; the sole
 * version of GripMinder today implements reference counting
 * (RefCountGripMinder); but the architecture would allow additional
 * subclasses affording other management models to be slotted into
 * place. (Note that multivalence at the GripMinder level is achieved
 * through subclassing.) ObRef_Guts provides the actual
 * machinery for coordinating referencing and managing the life
 * of the object given to ObRef. Any particular version of ObRef_Guts
 * typically holds (1) a pointer to the object in question, and (2)
 * a pointer to an instance of an appropriate GripMinder. Finally,
 * an ObRef wraps an instance of ObRef_Guts and exposes its functionality
 * to client code in a genteel way. The central idea in the aggregate
 * system is that there may be many ObRef instances that refer to
 * a single managed object; each ObRef holds its own distinct ObRef_Guts;
 * but all cooperating gutses hold on to the same shared, single
 * instance of GripMinder.
 *
 * As to this file's layout (templating makes it difficult to
 * understand, well, pretty much anything in the world of C++),
 * things progress inside out: at the top comes the abstract definition
 * of GripMinder (which includes, as an inner class, the starting
 * definition of WeakGripMinder, the analogous machine for dealing
 * with weak references); then the actual implementation of
 * RefCountGripMinder (and, inside, RefCountWeakGripMinder); then
 * the base-class version of ObRef_Guts; then ObRef_NoMemMgmtGuts,
 * a guts version that implements a devil-may-care approach in
 * which no memory management at all is done (and, accordingly,
 * ObRef_NoMemMgmtGuts holds a pointer to the object-in-question
 * but no pointer at all to a GripMinder); then, finally, ObRef itself.
 *
 * Elsewhere...
 *
 * -- in ankle-object-interface-specialization.h is a macro that
 *    allows little 'interface' classes that have been attached
 *    to AnkleObject derivatives (via, i.e., multiple inheritance)
 *    to be treated by ObRef mechanisms as if they were AO classes
 *    directly.
 *
 * -- in ObWeakRef lurks the parallel-universe version of ObRef
 *    that has not the power to resist deletion. Beware: it's not
 *    a subclass of ObRef (nor a templated sibling); it's its own
 *    thing altogether. Moreover, it doesn't (unlike ObRef) hold
 *    on to an ObRef_Guts of any kind; instead, it directly
 *    encapsulates (1) a pointer to the (weakly-)held object, and
 *    (2) a WeakGripMinder instance.
 *
 * TO BE DONE: reasonably explicated examples that show what
 * versions of templated structures end up getting assembled
 * under different circumstances...
 *
 */


namespace oblong {
namespace loam {


/**
 * \cond INTERNAL
 * GripMinder - abstract base class for memory management
 * implementations. So this is the parent of, for example, our
 * RefCountGripMinder, just below. An ObRef holds onto a pointer to one of
 * these (directly, for non-AnkleObject ObRefs, or by holding onto a
 * AnkleObject pointer which itself holds a pointer to one of
 * these). The critical methods are Grip() and Ungrip(), which are
 * similar to 'ref' and 'unref' operations in a typical
 * reference-counting API but in a more generalized sense that
 * anticipates the implementation of alternate memory management schemes.
 * Weak references are also sketched out in this
 * abstract API, but subclasses do not have to implement them.
 */
class GripMinder
{
 public:
  virtual void Grip () = 0;
  virtual bool Ungrip () = 0;

  class WeakGripMinder
  {
   public:
    bool is_valid;
    WeakGripMinder () : is_valid (true) {}
    virtual ~WeakGripMinder () {}
    virtual void Grip () = 0;
    virtual bool Ungrip () = 0;
    virtual bool IsValid () { return is_valid; }
  };

  virtual WeakGripMinder *FurnishWeakGripMinder () { return nullptr; }
  virtual void MangleToBeWeakRefShill () {}
  virtual bool QueryIsWeakRefShill () { return false; }

  virtual ~GripMinder () {}
};
/** \endcond */

/**
 * \cond INTERNAL
 * RefCountGripMinder - the memory management workhorse (for the moment, at
 * least). We just need a pointer to an int32, so we can share a
 * reference count between ObRefs that live on the stack.
 */
template <typename T>
class RefCountGripMinder : public GripMinder
{
 public:
  class RefCountWeakGripMinder;

  enum
  {
    WEAK_SHILL_INDICATOR = -100
  };

  int32 count;
  RefCountWeakGripMinder *weak_grip_mnd;

  RefCountGripMinder (OB_UNUSED T *p) : count (1), weak_grip_mnd (NULL) {}
  void Grip () override
  {
    if (QueryIsWeakRefShill ())
      count = 0;
    ++count;
  }
  ~RefCountGripMinder () override
  {
    if (weak_grip_mnd)
      {
        if (weak_grip_mnd->wk_count == 0)
          delete weak_grip_mnd;
        else
          weak_grip_mnd->is_valid = false;
      }
  }
  bool Ungrip () override
  {
    if (QueryIsWeakRefShill () || (--count != 0))
      return false;
    return true;
  }

  WeakGripMinder *FurnishWeakGripMinder () override
  {
    if (!weak_grip_mnd)
      weak_grip_mnd = new RefCountWeakGripMinder ();
    else
      weak_grip_mnd->Grip ();
    return weak_grip_mnd;
  }

  void MangleToBeWeakRefShill () override
  {
    if (count != 1)
      {
        OB_FATAL_BUG_CODE (0x11040000,
                           "oh no: expected weakref mangle to be called "
                           "only immediately after construction\n");
      }
    count = WEAK_SHILL_INDICATOR;
  }

  bool QueryIsWeakRefShill () override
  {
    return (count == WEAK_SHILL_INDICATOR);
  }

  // And now for the weak reference count implementation
  class RefCountWeakGripMinder : public GripMinder::WeakGripMinder
  {
   public:
    int32 wk_count;
    RefCountWeakGripMinder () : WeakGripMinder (), wk_count (1) {}
    ~RefCountWeakGripMinder () override {}
    void Grip () override { ++wk_count; }
    bool Ungrip () override
    {
      if (--wk_count == 0)
        {
          if (!is_valid)
            delete this;
          return true;
        }
      return false;
    }
  };
};
/** \endcond */


class AnkleObject;

/**
 * \cond INTERNAL
 *
 * ObRef_Guts - internal structural/manipulation details for ObRef.
 * In this base template, we manage a pointer to the gripped object
 * and a pointer to a grip minder. The methods here define the
 * interface that will need to be duplicated in any ObRef_Guts
 * specializations. The T template argument is the type of pointer
 * that we're managing; the MEM_MGR_CLASS argument is the specific
 * memory manager class to use if we need to construct a new
 * GripMinder (which we will not always need to do). The
 * ENABLEMENT_CRITERION makes it possible to write specializations for
 * groups of types (such as inheritance heirarchies).
 *
 * If you want to specialize on a single type, you can just leave out
 * the ENABLEMENT_CRITERION argument.
 *
 *   template <typename T, typename WRP_MEM_MGR_CLASS>
 *   struct ObRef_Guts <SomeSpecificType, WRP_MEM_MGR_CLASS>
 *   { ...
 */
template <typename T, typename MEM_MGR_CLASS,
          typename ENABLEMENT_CRITERION = void>
class ObRef_Guts
{
 public:
  T *raw_ptr;            // the pointer this ObRef is managing
  GripMinder *grip_mnd;  // memory manager (RefCountGripMinder, usually)

  /**
   * null and normal constructors. The normal constructor makes a new
   * memory management wrapper (so, in this case of
   * RefCountGripMinder, a new sharable pointer to an int32 with a
   * value of '1').
   */
  ObRef_Guts () noexcept : raw_ptr (nullptr), grip_mnd (nullptr) {}
  ObRef_Guts (T *p) : raw_ptr (p), grip_mnd (nullptr)
  {
    if (!raw_ptr)
      return;

    // Because AnkleObject carries around its grip_mnd, so we need to fetch it from p.
    grip_mnd = RetrieveGripMinder (raw_ptr);
    if (!grip_mnd)
      {
        grip_mnd = new MEM_MGR_CLASS (p);
        InstallGripMinder (raw_ptr, grip_mnd);
      }
    else
      {
        grip_mnd->Grip ();
      }
  }

  /**
   * "normal" constructor that will be chosen by the overloading
   * mechanism if this guts is part of an ObRef with a "guts override
   * type"
   */
  template <typename SUBT>
  ObRef_Guts (SUBT *p) : raw_ptr (p), grip_mnd (nullptr)
  {
    if (!raw_ptr)
      return;

    // Because AnkleObject carries around its grip_mnd, so we need to fetch it from p.
    grip_mnd = RetrieveGripMinder (raw_ptr);
    if (!grip_mnd)
      {
        grip_mnd = new MEM_MGR_CLASS (p);
        InstallGripMinder (raw_ptr, grip_mnd);
      }
    else
      {
        grip_mnd->Grip ();
      }
  }

  ~ObRef_Guts () { DisownObject (); }

  /**
   * a copy constructor of sorts, called by the ObRef copy
   * constructors, about which, see their very own commentary. since
   * this is called by ObRef's copy(ish) ctors, either p and m are
   * both nullptr or are both valid pointers. additionally, in the AO
   * case, p->grip_minder == m.
   */
  ObRef_Guts (T *p, GripMinder *m) : raw_ptr (p), grip_mnd (m)
  {
    if (!raw_ptr)
      return;

    if (grip_mnd)
      grip_mnd->Grip ();
  }

  /**
   * an assignment operator of sorts, called by the ObRef assignment
   * operators. We do assignment with this method, rather than '=', so
   * as to avoid the construction of a temporary. As this is only
   * called by the assigment(ish) operators of ObRef, either p and m
   * are both nullptr or are both valid pointers. additionally, in the
   * AO case, p->grip_minder == m.
   */
  void Become (T *p, GripMinder *m)
  {
    DisownObject ();
    raw_ptr = p;
    grip_mnd = m;
    ClaimObject ();
  }

  void Switch (ObRef_Guts &g) noexcept
  {
    std::swap (raw_ptr, g.raw_ptr);
    std::swap (grip_mnd, g.grip_mnd);
  }

  void ClaimObject ()
  {
    if (grip_mnd)
      grip_mnd->Grip ();
  }

  void DisownObject ()
  {
    if (raw_ptr && grip_mnd)
      {
        if (!grip_mnd->QueryIsWeakRefShill ())
          {
            if (grip_mnd->Ungrip ())
              {
                //deleting but take care to avoid circular ref problems
                delete grip_mnd;
                grip_mnd = nullptr;
                InstallGripMinder (raw_ptr, nullptr);
                T *tmp = raw_ptr;
                raw_ptr = nullptr;
                ReleaseMemory (tmp);
              }
            else
              {
                // references still exist so null internal ptr
                grip_mnd = nullptr;
                raw_ptr = nullptr;
              }
          }
        else
          {
            // nulling out the grip_mnd is causing weak ref shills to fail
            // grip_mnd = nullptr;
            raw_ptr = nullptr;
          }
      }
  }

  T *ItsPointer () const { return raw_ptr; }
  GripMinder *ItsGripMinder () const { return grip_mnd; }

  /**
   * AnkleObject and subclasses carry around a pointer to their minder
   * internally, so here use a templated function to selectively set
   * AO's internal pointer.
   */
  template <typename P,
            typename = typename std::enable_if<std::is_base_of<AnkleObject,
                                                               P>::value>::type>
  inline void InstallGripMinder (P *p, GripMinder *minder)
  {
    p->grip_mnd = minder;
  }

  /**
   * We want the above version of InstallGripMinder to be called if
   * ObRef is holding a pointer to AnkleObject, but we have to have a
   * method for the non-AO case. varargs makes this method much, much
   * lower priority in override resolution.
   */
  inline void InstallGripMinder (...) {}

  /**
   * Similar to Equip, selectively get the minder from object, if it's
   * an AnkleObject or subclass.
   */
  template <typename P,
            typename = typename std::enable_if<std::is_base_of<AnkleObject,
                                                               P>::value>::type>
  inline GripMinder *RetrieveGripMinder (P *p) const
  {
    return p ? p->grip_mnd : nullptr;
  }

  /**
   * We want the above version of RetrieveGripMinder to be called if
   * ObRef is holding a pointer to AnkleObject, but we have to have a
   * method for the non-AO case. varargs makes this method much, much
   * lower priority in override resolution.
   */
  inline GripMinder *RetrieveGripMinder (...) const { return nullptr; }

  /**
   * AnkleObjects use CleanUp instead of delete.
   */
  template <typename P,
            typename = typename std::enable_if<std::is_base_of<AnkleObject,
                                                               P>::value>::type>
  inline void ReleaseMemory (P *p)
  {
    if (p)
      p->CleanUp ();
  }

  /**
   * We want the above version of ReleaseMemory to be called if
   * ObRef is holding a pointer to AnkleObject, but we have to have a
   * method for the non-AO case. varargs makes this method much, much
   * lower priority in override resolution.
   */
  inline void ReleaseMemory (void *t) { delete (T *) t; }
};
/** \endcond */

/**
 * NoMemMgt - type "tag" specifying that an ObRef should do no memory
 * management on behalf of the pointer it holds. The ObRef_NoMemMgmtGuts
 * structure implements the internals for ObRefs so created (and
 * follows the ObRef_Guts API described above).
 */
template <typename T>
struct NoMemMgmt
{
};

/**
 * \cond INTERNAL
 */
template <typename T>
class ObRef_NoMemMgmtGuts
{
 public:
  T *obj_ptr;  // the pointer this ObRef is managing
  ObRef_NoMemMgmtGuts () noexcept : obj_ptr (NULL) {}
  ObRef_NoMemMgmtGuts (T *p) : obj_ptr (p) {}
  ~ObRef_NoMemMgmtGuts () {}
  ObRef_NoMemMgmtGuts (T *p, OB_UNUSED GripMinder *m) : obj_ptr (p) {}
  void Become (T *p, OB_UNUSED GripMinder *m) { obj_ptr = p; }
  void Switch (ObRef_NoMemMgmtGuts &r) noexcept
  {
    std::swap (obj_ptr, r.obj_ptr);
  }
  void ClaimObject () {}
  void DisownObject () { obj_ptr = NULL; }
  T *ItsPointer () const { return obj_ptr; }
  GripMinder *ItsGripMinder () const { return NULL; }
};
/** \endcond */

/**
 * UnspecifiedMemMgmt - placeholder template used as default for
 * ObRef's memory management wrapper argument. Use of this template
 * allows us to limit copy construction and left-side assignment to
 * ObRefs that either (a) have matched memory management internals
 * with their right-hand side arguments, or (b) don't have a specified
 * memory management scheme.
 */
template <typename T>
struct UnspecifiedMemMgmt
{
  /**
   * \cond INTERNAL
   */
  static void ERROR__In_CopyConstructing_an_ObRef_With_MemMgmt_Specified () {}
  static void ERROR__In_Assigning_to_an_ObRef_With_MemMgmt_Specified () {}
  /** \endcond */
};


// forward declaration of... well, you can read.
template <typename T>
class ObWeakRef;


/**
 * \cond INTERNAL
 * Who knows what I am?  Nobody has seen fit to document me!
 * Looks like I am just an empty class, which seems pretty boring.
 */
class GutsSameAsT
{
};
/** \endcond */


// ObRef - initial, abstract tempate declaration for
// ObRef. Constructors are private, so you can't make one of these.
//
template <typename T, template <class> class MEM_MGR_TAG = UnspecifiedMemMgmt,
          typename GUTS_OVERRIDE_T = GutsSameAsT>
class ObRef
{
 private:
  ObRef ();
  ObRef (T);
  ObRef<T, MEM_MGR_TAG> &operator= (const ObRef<T, MEM_MGR_TAG>);
};


/**
 * ObRef - well here we are, the life of the party: partial
 * specialization of ObRef for pointer types. The internal structure
 * of an ObRef differs depending on whether T* is a
 * AnkleObject-descendant or not. We provide as few methods as
 * possible here: constructor from pointer; copy constructor (which is
 * templatized to be polymorphic on T*); operator= (likewise
 * templated); and operator~ for pointer fetch. The
 * WRP_MEM_MGR_TEMPLATE argument is optional, as it has a default
 * value above in the initial ObRef template. The WRP_MEM_MGR_TEMPLATE
 * argument declares the type of memory management to use *if* it's
 * necessary for the ObRef to initialize memory management for the
 * pointer. The pointer may already have memory management associated
 * with it; an AnkleObject holds onto its own GripMinder pointer,
 * and even in the case of a vanilla object, if we construct or assign
 * from another ObRef rather than from a pointer, then we'll use that
 * ObRef's GripMinder rather than a new one. This strategy can be
 * thought of as making the memory management type (a) tied to a given
 * pointer rather than a given ObRef, and (b) polymorphic at run-time,
 * rather than compile-time. Note that this works consistently for
 * AnkleObject, because they keep track of things themselves. But for
 * non-AnkleObject pointers you have to carefully restrict yourself to
 * working with ObRefs, not bare pointers, or trouble will find
 * you. Please also note that copy constructing and assignment are
 * legal only for a ObRefs created with the default
 * WRP_MEM_MGR_TEMPLATE argument, or an ObRef that exactly matches in
 * type and WRP_MEM_MGR_TEMPLATE: we throw a compile-time error if you
 * try to copy-construct or assign to a non-matching ObRef.
 */
template <typename T, template <class> class MEM_MGR_TAG,
          typename GUTS_OVERRIDE_T>
class ObRef<T *, MEM_MGR_TAG, GUTS_OVERRIDE_T>
{
 public:
  /**
   * if we're instantiated with a template argument of
   * 'UnspecifiedMemMgmt', we need to pass RefCountGripMinder to the parts
   * of our internals that might need to construct a new memory
   * management wrapper. Otherwise we want to use our template
   * template argument.
   */
  typedef typename std::
    conditional<std::is_same<UnspecifiedMemMgmt<T>, MEM_MGR_TAG<T>>::value,
                RefCountGripMinder<T>, MEM_MGR_TAG<T>>::type MemMgrClass;

  /**
   * customize our internal structure so as: (1) if we're not doing
   * memory management we use the ObRef_NoMemMgmtGuts internals. (2)
   * otherwise, we need an ObRef_Guts structure appropriate to the
   * type we're managing. Specializing ObRef_Guts is easy, and
   * necessary if you want to do anything unusual -- call a custom
   * deleter, for example. Look above for the generic implementation
   * API. Formerly, AnkleObject-derived classes used a specialized
   * ObRef_Guts.
   */
  typedef typename std::
    conditional<std::is_same<NoMemMgmt<T>, MEM_MGR_TAG<T>>::value,
                ObRef_NoMemMgmtGuts<T>,
                typename std::
                  conditional<std::is_same<GutsSameAsT, GUTS_OVERRIDE_T>::value,
                              ObRef_Guts<T, MemMgrClass>,
                              ObRef_Guts<GUTS_OVERRIDE_T, MemMgrClass>>::type>::
      type GutsType;
  GutsType guts;

  // note that the foregoing ('guts') is the sole instance variable -- i.e.
  // memory usage -- in this class. ObRef is no more than its guts.

  /**
   * null and normal constructors.
   */
  //@{
  ObRef () noexcept (noexcept (GutsType ())) : guts () {}
  ObRef (T *p) : guts (p) {}
  //@}

  /**
   * copy constructors - we need two, because we have to define a
   * template function to handle derived-class conversions, but the
   * compiler's built-in is a better match than the templated version
   * so we get that for exact type matches unless we define our
   * own. Ergo, code duplication. When the copy constructor is
   * finished, two things should be true: we've got a new guts that
   * shares the GripMinder of the ObRef we're copying; and if
   * that GripMinder is non-null, we've called its Grip()
   * method. An exact match of both T and WRP_MEM_MGR_TEMPLATE are
   * required for this function to be called.
   */
  ObRef (const ObRef<T *, MEM_MGR_TAG> &r)
      : guts (r.guts.ItsPointer (), r.guts.ItsGripMinder ())
  {
  }

  /**
   * this "copy constructor" (ahem) template only compiles if the
   * pointer held by the ObRef we're copying is implicitly convertible
   * to our pointer type. What we're trying to do here is make ObRefs
   * polymorphic in the same way that bare pointers are. We also need
   * to check to make sure that this ObRef was created either with the
   * default wrap manager template argument, or with a memory
   * management scheme that matches the ObRef we're copying. We do
   * this error check by calling a particular function defined only in
   * the UnspecifiedMemMgmt class. We are counting on the compiler to
   * (a) optimize out this empty function, and (b) if the function
   * doesn't resolve, to put its name in the console error output so
   * that a library user can infer what she's done wrong.
   */
  template <typename SUBT, template <class> class R_WRP, typename R_GUT>
  ObRef (const ObRef<SUBT, R_WRP, R_GUT> &r)
      : guts (r.guts.ItsPointer (), r.guts.ItsGripMinder ())
  {
    typedef
      typename std::conditional<std::is_same<R_WRP<T>, MEM_MGR_TAG<T>>::value,
                                UnspecifiedMemMgmt<T>, MEM_MGR_TAG<T>>::type
        MEM_MGR_LEGALITY_CHECK;

    MEM_MGR_LEGALITY_CHECK::
      ERROR__In_CopyConstructing_an_ObRef_With_MemMgmt_Specified ();
  }

  ObRef (ObRef &&r) noexcept (noexcept (GutsType ())) : guts ()
  {
    guts.Switch (r.guts);
  }

  /**
   * Assignment - exactly the same intentions and pattern as the copy
   * constructors above: a templated version and an exact-match
   * version. We do need to call Ungrip on whatever we're holding onto
   * going into the operation, as we're effectively letting go of one
   * reference in order to take on a new one.
   */
  //@{
  ObRef<T *, MEM_MGR_TAG, GUTS_OVERRIDE_T> &
  operator= (const ObRef<T *, MEM_MGR_TAG, GUTS_OVERRIDE_T> &r)
  {
    if ((~(*this)) == ~r)  // same pointer, don't need to do anything
      return *this;
    guts.Become (r.guts.ItsPointer (), r.guts.ItsGripMinder ());
    return *this;
  }

  template <typename SUBT, template <class> class R_WRP, typename R_GUT>
  ObRef<T *> &operator= (const ObRef<SUBT, R_WRP, R_GUT> &r)
  {
    typedef
      typename std::conditional<std::is_same<R_WRP<T>, MEM_MGR_TAG<T>>::value,
                                UnspecifiedMemMgmt<T>, MEM_MGR_TAG<T>>::type
        MEM_MGR_LEGALITY_CHECK;

    MEM_MGR_LEGALITY_CHECK::
      ERROR__In_CopyConstructing_an_ObRef_With_MemMgmt_Specified ();

    guts.Become (r.guts.ItsPointer (), r.guts.ItsGripMinder ());
    return *this;
  }
  //@}

  ObRef &operator= (ObRef &&r)
  {
    if (this == &r)
      return *this;

    ObRef tmp (std::move (*this));

    guts.Switch (r.guts);
    return *this;
  }

#if 0
  /**
   * declaration sans definition of assignment from ObWeakRef of same type...
   */
  ObRef <T *, MEM_MGR_TAG> &
    operator = (const ObWeakRef <T *> &wr);
#endif

  /**
   * cast here in our pointer dereference to account for the
   * possibility of the guts override type
   */
  T *operator~ () const { return (T *) guts.ItsPointer (); }

  /**
   * Syntactically treat a ref like a pointer.
   */
  T *operator-> () const { return (T *) guts.ItsPointer (); }

  /**
   * synonymous with the foregoing tilde operator... but isn't for
   * ObWeakRef
   */
  T *RawPointer () const { return (T *) guts.ItsPointer (); }

  /**
   * Exists for parity with the genuinely useful version in ObWeakRef
   * (which expresses whether the object in question has been deleted
   * via deletion of the last holding ObRef). This one here, obviously,
   * has by definition only 'no' to say.
   */
  bool HasBeenExpunged () const { return false; }

  /**
   * just so we can be like every other yovo object
   */
  void Delete () { delete this; }

  /**
   * non-public methods for setting up weak references.
   * AssociatedWeakGripMinder () is called by ObWeakRef
   * constructors that take an ObRef.
   */
  GripMinder::WeakGripMinder *AssociatedWeakGripMinder () const
  {
    if (GripMinder *grip_mnd = guts.ItsGripMinder ())
      return grip_mnd->FurnishWeakGripMinder ();
    else
      return NULL;
  }

  /**
   * MangleToBeWeakRefShill is called by ObWeakRef constructor that
   * takes an AnkleObject. The constructor needs to make a grip_mnd
   * for the AnkleObject if one isn't already in place, but doesn't
   * want to put the object under memory management in the normal
   * sense. So we make a normal, default-managed ObRef, then tell it
   * that until someone else calls its Grip() method, it should
   * pretend it doesn't exist.
   */
  void MangleGutsToBecomeWeakRefShill ()
  {
    if (GripMinder *grip_mnd = guts.ItsGripMinder ())
      grip_mnd->MangleToBeWeakRefShill ();
  }

  /**
   * Use this to break circular references. You must not use the ObRef
   * at all after a call to Nullify (). If you were to make that
   * mistake, the resulting behavior is, of course, undefined.
   */
  void Nullify () { guts.DisownObject (); }

  ~ObRef () {}
};



/**
 * this shall forevermore dwell here, serving as a tag that can be
 * placed wherever (well, anywhere except as a template argument to
 * ObRef) you might otherwise say, say, "NoMemMgmt".
 *
 * Hey! Still here... wanted to say that ObTrove is a good example of
 * a place you'd use WeakRef as a tag/symbol. ObCons too.
 */
template <typename T>
struct WeakRef
{
};
}
}  // end namespace loam! end namespace oblong!

#endif
