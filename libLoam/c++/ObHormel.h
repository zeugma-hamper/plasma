
/* (c)  oblong industries */

#ifndef OB_HORMEL_IS_TASTIER_THAN_SCRAPPLE
#define OB_HORMEL_IS_TASTIER_THAN_SCRAPPLE


#include "ObHasher.h"
#include "OH_Functionary.h"

#include <libLoam/c++/ObCrawl.h>

#include <libLoam/c/ob-coretypes.h>


#define MAX_LOAD_FACTOR 0.75


namespace oblong {
namespace loam {


/**
 * An ObHormel is an unordered set of things, otherwise known as a hash
 * set.  The element must have a specialization of Hormel (see Hormel.h)
 * or have a function with the signature "size_t hash_value (const
 * ELEMType &)" in its namespace.  Some types are already provided for,
 * in particular pointers (hashed by address) and basic types.  For the
 * full list see Hormel.h.
 */
template <class ELEMType,
          template <typename> class MEM_MGR_CLASS = UnspecifiedMemMgmt,
          class Hash = ObHasher<ELEMType>>
class ObHormel
{
 private:
#define WITH_MEM_MANAGEMENT_SENSITIVITY
#include "OT_Helpy.h"
#undef WITH_MEM_MANAGEMENT_SENSITIVITY

  typedef OH_FUNCTIONARY::Videlicet___<Helpy::is_pointy, Helpy::to_be_wrapped,
                                       ELEMType, ACCESSType, STOREDType,
                                       ARGType>
    Viz;

  template <typename ST>
  class LinkedList
  {
   public:
    ST key;
    size_t hash;
    LinkedList *next;

    LinkedList (ARGType v, size_t h)
    {
      Viz::assign (key, v);
      hash = h;
      next = NULL;
    }

    LinkedList *Find (ARGType v)
    {
      if (v == Viz::access (key))
        return this;

      LinkedList *tmp = next;
      while (tmp && v != Viz::access (key))
        tmp = tmp->next;

      return tmp;
    }

    //that is, this -> next -> key == v
    LinkedList *FindPrior (ARGType v)
    {
      if (!next)
        return NULL;

      if (v == Viz::access (next->key))
        return this;

      LinkedList *tmp = next;
      while (tmp->next && v != Viz::access (tmp->next->key))
        tmp = tmp->next;

      if (!tmp->next)
        return NULL;

      return tmp;
    }

    // that is, this -> next = n
    LinkedList *FindPriorNode (LinkedList *n)
    {
      if (!next || !n)
        return NULL;

      if (n == next)
        return this;

      LinkedList *ltmp = next;
      while (ltmp && ltmp->next != n)
        ltmp = ltmp->next;

      return ltmp;
    }

    LinkedList *Last ()
    {
      if (!next)
        return this;

      LinkedList *ltmp = next;
      while (ltmp->next)
        ltmp = ltmp->next;

      return ltmp;
    }

    int64 Count ()
    {
      int64 c = 0;
      LinkedList *ltmp = next;
      while (ltmp)
        {
          ++c;
          ltmp = ltmp->next;
        }

      return c + 1;
    }
  };  // end 'struct LinkedList'


  typedef LinkedList<STOREDType> STLinkedList;


 OB_PROTECTED:
  int64 num_buckets;
  int64 num_entries;
  Hash hasher;
  mutable STLinkedList **buckets;

 public:
  typedef CrawlIterator<ELEMType, CRAWLRetType> iterator;
  typedef CrawlIterator<ELEMType, CRAWLRetType> const_iterator;

  /**
   * Creates an empty ObHormel.  ObHormel roughly doubles its size
   * (sticking to primes) when it grows past MAX_LOAD_FACTOR.
   */
  ObHormel (const Hash &h = Hash ())
      : num_buckets (7),
        num_entries (0),
        hasher (h),
        buckets (
          (STLinkedList **) calloc (num_buckets, sizeof (STLinkedList *)))
  {
  }

  /**
   * Creates an empty ObHormel capable of holding \a size
   * elements without resizing.
   */
  ObHormel (int64 size, const Hash &h = Hash ())
      : num_buckets (BucketSizeForEntries (size, MAX_LOAD_FACTOR)),
        num_entries (0),
        hasher (h),
        buckets (
          (STLinkedList **) calloc (num_buckets, sizeof (STLinkedList *)))
  {
  }

