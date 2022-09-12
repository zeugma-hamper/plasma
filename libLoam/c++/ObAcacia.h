
/* (c)  oblong industries */

#ifndef OB_ACACIA_SILHOUETTE_ON_THE_VELDT
#define OB_ACACIA_SILHOUETTE_ON_THE_VELDT


#include <libLoam/c++/ObRetort.h>

#include <libLoam/c++/ObRef.h>
#include <libLoam/c++/ObWeakRef.h>
#include <libLoam/c++/ObCrawl.h>
#include <libLoam/c++/CrawlIterator.h>

#include <utility>

namespace oblong {
namespace loam {


#include "OT_Functionary.h"


/**
 * An ObAcacia is a sorted collection of things; in implementation,
 * it's a self-balancing binary tree, of the avl-implementation variety.
 * So it's an analogue to ObUniqueTrove (in that each element may appear
 * only once) but features o(log(n)) search, insertion, and deletion.
 * The element type must have a no-argument constructor, a copy
 * constructor, an operator<, and an operator==.
 *
 * Of important note: ObAcacia may not play nicely with weak references.
 * In order to function efficiently (i.e. without prohibitively
 * obsessive checking), a binary tree needs to be able to assume that
 * it's always properly sorted. but with an ObWeakRef, which can
 * suddenly change from a meaningful pointer to zero (null), we violate
 * that assumption.
 */
template <typename ELEMType,
          template <typename DUMMY> class MEM_MGR_CLASS = UnspecifiedMemMgmt>
class ObAcacia
{

#define WITH_MEM_MANAGEMENT_SENSITIVITY
#include "OT_Helpy.h"
#undef WITH_MEM_MANAGEMENT_SENSITIVITY

  typedef OT_FUNCTIONARY::Videlicet___<Helpy::is_pointy, Helpy::to_be_wrapped,
                                       Helpy::is_constructorless, ELEMType,
                                       ACCESSType, STOREDType, ARGType>
    Viz;

  typedef typename ObCrawl<ELEMType>::OC_Guts OC_Guts;

 OB_PRIVATE:
  struct Noad
  {
    Noad *ell, *arr;
    Noad *upp;
    unt16 dep;
    STOREDType paylo;
    Noad () : ell (NULL), arr (NULL), upp (NULL), dep (1) {}
    Noad (ELEMType e) : ell (NULL), arr (NULL), upp (NULL), dep (1), paylo (e)
    {
    }
    explicit Noad (const Noad &n)
        : ell (n.ell ? new Noad (*n.ell) : NULL),
          arr (n.arr ? new Noad (*n.arr) : NULL),
          upp (NULL),
          dep (n.dep),
          paylo (n.paylo)

    {
      if (ell)
        ell->upp = this;
      if (arr)
        arr->upp = this;
    }
  };

  Noad *ruut;
  int64 num_elems;

 public:
  typedef CrawlIterator<ELEMType, CRAWLRetType> iterator;
  typedef CrawlIterator<ELEMType, CRAWLRetType> const_iterator;

  ObAcacia () noexcept : ruut (NULL), num_elems (0) {}

  ObAcacia (const ObAcacia &otha)
      : ruut (otha.ruut ? new Noad (*otha.ruut) : NULL),
        num_elems (otha.num_elems)
  {
  }

  ObAcacia (ObAcacia &&otha) noexcept : ruut (NULL), num_elems (0)
  {
    std::swap (ruut, otha.ruut);
    std::swap (num_elems, otha.num_elems);
  }

  ~ObAcacia () { Empty (); }


  ObAcacia &operator= (const ObAcacia &otha)
  {
    if (this == &otha)
      return *this;

    Empty ();

    if (otha.ruut)
      {
        ruut = new Noad (*otha.ruut);
        num_elems = otha.num_elems;
      }

    return *this;
  }

  ObAcacia &operator= (ObAcacia &&otha) noexcept
  {
    if (this == &otha)
      return *this;

    ObAcacia tmp (std::move (*this));

    std::swap (ruut, otha.ruut);
    std::swap (num_elems, otha.num_elems);

    return *this;
  }

  /**
   * Provided purely for lexical equivalence to the AnkleObject classes
   * ('purely' because this method is, in contrast to the AO case,
   * semantically identical to operator-delete, and, thus, not strictly
   * necessary).
   */
  void Delete () { delete this; }

