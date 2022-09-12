
/* (c)  oblong industries */

#ifndef OB_MAP_IS_NOT_THE_TERRITORY
#define OB_MAP_IS_NOT_THE_TERRITORY


#include <libLoam/c++/ObCons.h>
#include <libLoam/c++/ObTrove.h>
#include <libLoam/c++/CrawlIterator.h>


namespace oblong {
namespace loam {


#include "OM_Functionary.h"


/**
 * Can be an ordered map or an ordered multimap, depending on how
 * it's configured.  Lookup performance is O(n).
 */
template <typename KEYType, typename VALType,
          template <typename DUM1> class KEY_MEM_MGR_TAG = UnspecifiedMemMgmt,
          template <typename DUM2> class VAL_MEM_MGR_TAG = UnspecifiedMemMgmt>
class ObMap
{
/**
 * \cond INTERNAL
 * some preliminaries that'll let us get our various types, coming and
 * going, just right.
 */

#include "OM_Helpy.h"
  typedef OM_FUNCTIONARY::
    Videlicet___<Helpy::key_to_be_wrapped, Helpy::val_to_be_wrapped,
                 Helpy::key_is_contructorless, Helpy::val_is_contructorless,
                 KEYType, VALType>
      Viz;
  /** \endcond */

  // the worthwhile stuff follows.
 public:
  /**
   * The type of an entry
   */
  typedef ObCons<KEYType, VALType, KEY_MEM_MGR_TAG, VAL_MEM_MGR_TAG> MapCons;

 OB_PRIVATE:
  /**
   * \cond INTERNAL
   */
  ObTrove<MapCons *, NoMemMgmt> our_map;
  mutable KEYType null_k;
  mutable VALType null_v;
  /** \endcond */

 public:
  /**
   * This can be passed to SetDupKeyBehavior() to make ObMap behave
   * as a write-once map, a standard map, or a multimap.
   */
  enum Dup_Key_Behavior
  {
    Privilege_Earliest = 0,
    Privilege_Latest = 1,
    Allow_Duplicates = 2
  };

 OB_PRIVATE:
  /**
   * \cond INTERNAL
   */
  Dup_Key_Behavior dup_key_behavior;
  /** \endcond */

 public:
  typedef CrawlIterator<MapCons *, MapCons *> iterator;
  typedef CrawlIterator<MapCons *, MapCons *> const_iterator;

  /**
   * Constructs a new ObMap with Privilege_Latest behavior.
   */
  ObMap ()
      : null_k (Viz::keynullret ()),
        null_v (Viz::valnullret ()),
        dup_key_behavior (Privilege_Latest)
  {
  }

  /**
   * Create an empty map which grows geometrically, using \a multiplier.
   */
  explicit ObMap (float64 multiplier)
      : our_map (multiplier),
        null_k (Viz::keynullret ()),
        null_v (Viz::valnullret ()),
        dup_key_behavior (Privilege_Latest)
  {
  }

  /**
   * Copies the entries from \a otha, but the DupKeyBehavior()
   * is not copied; it is set to Privilege_Latest.
   */
  ObMap (const ObMap &otha)
      : null_k (Viz::keynullret ()),
        null_v (Viz::valnullret ()),
        dup_key_behavior (Privilege_Latest)
  {
    int64 cnt = otha.Count ();
    our_map.EnsureRoomFor (cnt);
    for (int64 q = 0; q < cnt; q++)
      if (MapCons *mc = otha.NthCons (q))
        our_map.Append (new MapCons (mc->Car (), mc->Cdr ()));
      else
        our_map.Append (NULL);
  }

  /**
   * Moves the entries from \a otha, and moves the DupKeyBehavior ().
   */
  ObMap (ObMap &&otha)
      : our_map (std::move (otha.our_map)),
        null_k (Viz::keynullret ()),
        null_v (Viz::valnullret ()),
        dup_key_behavior (otha.dup_key_behavior)
  {
    otha.dup_key_behavior = Privilege_Latest;
  }


  ~ObMap ()
  {
    ObCrawl<MapCons *> mort = our_map.Crawl ();
    while (!mort.isempty ())
      if (MapCons *mc = mort.popaft ())
        delete mc;
  }