  /**
   * Create an ObHormel from another ObHormel.
   */
  ObHormel (const ObHormel &oh)
      : num_buckets (oh.num_buckets),
        num_entries (oh.num_entries),
        hasher (oh.hasher)

  {
    if (!(buckets =
            (STLinkedList **) calloc (num_buckets, sizeof (STLinkedList *))))
      {
        OB_LOG_ERROR ("malloc has failed ObHormel::ObHormel "
                      "(const ObHormel &)!");
        throw std::bad_alloc ();
      }

    BlindlyCopyFrom (oh);
  }

  ObHormel (ObHormel &&oh) noexcept (
    std::is_nothrow_move_constructible<Hash>::value)
      : num_buckets (oh.num_buckets),
        num_entries (oh.num_entries),
        hasher (std::move (oh.hasher)),
        buckets (oh.buckets)
  {
    oh.num_buckets = 0;
    oh.num_entries = 0;
    oh.buckets = NULL;
  }

  ~ObHormel ()
  {
    Empty ();
    num_buckets = 0;
    free (buckets);
  }

  void Delete () { delete this; }

  /**
   * Add \a elem to the set.
   */
  ob_retort Append (ARGType elem)
  {
    size_t h = hasher (elem);
    size_t slot = num_buckets != 0 ? h % num_buckets : 0;
    STLinkedList *ll = 0;

    if (num_buckets != 0 && (ll = FindAndDetach (slot, elem)))
      {
        ll->next = buckets[slot];
        buckets[slot] = ll;
        return OB_NOTHING_TO_DO;
      }
    else
      {
        ob_retort ret = OB_OK;
        if (ShouldUpsizePreemptively ())
          {
            if (OB_OK != (ret = ResizeFor (num_buckets + 1)))
              return ret;

            slot = h % num_buckets;
          }
        STLinkedList *lnew = new STLinkedList (elem, h);
        lnew->next = buckets[slot];
        buckets[slot] = lnew;
        ++num_entries;
        return OB_OK;
      }

    return OB_OK;
  }

  /**
   * Checks to see if \a elem is in the set.  It returns 1 if the item
   * is in the set and -1 if not.
   */
  int64 Find (ARGType elem) const { return (Contains (elem) ? 1 : -1); }


  /**
   * Checks to see if \a elem is in the set. Returns true if so, false
   * otherwise.
   */
  bool Contains (ARGType elem) const
  {
    if (num_entries == 0 || num_buckets == 0)
      return false;

    size_t slot = hasher (elem) % num_buckets;

    STLinkedList *ll = FindAndDetach (slot, elem);
    if (!ll)
      return false;

    ll->next = buckets[slot];
    buckets[slot] = ll;
    return true;
  }

  /**
   * Removes \a elem from the set.  It returns OB_OK if \a elem was in
   * the set and removed, and return OB_NOT_FOUND if \a elem was not in
   * the set.
   */
  ob_retort Remove (ARGType elem)
  {
    if (num_entries == 0 || num_buckets == 0)
      return OB_NOT_FOUND;

    size_t slot = hasher (elem) % num_buckets;
    STLinkedList *ll = FindAndDetach (slot, elem);

    if (!ll)
      return OB_NOT_FOUND;

    delete ll;
    --num_entries;
    return OB_OK;
  }

  /**
   * Removes every element from the set.  It does not reduce the number of
   * buckets in the set.
   */
  ob_retort Empty ()
  {
    num_entries = 0;
    for (int64 i = 0; i < num_buckets; ++i)
      {
        for (STLinkedList **cur = &buckets[i]; *cur;)
          {
            STLinkedList *here = *cur;
            *cur = here->next;
            delete here;
          }
        buckets[i] = NULL;
      }
    return OB_OK;
  }

  /**
   * Resizes the set so that having \a num items in the set won't trigger a
   * resize.  It will not shrink the set.  Returns OB_NOTHING_TO_DO if
   * the set size is already sufficient or OB_OK if the operation worked.
   */
  ob_retort EnsureRoomFor (int64 num)
  {
    //make sure that having num entries won't trigger a resize
    if (num_buckets != 0 && num / (double) num_buckets < MAX_LOAD_FACTOR)
      return OB_NOTHING_TO_DO;

    int64 b_count = BucketSizeForEntries (num, MAX_LOAD_FACTOR);
    return ResizeFor (b_count);
  }