  /**
   * Remove all elements from the acacia.
   */
  void Empty ()
  {
    if (!ruut)
      return;

    ObTrove<Noad *, NoMemMgmt> noads;
    noads.EnsureRoomFor (num_elems);

    //get to a leaf and work our way back up to root (err...ruut)
    Noad *curn = ruut;
    while (curn)
      {
        if (curn->arr)
          curn = curn->arr;
        else if (curn->ell)
          curn = curn->ell;
        else
          {
            noads.Append (curn);
            if (curn->upp == NULL)  //at root, bail
              {
                break;
              }
            else if (curn->upp->ell == curn)  //remove self
              {
                curn->upp->ell = NULL;
                curn = curn->upp;
              }
            else if (curn->upp->arr == curn)  //remove self
              {
                curn->upp->arr = NULL;
                curn = curn->upp;
              }
          }
      }

    for (int64 i = 0; i < num_elems; ++i)
      delete noads.Nth (i);

    ruut = NULL;
    num_elems = 0;
  }

  /**
   * Return the number of elements in this acacia.
   */
  inline int64 Count () const { return num_elems; }

  /**
   * Return the current depth of the tree (unlike other libraries'
   * approach to abstracting collection classes, we don't pretend that
   * we don't know this to be a binary search teee; the depth is thus
   * useful and usable information).
   */
  inline int64 Depth () const { return (ruut ? ruut->dep : 0); }

  /**
   * By analogy with ObTrove's Find(), return a zero-or-greater integer
   * if \a elem is found the acacia, and a negative integer if it's
   * absent. For the moment, the only 'elem-is-present' return value
   * is positive unity -- the method is saying only that \a elem is
   * present, but nothing about where; the 'position' of an element
   * within a binary search tree is far less semantically meaningful
   * than is the index of an element in a linear array (which is the
   * meaning ascribed to ObTrove::Find()'s return value).
   */
  int64 Find (ARGType elem) const { return (Contains (elem) ? 1 : -1); }

  /**
   * Return true if the provided element is found in the acacia,
   * and, obviously, false if not.
   */
  bool Contains (ARGType elem) const
  {
    Noad *enn = ruut;
    while (enn)
      {
        ACCESSType payl = Viz::access (enn->paylo);
        if (elem == payl)
          return true;
        if (elem < payl)
          enn = enn->ell;
        else
          enn = enn->arr;
      }
    return false;
  }


  static void _ReEndepthen (Noad *n, int32 extra_carefulness = 0)
  {
    unt16 l, r;
    for (; n; n = n->upp)
      {
        l = (n->ell ? n->ell->dep : 0);
        r = (n->arr ? n->arr->dep : 0);
        if (l > r)
          r = l;
        if (++r != n->dep)
          n->dep = r;
        else if (extra_carefulness >= 0)
          if (--extra_carefulness < 0)
            break;
      }
  }

  void _PossiblyRebalance (Noad *n, bool keep_on = false)
  {
    while (n)
      {
        int32 bal = (n->arr ? n->arr->dep : 0) - (n->ell ? n->ell->dep : 0);
        if (bal == 1 || bal == 0 || bal == -1)
          {
            n = n->upp;
            continue;
          }
        Noad *B, *C, *D, *E, *A = n;
        Noad *mom = n->upp;
        Noad **lnk = &ruut;
        if (mom)
          {
            if (mom->ell == A)
              lnk = &(mom->ell);
            else
              lnk = &(mom->arr);
          }
        if (bal < 0)  // bal == -2
          {
            B = A->ell;
            C = B->arr;
            if ((n->ell->arr ? n->ell->arr->dep : 0)
                  - (n->ell->ell ? n->ell->ell->dep : 0)
                <= 0)  // hard left
              {
                *lnk = B;
                B->upp = mom;
                B->arr = A;
                A->upp = B;
                A->ell = C;
                if (C)
                  C->upp = A;
                _ReEndepthen (A, 1);
                n = B->upp;
              }
            else  // dogleg
              {
                D = C->ell;
                E = C->arr;
                *lnk = C;
                C->upp = mom;
                C->ell = B;
                B->upp = C;
                B->arr = D;
                if (D)
                  D->upp = B;
                C->arr = A;
                A->upp = C;
                A->ell = E;
                if (E)
                  E->upp = A;
                _ReEndepthen (B);
                _ReEndepthen (A, 1);
                n = C->upp;
              }
          }
        else  // bal == 2
          {
            B = A->arr;
            C = B->ell;
            if ((n->arr->arr ? n->arr->arr->dep : 0)
                  - (n->arr->ell ? n->arr->ell->dep : 0)
                >= 0)  // pure right
              {
                *lnk = B;
                B->upp = mom;
                B->ell = A;
                A->upp = B;
                A->arr = C;
                if (C)
                  C->upp = A;
                _ReEndepthen (A, 1);
                n = B->upp;
              }
            else  // leg of dog
              {
                D = C->arr;
                E = C->ell;
                *lnk = C;
                C->upp = mom;
                C->arr = B;
                B->upp = C;
                B->ell = D;
                if (D)
                  D->upp = B;
                C->ell = A;
                A->upp = C;
                A->arr = E;
                if (E)
                  E->upp = A;
                _ReEndepthen (B);
                _ReEndepthen (A, 1);
                n = C->upp;
              }
          }
        if (!keep_on)
          break;
      }
  }