  void Delete () { delete this; }


  /**
   * Returns the number of entries in the ObMap.
   */
  int64 Count () const { return our_map.Count (); }

  /**
   * Return the additive amount by which the map would grow if it
   * were necessary (and if that amount exceeds the multiplicative
   * growth's).
   */
  unt32 ArithmeticGrowthFactor () const
  {
    return our_map.ArithmeticGrowthFactor ();
  }

  /**
   * Return the multiplicative factor by which the map will grow when
   * necessary (and if that size exceeds the additive growth version).
   */
  float64 GeometricGrowthFactor () const
  {
    return our_map.GeometricGrowthFactor ();
  }

  /**
   * Return the map's current capacity.
   */
  int64 Capacity () const { return our_map.Capacity (); }

  /**
   * Return the future capacity of the map (upon next enlargement).
   */
  int64 NextLargerCapacity () const { return our_map.NextLargerCapacity (); }

  /**
   * Configure how this map will expand.  When the map needs to grow,
   * the new capacity is set to either capacity times
   * \a geom_mult, or capacity plus \a arith_incr, whichever is greater.
   *
   * \code
   *     To grow arithmetically by increments of 8:
   *       SetGrowthFactors (8);
   *
   *     To grow geometrically by factors of 2:
   *       SetGrowthFactors (1, 2.0);
   *
   *     To grow by 10%, but at least by 16 elements:
   *       SetGrowthFactors (16, 1.1);
   * \endcode
   *
   * \a s must be at least 1.  If it is not, OB_INVALID_ARGUMENT
   * is returned.
   */
  ObRetort SetGrowthFactors (unt32 arith_incr, float64 geom_mult = 1.0)
  {
    return our_map.SetGrowthFactors (arith_incr, geom_mult);
  }

  /**
   * Make sure the map has room for at least \a num entries.
   */
  ObRetort EnsureRoomFor (int64 num) { return our_map.EnsureRoomFor (num); }


  /**
   * Returns the Dup_Key_Behavior that was set with SetDupKeyBehavior().
   */
  Dup_Key_Behavior DupKeyBehavior () const { return dup_key_behavior; }

  /**
   * Changes the behavior of the ObMap to behave
   * as a write-once map, a standard map, or a multimap.
   */
  void SetDupKeyBehavior (Dup_Key_Behavior dkb) { dup_key_behavior = dkb; }

  /**
   * Changes the behavior of the ObMap to behave as a write-once map.
   */
  void PrivilegeEarliest () { dup_key_behavior = Privilege_Earliest; }

  /**
   * Changes the behavior of the ObMap to behave as a standard map.
   */
  void PrivilegeLatest () { dup_key_behavior = Privilege_Latest; }

  /**
   * Changes the behavior of the ObMap to behave as a multimap.
   */
  void AllowDuplicates () { dup_key_behavior = Allow_Duplicates; }


  /**
   * Adds a new key-value pair to the ObMap.  If the key already
   * exists, the behavior is dictated by DupKeyBehavior().
   */
  ObRetort Put (KeyARGType k, ValARGType v)
  {
    if (dup_key_behavior == Allow_Duplicates)
      return our_map.Append (new MapCons (k, v));
    else if (MapCons *mc = ConsFromKey (k))
      {
        if (dup_key_behavior == Privilege_Latest)
          mc->SetCdr (v);
        return OB_OK;
      }
    else
      return our_map.Append (new MapCons (k, v));
  }


  /**
   * Places a new key-value pair in the ObMap at index 'ind'.
   * If the key already exists, the behavior is dictated by
   * DupKeyBehavior().  (note that if the key already exists
   * and the DupKeyBehavior() is set to 'Privilege_Latest',
   * the cons will effectively get 'moved' inside the map;
   * if 'Privilege_Earliest', then the existing cons is not
   * moved...)
   */
  ObRetort PutAtIndex (KeyARGType k, ValARGType v, int64 ind)
  {
    int64 cnt = our_map.Count ();
    if (ind < 0)
      {
        if (ind < -cnt)
          ind = 0;
        else
          ind += cnt;
      }
    else if (ind > cnt)
      ind = cnt;
    if (dup_key_behavior == Allow_Duplicates)
      return our_map.Insert (new MapCons (k, v), ind);
    else if (MapCons *mc = ConsFromKey (k))
      {
        if (dup_key_behavior == Privilege_Latest)
          {
            if (ind == cnt)
              ind = cnt - 1;
            int64 cur_ind = our_map.Find (mc);
            mc->SetCdr (v);
            if (cur_ind != ind)
              {
                our_map.Remove (mc);
                our_map.Insert (mc, ind);
              }
          }
        return OB_OK;
      }
    else
      return our_map.Insert (new MapCons (k, v), ind);
  }