  /**
   * Empties the set, resizes the set to the same size as \a oh, and copies
   * over the items.
   */
  ObHormel &operator= (const ObHormel &oh)
  {
    if (this == &oh)
      return *this;

    Empty ();
    num_buckets = oh.num_buckets;
    num_entries = oh.num_entries;
    hasher = oh.hasher;
    if (num_buckets > 0)
      {
        buckets =
          (STLinkedList **) realloc (buckets,
                                     num_buckets * sizeof (STLinkedList *));
        memset (buckets, 0, num_buckets * sizeof (STLinkedList *));

        BlindlyCopyFrom (oh);
      }
    else
      {
        free (buckets);
        buckets = NULL;
      }

    return *this;
  }

  ObHormel &operator= (ObHormel &&oh) noexcept (
    std::is_nothrow_move_assignable<Hash>::value)
  {
    if (this == &oh)
      return *this;

    ObHormel tmp (std::move (*this));

    std::swap (num_buckets, oh.num_buckets);
    std::swap (num_entries, oh.num_entries);
    hasher = std::move (oh.hasher);
    std::swap (buckets, oh.buckets);

    return *this;
  }

  /**
   * Removes any NULL elements from the set.  This function only works
   * if the elements are pointers.  Since this is a set (only one of an
   * item allowed) you'd expect that there could only be one NULL
   * pointer in the set, but Oblong memory management provides weak refs
   * which may become NULL if the ref count drops to zero.
   * Returns how many entries were removed.
   */
  int64 CompactNulls ()
  {
    Viz::__OBERR_Cannot_Use_CompactNulls_With_Non_Pointer_Types__ ();
    int64 count = 0;
    for (int64 i = 0; i < num_buckets; ++i)
      {
        for (STLinkedList **cur = &buckets[i]; *cur;)
          {
            STLinkedList *here = *cur;
            if (NULL == Viz::access (here->key))
              {
                *cur = here->next;
                delete here;
                --num_entries;
                ++count;
              }
            else
              cur = &here->next;
          }
      }

    return count;
  }


  /**
   * Returns the number of entries in the set.
   */
  int64 Count () const { return EntryCount (); }

  /**
   * Returns the number of entries in the set.
   */
  int64 EntryCount () const { return num_entries; }


  /**
   * Returns the number of buckets.  BucketCount () > EntryCount ().
   */
  int64 BucketCount () const { return num_buckets; }


  /**
   * Returns the load factor of the hash set, defined as the number of
   * entries divided by the number of buckets or zero if the number of
   * buckets is zero.
   */
  float64 LoadFactor () const
  {
    return num_buckets == 0 ? 0.0 : num_entries / (float64) num_buckets;
  }

 OB_PROTECTED:
  /*
   * Checks for elem in a bucket.  If elem is found, the linked list
   * node is removed from the bucket and returned.
   */
  STLinkedList *FindAndDetach (int64 bucket, ARGType elem) const
  {
    if (bucket >= num_buckets || bucket < 0)
      return 0;

    for (STLinkedList **cur = &buckets[bucket]; *cur;)
      {
        STLinkedList *entry = *cur;
        if (Viz::access (entry->key) == elem)
          {
            *cur = entry->next;
            return entry;
          }
        else
          cur = &entry->next;
      }

    return 0;
  }

  /*
   * Checks whether adding another element would trigger and upsizing of
   * the hash set.
   */
  bool ShouldUpsizePreemptively () const
  {
    return num_buckets == 0
           || ((num_entries + 1) / (float64) num_buckets) > MAX_LOAD_FACTOR;
  }

  /*
   * Creates NextBucketSize(bucket_count) buckets and moves element over to
   * the new buckets.
   */
  ob_retort ResizeFor (int64 bucket_count)
  {
    int64 new_bucket_count = NextBucketSize (bucket_count);
    STLinkedList **new_buckets =
      (STLinkedList **) calloc (new_bucket_count, sizeof (STLinkedList *));


    STLinkedList **old_buckets = buckets;
    int64 num_old_buckets = num_buckets;

    buckets = new_buckets;
    num_entries = 0;
    num_buckets = new_bucket_count;

    for (int64 i = 0; i < num_old_buckets; ++i)
      {
        for (STLinkedList *cur = old_buckets[i]; cur;)
          {
            STLinkedList *here = cur;
            cur = here->next;
            int64 slot = here->hash % num_buckets;
            here->next = buckets[slot];
            buckets[slot] = here;
            ++num_entries;
          }
      }

    free (old_buckets);

    return OB_OK;
  }