  /**
   * Add \a elem to the acacia. Return OB_OK if the appendment was
   * successful, or OB_ALREADY_PRESENT if \a elem was already extant
   * in the acacia. Internally, of course, this action may well
   * invole rebalancing the tree, after the AVL fashion.
   */
  ObRetort Append (ARGType elem)
  {
    if (!ruut)
      {
        ruut = new Noad (elem);
        ++num_elems;
        return OB_OK;
      }
    Noad *down, *curn = ruut;
    while (true)
      {
        ACCESSType payl = Viz::access (curn->paylo);
        if (elem == payl)
          return OB_ALREADY_PRESENT;
        if (elem < payl)
          {
            if (!(down = curn->ell))
              {
                curn->ell = (down = new Noad (elem));
                down->upp = curn;
                if (!curn->arr)
                  {
                    _ReEndepthen (curn);
                    _PossiblyRebalance (curn);
                  }
                ++num_elems;
                return OB_OK;
              }
          }
        else
          {
            if (!(down = curn->arr))
              {
                curn->arr = (down = new Noad (elem));
                down->upp = curn;
                if (!curn->ell)
                  {
                    _ReEndepthen (curn);
                    _PossiblyRebalance (curn);
                  }
                ++num_elems;
                return OB_OK;
              }
          }
        curn = down;
      }
  }

  /**
   * Search the acacia for an element equal to \a elem: if found,
   * remove it and return OB_OK; if not found, return OB_NOT_FOUND.
   */
  ObRetort Remove (ARGType elem)
  {
    Noad *enn = ruut;
    Noad *mom = NULL;
    Noad **lnk = &ruut;
    while (true)
      {
        if (!enn)
          return OB_NOT_FOUND;
        ACCESSType payl = Viz::access (enn->paylo);
        if (elem == payl)
          break;
        mom = enn;
        if (elem < payl)
          {
            lnk = &(enn->ell);
            enn = enn->ell;
          }
        else
          {
            lnk = &(enn->arr);
            enn = enn->arr;
          }
      }

    // found it. now to whack it, complexly...
    if (enn->ell)
      if (enn->arr)  // ell and arr
        {
          Noad *nei, *dwn, *prn, *chl;
          if (enn->ell->dep < enn->arr->dep)
            {
              nei = enn->arr;
              while ((dwn = nei->ell))
                nei = dwn;
              prn = (enn == nei->upp ? NULL : nei->upp);
              chl = nei->arr;
              if (prn)
                prn->ell = chl;
            }
          else
            {
              nei = enn->ell;
              while ((dwn = nei->arr))
                nei = dwn;
              prn = (enn == nei->upp ? NULL : nei->upp);
              chl = nei->ell;
              if (prn)
                prn->arr = chl;
            }
          if (nei != enn->ell)
            {
              nei->ell = enn->ell;
              enn->ell->upp = nei;
            }
          if (nei != enn->arr)
            {
              nei->arr = enn->arr;
              enn->arr->upp = nei;
            }
          nei->upp = mom;
          *lnk = nei;
          if (prn)
            {
              if (chl)
                chl->upp = prn;
              if (prn->upp == enn)
                prn->upp = nei;
              _ReEndepthen (prn, 1);
              _ReEndepthen (nei);
              _PossiblyRebalance (prn, true);
            }
          else
            {
              _ReEndepthen (nei, 1);
              _PossiblyRebalance (nei, true);
            }
        }
      else  // ell but not arr
        {
          *lnk = enn->ell;
          enn->ell->upp = mom;
          _ReEndepthen (mom);
          _PossiblyRebalance (mom, true);
        }
    else if (enn->arr)  // arr but not ell
      {
        *lnk = enn->arr;
        enn->arr->upp = mom;
        _ReEndepthen (mom);
        _PossiblyRebalance (mom, true);
      }
    else  // neither ell nor arr
      {
        *lnk = NULL;
        _ReEndepthen (mom);
        _PossiblyRebalance (mom, true);
      }

    delete enn;
    --num_elems;
    return OB_OK;
  }


  /**
   * Return an ObCrawl (on the stack) that can be used to traverse
   * this acacia.
   */
  ObCrawl<ELEMType> Crawl () const
  {
    return ObCrawl<ELEMType> (new OC_AcaciaGuts (ruut, Depth ()));
  }

  /**
   * Return an ObCrawl (allocated from the heap) that can be used to
   * traverse this acacia. The heapiness of the returned object means,
   * naturally, that you'll need to take responsibility for freeing
   * it once it's outlived its usefulness.
   */
  ObCrawl<ELEMType> *HeapyCrawl () const
  {
    return new ObCrawl<ELEMType> (new OC_AcaciaGuts (ruut, Depth ()));
  }