  /**
   * returns the ordinal at which the key appears (or -1 if absent)
   */
  int64 IndexForKey (KeyARGType k) const
  {
    MapCons *mc;
    for (int64 q = our_map.Count () - 1; q >= 0; q--)
      if ((mc = our_map.Nth (q)) && (k == mc->Car ()))
        return q;
    return -1;
  }

  /**
   * Returns true if \a k is a key in the ObMap.
   */
  bool KeyIsPresent (KeyARGType k) const { return (IndexForKey (k) >= 0); }
  bool ContainsKey (KeyARGType k) const { return (IndexForKey (k) >= 0); }


  /**
   * returns the ordinal at which the val appears (or -1 if absent)
   */
  int64 IndexForVal (ValARGType v) const
  {
    MapCons *mc;
    for (int64 q = our_map.Count () - 1; q >= 0; q--)
      if ((mc = our_map.Nth (q)) && (v == mc->Cdr ()))
        return q;
    return -1;
  }

  /**
   * Returns true if \a v is a value in the ObMap.
   */
  bool ValIsPresent (ValARGType v) const { return (IndexForVal (v) >= 0); }
  bool ContainsVal (ValARGType v) const { return (IndexForVal (v) >= 0); }


  /**
   * Looks up the key \a k in the map, and if it is present, returns the
   * cons for that entry.
   */
  MapCons *ConsFromKey (KeyARGType k) const
  {
    int64 q, num = our_map.Count ();
    MapCons *mc;
    for (q = 0; q < num; q++)
      if ((mc = our_map.Nth (q)) && (k == mc->Car ()))
        return mc;
    return NULL;
  }

  /**
   * Looks up the value \a v in the map, and if it is present, returns the
   * cons for that entry.
   */
  MapCons *ConsFromVal (ValARGType v) const
  {
    int64 q, num = our_map.Count ();
    MapCons *mc;
    for (q = 0; q < num; q++)
      if ((mc = our_map.Nth (q)) && (v == mc->Cdr ()))
        return mc;
    return NULL;
  }


  /**
   * Returns the \a n th entry of the map.
   */
  MapCons *NthCons (int64 n) const { return our_map.Nth (n); }

  /**
   * Returns the key (car) of the \a ind th entry of the map.
   */
  KeyACCESSType NthKey (int64 ind) const
  {
    if (MapCons *mc = our_map.Nth (ind))
      return mc->Car ();
    return (null_k = Viz::keynullret ());
  }

  /**
   * Returns the value (cdr) of the \a ind th entry of the map.
   */
  ValACCESSType NthVal (int64 ind) const
  {
    if (MapCons *mc = our_map.Nth (ind))
      return mc->Cdr ();
    return (null_v = Viz::valnullret ());
  }


  /**
   * If \a k is a key in the map, this will set \a v to the corresponding
   * value and return OB_OK.  If not found, returns OB_NOT_FOUND. Note: This
   * does not modify the underlying map.
   */
  ObRetort FillValFromKey (KeyARGType k, VALType &v) const
  {
    MapCons *mc = ConsFromKey (k);
    if (mc)
      {
        v = mc->Cdr ();
        return OB_OK;
      }
    return OB_NOT_FOUND;
  }

  /**
   * If \a k is a key in the map, returns the corresponding
   * value.  If not found, returns the "null value".
   */
  ValACCESSType ValFromKey (KeyARGType k) const
  {
    if (MapCons *mc = ConsFromKey (k))
      return mc->Cdr ();
    return (null_v = Viz::valnullret ());
  }