  /*
   * Copies elements from oh into this ObHormel.  Assumes that the this
   * ObHormel is the same size as the incoming ObHormel.
   */
  void BlindlyCopyFrom (const ObHormel &oh)
  {
    STLinkedList **ins_ptr;
    for (int64 i = 0; i < num_buckets; ++i)
      {
        ins_ptr = &buckets[i];
        for (STLinkedList *cur = oh.buckets[i]; cur;)
          {
            STLinkedList *here = cur;
            cur = here->next;

            STLinkedList *st =
              new STLinkedList (Viz::access (here->key), here->hash);
            *ins_ptr = st;
            ins_ptr = &(st->next);
          }
      }
  }

  typedef typename ObCrawl<ELEMType>::OC_Guts OC_Guts;
  class OC_HormelGuts : public OC_Guts
  {
    PATELLA_SUBCLASS (OC_HormelGuts, OC_Guts);

   OB_PRIVATE:
    STLinkedList **buckets;
    int64 num_buckets;
    int64 fn_bucket, ln_bucket;  //firstnode_bucket, lastnode_bucket
    STLinkedList *fn, *ln;       //firstnode, lastnode
    int64 frn_bucket,
      arn_bucket;             //forwardnoderange_bucket, aftnoderange_bucket
    STLinkedList *frn, *arn;  //forwardrangenode, aftrangenode
   private:
    OC_HormelGuts () {}

   public:
    OC_HormelGuts (STLinkedList **b, int64 num_b)
        : buckets (b),
          num_buckets (num_b),
          fn_bucket (0),
          ln_bucket (num_b - 1),
          fn (NULL),
          ln (NULL)
    {
      for (; fn_bucket <= ln_bucket && !buckets[fn_bucket]; ++fn_bucket)
        {
        }

      fn = (fn_bucket <= ln_bucket) ? buckets[fn_bucket] : NULL;
      if (!fn)
        {
          ln = fn = frn = arn = NULL;
          fn_bucket = ln_bucket = 0;
          frn_bucket = ln_bucket + 1;
          arn_bucket = fn_bucket - 1;
        }
      else
        {
          for (; ln_bucket >= fn_bucket && !buckets[ln_bucket]; --ln_bucket)
            {
            }
          ln = buckets[ln_bucket]->Last ();
          frn_bucket = fn_bucket;
          frn = fn;
          arn_bucket = ln_bucket;
          arn = ln;
        }
    }

    OC_HormelGuts (STLinkedList **b, int64 num_b, int64 firstnode_bucket,
                   int64 lastnode_bucket, STLinkedList *firstnode,
                   STLinkedList *lastnode, int64 forwardrangenode_bucket,
                   int64 aftrangenode_bucket, STLinkedList *forwardrangenode,
                   STLinkedList *aftrangenode)
        : buckets (b),
          num_buckets (num_b),
          fn_bucket (firstnode_bucket),
          ln_bucket (lastnode_bucket),
          frn_bucket (forwardrangenode_bucket),
          arn_bucket (aftrangenode_bucket),
          frn (forwardrangenode),
          arn (aftrangenode)
    {
    }
    bool IsEmpty () const override
    {
      return (arn_bucket < frn_bucket
              || (frn_bucket == arn_bucket && arn->next == frn));
    }
    CRAWLRetType PopFore () override
    {
      assert (!IsEmpty ());

      STLinkedList *frn_tmp = frn;
      if (frn->next)
        frn = frn->next;
      else
        {
          frn_bucket++;
          while (frn_bucket <= ln_bucket && !buckets[frn_bucket])
            frn_bucket++;
          if (frn_bucket <= ln_bucket)
            frn = buckets[frn_bucket];
        }

      return Viz::access (frn_tmp->key);
    }
    CRAWLRetType PopAft () override
    {
      assert (!IsEmpty ());
      STLinkedList *arn_tmp = arn;

      if (buckets[arn_bucket] != arn)
        arn = buckets[arn_bucket]->FindPriorNode (arn);
      else
        {
          arn_bucket--;
          while (arn_bucket >= fn_bucket && !buckets[arn_bucket])
            arn_bucket--;
          if (arn_bucket >= fn_bucket)
            {
              arn = buckets[arn_bucket];
              if (arn)
                arn = arn->Last ();
            }
        }

      return Viz::access (arn_tmp->key);
    }
    CRAWLRetType Fore () const override
    {
      assert (!IsEmpty ());
      return Viz::access (frn->key);
    }
    CRAWLRetType Aft () const override
    {
      assert (!IsEmpty ());
      return Viz::access (arn->key);
    }
    void Reload () override
    {
      arn = ln;
      frn = fn;
      frn_bucket = fn_bucket;
      arn_bucket = ln_bucket;
    }
    typename ObCrawl<ELEMType>::OC_Guts *Dup () const override
    {
      return new OC_HormelGuts (buckets, num_buckets, fn_bucket, ln_bucket, fn,
                                ln, frn_bucket, arn_bucket, frn, arn);
    }
  };