  /**
   * For compatibility with C++11 range-based for loop.  Beware that
   * what is returned is not really a proper STL iterator; it is just
   * good enough to make the for loop work.
   */
  const_iterator begin () const
  {
    return const_iterator (new OC_AcaciaGuts (ruut, Depth ()));
  }

  /**
   * For compatibility with C++11 range-based for loop.
   */
  const_iterator end () const { return const_iterator (); }


  class OC_AcaciaGuts : public OC_Guts
  {
    PATELLA_SUBCLASS (OC_AcaciaGuts, OC_Guts);

   public:
    struct Gutlet
    {
      Noad *nob;
      bool used;
    };
    mutable Gutlet *lstack, *rstack;
    mutable Noad *l, *r;
    Noad *ur_nob;
    mutable int32 lind, rind;
    int32 ur_dep;

   private:
    OC_AcaciaGuts () {}  // won't be making one of these by accident. nope.
   public:
    OC_AcaciaGuts (Noad *rooty, unt16 dpth)
        : lstack (NULL),
          rstack (NULL),
          l (rooty),
          r (rooty),
          ur_nob (rooty),
          ur_dep (dpth)
    {
    }
    ~OC_AcaciaGuts () override
    {
      delete[] lstack;
      delete[] rstack;
    }
    inline void Inflate () const  // oy for sure...
    {
      lstack = new Gutlet[ur_dep + 1];
      rstack = new Gutlet[ur_dep + 1];
      lstack[lind = 0].nob = (l = ur_nob);
      lstack[0].used = false;
      rstack[rind = 0].nob = (r = ur_nob);
      rstack[0].used = false;
      while (Noad *dn = l->ell)
        {
          lstack[++lind].used = false;
          lstack[lind].nob = l = dn;
        }
      while (Noad *dn = r->arr)
        {
          rstack[++rind].used = false;
          rstack[rind].nob = r = dn;
        }
    }
#define ASSERTFLATE()                                                          \
  do                                                                           \
    {                                                                          \
      assert (!IsEmpty ());                                                    \
      if (!lstack)                                                             \
        Inflate ();                                                            \
    }                                                                          \
  while (0)

    bool IsEmpty () const override { return (l == NULL); }
    CRAWLRetType PopFore () override
    {
      ASSERTFLATE ();
      Noad *retty = l;
      if (l == r)  // we're done; empty; kaput
        l = NULL;
      else
        {
          lstack[lind].used = true;
          if (l->arr)  // down-and-to-the-right is still untraversed
            {
              lstack[++lind].nob = (l = l->arr);
              lstack[lind].used = false;
              while (Noad *dn = l->ell)
                {
                  lstack[++lind].nob = l = dn;
                  lstack[lind].used = false;
                }
            }
          else  // now we need to climb back up
            do
              {
                l = lstack[--lind].nob;
              }
            while (lstack[lind].used);
        }
      return Viz::access (retty->paylo);
    }
    CRAWLRetType PopAft () override
    {
      ASSERTFLATE ();
      Noad *retty = r;
      if (l == r)  // we're done; empty; kaput
        l = NULL;
      else
        {
          rstack[rind].used = true;
          if (r->ell)  // down-and-to-the-left is waiting for us
            {
              rstack[++rind].nob = (r = r->ell);
              rstack[rind].used = false;
              while (Noad *dn = r->arr)
                {
                  rstack[++rind].nob = r = dn;
                  rstack[rind].used = false;
                }
            }
          else  // now we need to climb back up
            do
              {
                r = rstack[--rind].nob;
              }
            while (rstack[rind].used);
        }
      return Viz::access (retty->paylo);
    }
    CRAWLRetType Fore () const override
    {
      ASSERTFLATE ();
      return Viz::access (l->paylo);
    }
    CRAWLRetType Aft () const override
    {
      ASSERTFLATE ();
      return Viz::access (r->paylo);
    }
#undef ASSERTFLATE
    void Reload () override
    {
      l = r = ur_nob;
      if (ur_nob && !lstack)
        Inflate ();
      else if (ur_nob)
        {
          lstack[lind = 0].nob = (l = ur_nob);
          lstack[0].used = false;
          rstack[rind = 0].nob = (r = ur_nob);
          rstack[0].used = false;
          while (Noad *dn = l->ell)
            {
              lstack[++lind].used = false;
              lstack[lind].nob = l = dn;
            }
          while (Noad *dn = r->arr)
            {
              rstack[++rind].used = false;
              rstack[rind].nob = r = dn;
            }
        }
    }
    typename ObCrawl<ELEMType>::OC_Guts *Dup () const override
    {
      return new OC_AcaciaGuts (ur_nob, ur_dep);
    }
  };
};
}
}  // time to snuff it for namespaces loam and oblong


#endif