  /**
   * Synonym for ValFromKey().
   */
  inline ValACCESSType Find (KeyARGType k) const { return ValFromKey (k); }


  /**
   * If \a v is a value in the map, sets \a k to the corresponding
   * key and returns OB_OK.  If not found, returns OB_NOT_FOUND.
   */
  ObRetort FillKeyFromVal (ValARGType v, KEYType &k) const
  {
    MapCons *mc = ConsFromVal (v);
    if (mc)
      {
        k = mc->Car ();
        return OB_OK;
      }
    return OB_NOT_FOUND;
  }

  /**
   * If \a v is a value in the map, returns the corresponding
   * key.  If not found, returns the "null key".
   */
  KeyACCESSType KeyFromVal (ValARGType v) const
  {
    if (MapCons *mc = ConsFromVal (v))
      return mc->Car ();
    return (null_k = Viz::keynullret ());
  }

  /**
   * If \a k is a key in the map, removes that entry and returns OB_OK.
   * If not found, returns OB_NOT_FOUND.
   */
  ObRetort RemoveByKey (KeyARGType k)
  {
    MapCons *mc = ConsFromKey (k);
    if (mc)
      {
        our_map.Remove (mc);
        delete mc;
        return OB_OK;
      }
    return OB_NOT_FOUND;
  }

  /**
   * Synonym for RemoveByKey().
   */
  inline ObRetort Remove (KeyARGType k) { return RemoveByKey (k); }


  /**
   * If \a v is a value in the map, removes that entry and returns OB_OK.
   * If not found, returns OB_NOT_FOUND.
   */
  ObRetort RemoveByVal (ValARGType v)
  {
    MapCons *mc = ConsFromVal (v);
    if (mc)
      {
        our_map.Remove (mc);
        delete mc;
        return OB_OK;
      }
    return OB_NOT_FOUND;
  }

  /**
   * Removes the \a ind th entry from the map.  Returns OB_OK if
   * successful, or OB_BAD_INDEX if \a ind was out of range.
   */
  ObRetort RemoveNthCons (int64 ind)
  {
    MapCons *mc = NULL;  // not even going to wait for complaints
    ObRetort tort;
    if (OB_OK != (tort = our_map.SafeNth (ind, mc)))
      return tort;
    our_map.RemoveNth (ind);
    delete mc;
    return OB_OK;
  }

  /**
   * Synonym for RemoveNthCons().
   */
  inline ObRetort RemoveNth (int64 ind) { return RemoveNthCons (ind); }


  /**
   * Removes any entries whose keys are null.
   * Returns the number of entries removed, or -1 if the key
   * type is not a pointer type.
   */
  int64 CompactNullKeys ()
  {
    if (!Helpy::key_is_pointy)
      return -1;
    MapCons *mc;
    for (int64 q = our_map.Count () - 1; q >= 0; q--)
      if ((mc = our_map.Nth (q)) && !mc->Car ())
        {
          our_map.NullifyNth (q);
          delete mc;
        }
    if (our_map.IsHoldingNullifiedElements ())
      return our_map.CompactNulls ();
    return 0;
  }

  /**
   * Removes any entries whose values are null.
   * Returns the number of entries removed, or -1 if the value
   * type is not a pointer type.
   */
  int64 CompactNullVals ()
  {
    if (!Helpy::val_is_pointy)
      return -1;
    MapCons *mc;
    for (int64 q = our_map.Count () - 1; q >= 0; q--)
      if ((mc = our_map.Nth (q)) && !mc->Cdr ())
        {
          our_map.NullifyNth (q);
          delete mc;
        }
    if (our_map.IsHoldingNullifiedElements ())
      return our_map.CompactNulls ();
    return 0;
  }

  /**
   * Removes any entries which are null.
   * XXX: How would that happen, exactly?
   */
  int64 CompactNulls () { return our_map.CompactNulls (); }

  /**
   * Removes all entries from the map.
   */
  void Empty ()
  {
    ObCrawl<MapCons *> mort = our_map.Crawl ();
    while (!mort.isempty ())
      if (MapCons *mc = mort.popaft ())
        delete mc;
    our_map.Empty ();
  }