 public:
  ObCrawl<ELEMType> Crawl () const
  {
    return ObCrawl<ELEMType> (new OC_HormelGuts (buckets, num_buckets));
  }
  ObCrawl<ELEMType> *HeapyCrawl () const
  {
    return new ObCrawl<ELEMType> (new OC_HormelGuts (buckets, num_buckets));
  }

  /**
   * For compatibility with C++11 range-based for loop.  Beware that
   * what is returned is not really a proper STL iterator; it is just
   * good enough to make the for loop work.
   */
  const_iterator begin () const
  {
    return const_iterator (new OC_HormelGuts (buckets, num_buckets));
  }

  /**
   * For compatibility with C++11 range-based for loop.
   */
  const_iterator end () const { return const_iterator (); }


 private:
  /*
   * Calculates how many buckets are needed to fit num_entries elements
   * in the set without triggering a resize given max_load_factor.
   */
  static int64 BucketSizeForEntries (int64 num_entries, float64 max_load_factor)
  {
    if (max_load_factor == 0.0)
      return num_entries;

    return NextBucketSize ((num_entries / max_load_factor));
  }

  /*
   * Returns the next appropriate size for set given the current size.
   * Each size is a prime and is approximately double the previous size.
   */
  static int64 NextBucketSize (int64 cur)
  {
    static const int64 sizes[] = {7,
                                  17,
                                  29,
                                  59,
                                  113,
                                  227,
                                  449,
                                  907,
                                  1801,
                                  3593,
                                  7177,
                                  14341,
                                  28687,
                                  57347,
                                  114689,
                                  229393,
                                  458789,
                                  917513,
                                  1835017,
                                  3670027,
                                  7340033,
                                  14680067,
                                  29360147,
                                  58720267,
                                  117440551,
                                  234881033,
                                  469762049,
                                  939524129,
                                  1879048201,
                                  OB_CONST_I64 (3758096411),
                                  OB_CONST_I64 (7516192771),
                                  OB_CONST_I64 (15032385569),
                                  OB_CONST_I64 (30064771081),
                                  OB_CONST_I64 (60129542171),
                                  OB_CONST_I64 (120259084301),
                                  OB_CONST_I64 (240518168603),
                                  OB_CONST_I64 (481036337167),
                                  OB_CONST_I64 (962072674313),
                                  OB_CONST_I64 (1924145348627),
                                  OB_CONST_I64 (3848290697227)};

    static const int64 count = 40;
    for (int64 i = 0; i < count; ++i)
      {
        if (sizes[i] > cur)
          return sizes[i];
      }

    int64 ns = 2 * sizes[count - 1];
    while (ns < cur)
      ns *= 2;

    return ns;
  }
};

template <typename T, template <typename> class M, typename H>
struct ObHasher<oblong::loam::ObHormel<T, M, H>>
{
  ObHasher<T> h;
  inline std::size_t operator() (const oblong::loam::ObHormel<T, M> &hm) const
  {
    oblong::loam::ObCrawl<T> cr = hm.Crawl ();
    if (cr.isempty ())
      return 0;

    std::size_t hash = 0;
    while (!cr.isempty ())
      hash += h (cr.popfore ());

    return ob_hash_size_t (hash);
  }
};
}
}


#endif