  /**
   * a very raw sort and quicksort option for ObMap-- the function pointer
   * passed in receives two "map cons" pointers, with which it does whatever
   * it may need to do...
   *
   * note that, because of the many-splendored delights of the type system,
   * comparator functions will need to limn arguments like this:
   *
   *  int comppy (ObCons <K, V> * const &a, ...)
   */

  template <typename CMPFUNQ>
  struct KeyCmp
  {
    CMPFUNQ cmp;
    KeyCmp (const CMPFUNQ &c) : cmp (c) {}
    int operator() (MapCons *const &a, MapCons *const &b) const
    {
      return cmp (a->Car (), b->Car ());
    }
  };
  template <typename CMPFUNQ>
  struct ValCmp
  {
    CMPFUNQ cmp;
    ValCmp (const CMPFUNQ &c) : cmp (c) {}
    int operator() (MapCons *const &a, MapCons *const &b) const
    {
      return cmp (a->Cdr (), b->Cdr ());
    }
  };

  template <typename CMPFUNQ>
  void Sort (CMPFUNQ cmp, int64 left = 0, int64 right = -1)
  {
    our_map.Sort (cmp, left, right);
  }

  template <typename CMPFUNQ>
  void Quicksort (CMPFUNQ cmp, int64 left = 0, int64 right = -1)
  {
    our_map.Quicksort (cmp, left, right);
  }

  /**
   * sort (or quicksort) an ObMap with respect to its keys.
   * again, type system shambolicisms require that the comparator
   * function's arguments will have an 'extra' const and &, like e.g.
   *
   *  int keycomppy (const int64 &key_a, ...)
   */
  template <typename CMPFUNQ>
  void SortByKey (CMPFUNQ cmp, int64 left = 0, int64 right = -1)
  {
    our_map.Sort (KeyCmp<CMPFUNQ> (cmp), left, right);
  }

  template <typename CMPFUNQ>
  void QuicksortByKey (CMPFUNQ cmp, int64 left = 0, int64 right = -1)
  {
    our_map.Quicksort (KeyCmp<CMPFUNQ> (cmp), left, right);
  }


  /**
   * quicksort (or sort) an ObMap with respect to its vals.
   * once more, the riproar of the type system means that the comparator
   * function's arguments will have an 'extra' const and &, like e.g.
   *
   *  int valcomppy (const Str &val_a, ...)
   */

  template <typename CMPFUNQ>
  void SortByVal (CMPFUNQ cmp, int64 left = 0, int64 right = -1)
  {
    our_map.Sort (ValCmp<CMPFUNQ> (cmp), left, right);
  }

  template <typename CMPFUNQ>
  void QuicksortByVal (CMPFUNQ cmp, int64 left = 0, int64 right = -1)
  {
    our_map.Quicksort (ValCmp<CMPFUNQ> (cmp), left, right);
  }


  /**
   * very disappointing. the compiler should have autogenerated an
   * assignment operator that properly called our sole instance
   * variable's assignment operator... but it does not. so we take
   * bjarne's matters into our own hands.
   */
  ObMap<KEYType, VALType, KEY_MEM_MGR_TAG, VAL_MEM_MGR_TAG> &operator= (
    const ObMap<KEYType, VALType, KEY_MEM_MGR_TAG, VAL_MEM_MGR_TAG> &otha)
  {
    if (this == &otha)
      return *this;

    Empty ();
    int64 cnt = otha.Count ();
    our_map.EnsureRoomFor (cnt);
    for (int64 q = 0; q < cnt; q++)
      if (MapCons *mc = otha.NthCons (q))
        our_map.Append (new MapCons (mc->Car (), mc->Cdr ()));
      else
        our_map.Append (NULL);

    return *this;
  }

  /**
   * Sets this instance to have the exact same entries as
   * \a otha.  However, the DupKeyBehavior() for this instance
   * remains unchanged.
   */
  ObMap<KEYType, VALType, KEY_MEM_MGR_TAG, VAL_MEM_MGR_TAG> &
  CopyFrom (const ObMap<KEYType, VALType> &otha)
  {
    Empty ();
    int64 cnt = otha.Count ();
    our_map.EnsureRoomFor (cnt);
    for (int64 q = 0; q < cnt; q++)
      if (MapCons *mc = otha.NthCons (q))
        our_map.Append (new MapCons (mc->Car (), mc->Cdr ()));
      else
        our_map.Append (NULL);
    return *this;
  }

  ObMap &operator= (ObMap &&otha) noexcept
  {
    if (this == &otha)
      return *this;

    our_map = std::move (otha.our_map);
    dup_key_behavior = otha.dup_key_behavior;
    otha.dup_key_behavior = Privilege_Latest;

    return *this;
  }

  /**
   * Returns an ObCrawl that iterates over the entries of this map.
   */
  ObCrawl<MapCons *> Crawl () const { return our_map.Crawl (); }

  /**
   * For compatibility with C++11 range-based for loop.  Beware that
   * what is returned is not really a proper STL iterator; it is just
   * good enough to make the for loop work.
   */
  const_iterator begin () const { return our_map.begin (); }

  /**
   * For compatibility with C++11 range-based for loop.
   */
  const_iterator end () const { return our_map.end (); }



  /**
   * \cond INTERNAL
   */
  class OC_MapKeyGuts : public ObCrawl<KEYType>::OC_Guts
  {
   OB_PRIVATE:
    ObCrawl<MapCons *> cons_crawl;

   public:
    OC_MapKeyGuts (const ObMap *om) : cons_crawl (om->Crawl ()) {}
    OC_MapKeyGuts (const ObCrawl<MapCons *> &other_crawl)
        : cons_crawl (other_crawl)
    {
    }
    OC_MapKeyGuts (const OC_MapKeyGuts &other_mkg)
        : cons_crawl (other_mkg.cons_crawl)
    {
    }
    bool IsEmpty () const override { return cons_crawl.isempty (); }
    KeyCRAWLRetType PopFore () override
    {
      return cons_crawl.popfore ()->Car ();
    }
    KeyCRAWLRetType PopAft () override { return cons_crawl.popaft ()->Car (); }
    KeyCRAWLRetType Fore () const override
    {
      return cons_crawl.fore ()->Car ();
    }
    KeyCRAWLRetType Aft () const override { return cons_crawl.aft ()->Car (); }
    void Reload () override { cons_crawl.reload (); }
    typename ObCrawl<KEYType>::OC_Guts *Dup () const override
    {
      return new OC_MapKeyGuts (cons_crawl);
    }
  };
  /** \endcond */

  /** Returns an ObCrawl that iterates over the keys of this map. */
  ObCrawl<KEYType> CrawlKeys () const
  {
    return ObCrawl<KEYType> (new OC_MapKeyGuts (this));
  }



  /**
   * \cond INTERNAL
   */
  class OC_MapValGuts : public ObCrawl<VALType>::OC_Guts
  {
   OB_PRIVATE:
    ObCrawl<MapCons *> cons_crawl;

   public:
    OC_MapValGuts (const ObMap *om) : cons_crawl (om->Crawl ()) {}
    OC_MapValGuts (const ObCrawl<MapCons *> &other_crawl)
        : cons_crawl (other_crawl)
    {
    }
    OC_MapValGuts (const OC_MapValGuts &other_mkg)
        : cons_crawl (other_mkg.cons_crawl)
    {
    }
    bool IsEmpty () const override { return cons_crawl.isempty (); }
    ValCRAWLRetType PopFore () override
    {
      return cons_crawl.popfore ()->Cdr ();
    }
    ValCRAWLRetType PopAft () override { return cons_crawl.popaft ()->Cdr (); }
    ValCRAWLRetType Fore () const override
    {
      return cons_crawl.fore ()->Cdr ();
    }
    ValCRAWLRetType Aft () const override { return cons_crawl.aft ()->Cdr (); }
    void Reload () override { cons_crawl.reload (); }
    typename ObCrawl<VALType>::OC_Guts *Dup () const override
    {
      return new OC_MapValGuts (cons_crawl);
    }
  };
  /** \endcond */

  /** Returns an ObCrawl that iterates over the values of this map. */
  ObCrawl<VALType> CrawlVals () const
  {
    return ObCrawl<VALType> (new OC_MapValGuts (this));
  }
};
}
}  // end namespaces loam, oblong...


#endif
