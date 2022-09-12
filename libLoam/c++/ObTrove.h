
/* (c)  oblong industries */

#ifndef OB_TROVE_AU_JUS
#define OB_TROVE_AU_JUS


#include <stdlib.h>
#include <stdio.h>

#include <assert.h>

#include <new>  // you have to be kidding, right? nope.
#include <utility>

#include <libLoam/c++/ObRetort.h>

#include <libLoam/c++/ObRef.h>
#include <libLoam/c++/ObWeakRef.h>
#include <libLoam/c++/ObCrawl.h>
#include <libLoam/c++/CrawlIterator.h>

namespace oblong {
namespace loam {


//
// the following must dwell here b'cuz we can't (mayn't) have partial
// specializations inside the class scope. wouldn't that be nice, though?
//
#include "OT_Functionary.h"

/**
 * An ObTrove is an ordered collection of things.  It is what other
 * libraries would call a list or a vector.  (Though calling it a
 * vector might confuse it with a mathematical vector, which it is
 * not.)  The element type must have a no-argument constructor, a copy
 * constructor, and an operator==.
 */
template <typename ELEMType,
          template <typename DUMMY> class MEM_MGR_CLASS = UnspecifiedMemMgmt>
class ObTrove
{
//
// first: some slight assistance...
//

#define WITH_MEM_MANAGEMENT_SENSITIVITY
#include "OT_Helpy.h"
#undef WITH_MEM_MANAGEMENT_SENSITIVITY

  /**
   * \cond INTERNAL
   * Hey, here's something important to know if you are in the
   * unfortunate position of trying to figure out how this code
   * works: despite the name, Viz::assign is essentially the
   * COPY CONSTRUCTOR, not the assignment operator.  In other words,
   * you should call it to copy something to memory which has
   * NOT BEEN INITIALIZED YET, but not to assign to an existing
   * instance.  But you should use Viz::assign instead of the
   * actual copy constructor, because the actual copy constructor
   * can have issues on obscure types, like bug 2693, which
   * Viz::assign somehow avoids.
   *
   * Though I still haven't figured out what "Viz" and "Videlicet"
   * stand for, or why the trailing triple underscore.  Some things
   * just remain a mystery.
   */
  typedef OT_FUNCTIONARY::Videlicet___<Helpy::is_pointy, Helpy::to_be_wrapped,
                                       Helpy::is_constructorless, ELEMType,
                                       ACCESSType, STOREDType, ARGType>
    Viz;

  typedef typename ObCrawl<ELEMType>::OC_Guts OC_Guts;

/**
 * This macro performs two basic functions:
 * - checks whether the index \a idx is in range, and returns \a fail
 *   if it is not
 * - if \a idx is negative, modifies it to be positive, such that
 *   -1 names the last element, -2 names the next to last element, etc.
 *
 * \a op should nearly always be ">=", for methods that want to act upon
 * existing elements.  The only exception is Insert(), which uses plain
 * ">", because it wants to allow insertion at the index past the last
 * element.
 *
 * It's important that this macro first checks where the index falls,
 * and then does the math, rather than mixing the two, to avoid a
 * funny theoretical signed overflow problem (which should never occur
 * in practice anyway, since an int64 should never overflow for our
 * purposes here) that occurs when algebra and computer math differ,
 * and the compiler starts worrying that its optimizations may not be
 * correct.  For more details, see bug 886.
 */
#define OB_CHECK_INDEX(idx, op, fail)                                          \
  if ((idx) < 0)                                                               \
    {                                                                          \
      if ((idx) < -num_p)                                                      \
        return (fail);                                                         \
      else                                                                     \
        (idx) += num_p;                                                        \
    }                                                                          \
  else if ((idx) op num_p)                                                     \
  return (fail)

#define OB_TR_MAX(a, b) ((a) < (b) ? (b) : (a))

  //
  // and now for ObTrove proper.
  //

 OB_PRIVATE:
  int64 num_p, capac;
  float64 growth_factor;
  unt32 inc_size;
  bool holding_nullified_elements;
  STOREDType *pees;
  mutable ELEMType nully;
  constexpr static int MIN_CAP = 4;

  int64 Bigger () const
  {
    if (capac <= 0)
      return MIN_CAP;
    const int64 geom = static_cast<int64> (capac * growth_factor);
    const int64 arith = capac + inc_size;
    return OB_TR_MAX (geom, arith);
  }
  /* \endcond */


  void SwapInternals (ObTrove &otha) noexcept
  {
    std::swap (num_p, otha.num_p);
    std::swap (capac, otha.capac);
    std::swap (growth_factor, otha.growth_factor);
    std::swap (inc_size, otha.inc_size);
    std::swap (holding_nullified_elements, otha.holding_nullified_elements);
    std::swap (pees, otha.pees);
  }

 public:
  typedef CrawlIterator<ELEMType, CRAWLRetType> iterator;
  typedef CrawlIterator<ELEMType, CRAWLRetType> const_iterator;

  /**
   * Creates an empty trove which will grow arithmetically with
   * an increment size of 8.
   */
  ObTrove ()
      : num_p (0),
        capac (0),
        growth_factor (1.0),
        inc_size (8),
        holding_nullified_elements (false),
        pees (NULL),
        nully (Viz::nullret ())
  {
  }

  /**
   * Creates an empty trove which grows geometrically rather than
   * arithmetically.  2147483647 out of 2147483648 computer scientists
   * agree that this is a more scalable approach.  A popular value for
   * \a multiplier is 2.0.
   */
  explicit ObTrove (float64 multiplier)
      : num_p (0),
        capac (0),
        growth_factor (multiplier),
        inc_size (8),
        holding_nullified_elements (false),
        pees (NULL),
        nully (Viz::nullret ())
  {
  }

  /**
   * Construct a trove from another trove with contents similar enough to copy
   */
  template <typename OtherELEMType,
            template <typename DUMMY> class OTHER_MEM_MGR_CLASS>
  ObTrove (const ObTrove<OtherELEMType, OTHER_MEM_MGR_CLASS> &otha)
      : num_p (otha.Count ()),
        capac (OB_TR_MAX (MIN_CAP, otha.Count ())),
        growth_factor (otha.GeometricGrowthFactor ()),
        inc_size (otha.ArithmeticGrowthFactor ()),
        holding_nullified_elements (otha.IsHoldingNullifiedElements ()),
        nully (Viz::nullret ())
  {
    if (!(pees = (STOREDType *) malloc (capac * sizeof (STOREDType))))
      {
        OB_LOG_ERROR_CODE (0x11050000,
                           "true horror: malloc failed in "
                           "ObTrove::ObTrove(const ObTrove &)...\n");
        throw std::bad_alloc ();
      }
    for (int64 q = 0; q < num_p; q++)
      Viz::assign (pees, q, otha.Nth (q));
  }

  /**
   * Constructs a trove identical to \a otha.
   */
  ObTrove (const ObTrove &otha)
      : num_p (otha.num_p),
        capac (OB_TR_MAX (MIN_CAP, otha.num_p)),
        growth_factor (otha.growth_factor),
        inc_size (otha.inc_size),
        holding_nullified_elements (otha.holding_nullified_elements),
        nully (Viz::nullret ())
  {
    if (!(pees = (STOREDType *) malloc (capac * sizeof (STOREDType))))
      {
        OB_LOG_ERROR_CODE (0x11050001,
                           "true horror: malloc failed in "
                           "ObTrove::ObTrove(const ObTrove &)...\n");
        throw std::bad_alloc ();
      }
    for (int64 q = 0; q < num_p; q++)
      Viz::assign (pees, q, Viz::access (otha.pees[q]));
  }


  // this could possibly be marked noexcept if nully copy or move ctor is
  // noexcept but noexcept (nully (Viz::nullret())) takes the call literally
  // rather than figuring out what ctor the initializer actually calls
  ObTrove (ObTrove &&otha) : ObTrove () { SwapInternals (otha); }

  /**
   * Constructs a trove from a C array \a array of length \a count.
   */
  ObTrove (const ELEMType *array, int64 count)
      : num_p (count),
        capac (OB_TR_MAX (MIN_CAP, count)),
        growth_factor (1.0),
        inc_size (8),
        holding_nullified_elements (false),
        nully (Viz::nullret ())
  {
    if (!(pees = (STOREDType *) malloc (capac * sizeof (STOREDType))))
      {
        OB_LOG_ERROR_CODE (0x11050002,
                           "true horror: malloc failed in "
                           "ObTrove::ObTrove(ELemType *array, int64 count)"
                           "...\n");
        throw std::bad_alloc ();
      }
    for (int64 q = 0; q < num_p; q++)
      Viz::assign (pees, q, array[q]);
  }


  virtual ~ObTrove ()
  {
    if (pees)
      {
        if (num_p > 0)
          {
            STOREDType *p = pees + num_p;
            num_p = 0;
            while (1)
              {
                p--;
                p->~STOREDType ();
                if (p == pees)
                  break;
              }
          }
        free (pees);
        pees = NULL;
      }
  }

  void Delete () { delete this; }


  /**
   * Returns the number of elements in this trove.
   */
  int64 Count () const { return num_p; }


  /**
   * Searches the trove for an element equal to \a elem, and
   * returns its index, or -1 if not found.
   * \note If more than one element matches, this
   * returns the match at the lowest-number index.
   */
  int64 Find (ARGType elem) const
  {
    int64 q;
    for (q = 0; q < num_p; q++)
      if (elem == Viz::access (pees[q]))
        return q;
    return -1;
  }

  /**
   * Return true if \a elem is present in the trove.
   */
  bool Contains (ARGType elem) const { return (Find (elem) >= 0); }


  /**
   * Returns the \a ind th element of the trove, or a null
   * value if \a ind is out of range.
   * \a ind may either be a nonnegative integer, in which case
   * it means the obvious thing, or it can be a negative integer,
   * in which case it means count from the end (i. e. -1 means the
   * last element).
   */
  //NOTE: that nasty comma operator after the assignment is there
  //to avoid a problem with qt4's QVector::operator= (QVector &&)
  //which returns a value rather than a reference. qt5 has fixed
  //this problem. await dependent projects updates.
  ACCESSType Nth (int64 ind) const
  {
    OB_CHECK_INDEX (ind, >=, (nully = Viz::nullret (), nully));
    return Viz::access (pees[ind]);
  }


  /**
   * Sets \a into to the \a ind th element of the trove
   * and returns OB_OK if \a ind is in range.  Returns
   * OB_BAD_INDEX if \a ind is out of range.
   * \a ind may either be a nonnegative integer, in which case
   * it means the obvious thing, or it can be a negative integer,
   * in which case it means count from the end (i. e. -1 means the
   * last element).
   */
  ObRetort SafeNth (int64 ind, ELEMType &into) const
  {
    OB_CHECK_INDEX (ind, >=, OB_BAD_INDEX);
    into = Viz::access (pees[ind]);
    return OB_OK;
  }


  /**
   * really for pointy things -- won't work with objects...
   * \a ind may either be a nonnegative integer, in which case
   * it means the obvious thing, or it can be a negative integer,
   * in which case it means count from the end (i. e. -1 means the
   * last element).
   */
  STOREDType NthObRef (int64 ind) const
  {
    Viz::__OBERR_Cannot_Use_ObRef_With_Non_Pointer_Types__ ();
    OB_CHECK_INDEX (ind, >=, STOREDType (NULL));
    return STOREDType (pees[ind]);
  }


  /**
   * Appends \a elem to the end of the trove.  Returns OB_OK
   * on success.  Throws std::bad_alloc if memory can't be allocated.
   */
  virtual ObRetort Append (ARGType elem)
  {
    if (num_p >= capac)
      EnsureRoomFor (Bigger ());
    Viz::assign (pees, num_p++, elem);
    return OB_OK;
  }


  /**
   * Appends each element of \a other. Returns OB_OK on sucess.
   * Throws std::bad_alloc if memory can't be allocated.
   */
  virtual ObRetort Append (const ObTrove<ELEMType, MEM_MGR_CLASS> &other)
  {
    EnsureRoomFor (Count () + other.Count ());

    const int64 count = other.Count ();
    for (int64 q = 0; q < count; q++)
      Viz::assign (pees, num_p++, other.Nth (q));

    return OB_OK;
  }


  /**
   * Inserts \a elem at index \a ind in the trove.  Returns OB_OK
   * on success.  Throws std::bad_alloc if memory can't be allocated.
   * \a ind may either be a nonnegative integer, in which case
   * it means the obvious thing, or it can be a negative integer,
   * in which case it means count from the end (i. e. -1 means the
   * last element).  In either case, if \a ind is out of range, by
   * being either too positive or too negative, then OB_BAD_INDEX is
   * returned.
   */
  virtual ObRetort Insert (ARGType elem, int64 ind)
  {
    int64 q;
    OB_CHECK_INDEX (ind, >, OB_BAD_INDEX);
    if (num_p >= capac)
      EnsureRoomFor (Bigger ());
    new (pees + (q = num_p)) STOREDType ();
    for (; q > ind; q--)
      pees[q] = pees[q - 1];
    pees[ind] = elem;
    num_p++;
    return OB_OK;
  }


  /**
   * Searches the trove for an element equal to \a elem, and
   * removes it from the trove and returns OB_OK.  If not
   * found, returns OB_NOT_FOUND.
   * \note As of 2 january 2012, the search order for removal proceeds
   * just like Find()'s, Nullify()'s, etc.'s; which is to say: forward,
   * beginning with the lowest index. (Formerly, the removal search
   * went backwards.)
   */
  ObRetort Remove (ARGType elem)
  {
    for (int64 q = 0; q < num_p; q++)
      if (elem == Viz::access (pees[q]))
        {
          for (--num_p; q < num_p; q++)
            pees[q] = pees[q + 1];
          pees[num_p].~STOREDType ();
          return OB_OK;
        }
    return OB_NOT_FOUND;
  }


  /**
   * remove every element that matches. returns number removed.
   */
  int64 RemoveEvery (ARGType elem)
  {
    int32 gap = 0;
    STOREDType *fleet, *stolid, *finis;
    finis = (fleet = stolid = pees) + num_p;
    while (fleet < finis)
      {
        if (elem == Viz::access (*fleet))
          {
            ++gap;
            ++fleet;
            continue;
          }
        if (gap > 0)
          *stolid = *fleet;
        ++stolid;
        ++fleet;
      }
    while (stolid < finis)
      {
        stolid->~STOREDType ();
        ++stolid;
      }
    num_p -= gap;
    return gap;
  }


  /**
   * If \a ind is in range, replaces the \a ind th element
   * with \a elem and returns \a OB_OK.  If \a ind is not
   * in range, returns OB_BAD_INDEX.
   * \a ind may either be a nonnegative integer, in which case
   * it means the obvious thing, or it can be a negative integer,
   * in which case it means count from the end (i. e. -1 means the
   * last element).
   */
  virtual ObRetort ReplaceNth (int64 ind, ARGType elem)
  {
    OB_CHECK_INDEX (ind, >=, OB_BAD_INDEX);
    pees[ind] = elem;
    return OB_OK;
  }


  /**
   * Removes the last element of the trove, and returns OB_OK.
   * If the trove is empty, returns OB_EMPTY.
   */
  ObRetort PopAft ()
  {
    if (num_p > 0)
      {
        pees[--num_p].~STOREDType ();
        return OB_OK;
      }
    return OB_EMPTY;
  }


  /**
   * Removes the \a ind th element of the trove, and returns
   * OB_OK.  If \a ind is out of range, returns OB_BAD_INDEX.
   * \a ind may either be a nonnegative integer, in which case
   * it means the obvious thing, or it can be a negative integer,
   * in which case it means count from the end (i. e. -1 means the
   * last element).
   */
  ObRetort RemoveNth (int64 ind)
  {
    OB_CHECK_INDEX (ind, >=, OB_BAD_INDEX);
    for (--num_p; ind < num_p; ind++)
      pees[ind] = pees[ind + 1];
    pees[num_p].~STOREDType ();
    return OB_OK;
  }


  /**
   * Sets the \a N th element to null, and returns OB_OK.
   * If \a N is out of range, returns OB_BAD_INDEX.
   * \a N may either be a nonnegative integer, in which case
   * it means the obvious thing, or it can be a negative integer,
   * in which case it means count from the end (i. e. -1 means the
   * last element).
   */
  ObRetort NullifyNth (int64 N)
  {
    Viz::__OBERR_Cannot_Use_NullifyNth_With_Non_Pointer_Types__ ();
    OB_CHECK_INDEX (N, >=, OB_BAD_INDEX);
    pees[N] = STOREDType ();
    holding_nullified_elements = true;
    return OB_OK;
  }

  /**
   * Searches the trove for an element equal to \a elem, and
   * sets it to null.  Returns OB_OK on success, or OB_NOT_FOUND
   * on failure.
   * \note If more than one element matches, nullifies the match at
   * the lowest-number index.  (Like Find().)
   */
  ObRetort Nullify (ARGType elem)
  {
    int64 ind = Find (elem);
    if (ind < 0)
      return OB_NOT_FOUND;
    return NullifyNth (ind);
  }


  /**
   * Searches the trove for elements equal to \a elem1 and
   * \a elem2.  If both are found, swaps them and returns
   * OB_OK.  If either is not found, returns OB_NOT_FOUND.
   * \note For both \a elem1 and \a elem2, if more than one
   * element matches, uses the match at the lowest-number index.
   */
  ObRetort SwapElems (ARGType elem1, ARGType elem2)
  {
    int64 pos1, pos2;
    if (((pos1 = Find (elem1)) < 0) || ((pos2 = Find (elem2)) < 0))
      return OB_NOT_FOUND;
    STOREDType tempy = pees[pos1];
    pees[pos1] = pees[pos2];
    pees[pos2] = tempy;
    return OB_OK;
  }


  /**
   * Swaps the element at \a ind1 with the element at \a ind2.
   * Returns OB_OK on success, or OB_BAD_INDEX if either element
   * is out of range.  For both \a ind1 and \a ind2, negative indices
   * work the same way they do elsewhere in this API.
   */
  ObRetort SwapElemsAt (int64 ind1, int64 ind2)
  {
    OB_CHECK_INDEX (ind1, >=, OB_BAD_INDEX);
    OB_CHECK_INDEX (ind2, >=, OB_BAD_INDEX);
    STOREDType tempeh = pees[ind1];
    pees[ind1] = pees[ind2];
    pees[ind2] = tempeh;
    return OB_OK;
  }


  /**
   * Finds the first occurrence of \a elem and moves it to the back of
   * the trove.  Return OB_OK if \a elem was found and moved, or
   * OB_NOTHING_TO_DO if \a elem was found to already be at the back,
   * both of which are success codes.  The error code OB_NOT_FOUND is
   * returned if \a elem is not found.
   */
  ObRetort MoveToBack (ARGType elem)
  {
    int64 pos, terminus;
    if ((pos = Find (elem)) < 0)
      return OB_NOT_FOUND;
    if (pos == num_p - 1)
      return OB_NOTHING_TO_DO;
    STOREDType templar = pees[pos];
    for (terminus = num_p - 1; pos < terminus; pos++)
      pees[pos] = pees[pos + 1];
    pees[pos] = templar;
    return OB_OK;
  }


  /**
   * Moves the \a ind th element to the back of the trove.  \a ind
   * may be either positive or negative, with the usual meaning.
   * OB_BAD_INDEX is returned if \a ind is out of range.  OB_OK is
   * returned if the element is moved, and OB_NOTHING_TO_DO is
   * returned if \a ind already names the last element.
   */
  ObRetort MoveNthToBack (int64 ind)
  {
    OB_CHECK_INDEX (ind, >=, OB_BAD_INDEX);
    if (ind == num_p - 1)
      return OB_NOTHING_TO_DO;
    STOREDType templar = pees[ind];
    for (int64 terminus = num_p - 1; ind < terminus; ind++)
      pees[ind] = pees[ind + 1];
    pees[ind] = templar;
    return OB_OK;
  }


  /**
   * Finds the first occurrence of \a elem and moves it to the front of
   * the trove.  Return OB_OK if \a elem was found and moved, or
   * OB_NOTHING_TO_DO if \a elem was found to already be at the front,
   * both of which are success codes.  The error code OB_NOT_FOUND is
   * returned if \a elem is not found.
   */
  ObRetort MoveToFront (ARGType elem)
  {
    int64 pos, q;
    if ((pos = Find (elem)) < 0)
      return OB_NOT_FOUND;
    if (pos == 0)
      return OB_NOTHING_TO_DO;
    STOREDType temper = pees[pos];
    for (q = pos; q > 0; q--)
      pees[q] = pees[q - 1];
    pees[0] = temper;
    return OB_OK;
  }


  /**
   * Moves the \a ind th element to the front of the trove.  \a ind
   * may be either positive or negative, with the usual meaning.
   * OB_BAD_INDEX is returned if \a ind is out of range.  OB_OK is
   * returned if the element is moved, and OB_NOTHING_TO_DO is
   * returned if \a ind already names the first element.
   */
  ObRetort MoveNthToFront (int64 ind)
  {
    OB_CHECK_INDEX (ind, >=, OB_BAD_INDEX);
    if (ind == 0)
      return OB_NOTHING_TO_DO;
    STOREDType temper = pees[ind];
    for (int64 q = ind; q > 0; q--)
      pees[q] = pees[q - 1];
    pees[0] = temper;
    return OB_OK;
  }


  /**
   * Moves the element at \a from_ind to be at \a to_ind instead.
   * Either index may be positive or negative, and OB_BAD_INDEX is
   * returned if either is out of range.  Returns OB_OK on success,
   * or OB_NOTHING_TO_DO if \a from_ind and \a to_ind name the same
   * element.  (Which does not necessarily imply they are numerically
   * equal, since one could name an element positively and the other
   * could name the same element negatively.)
   */
  ObRetort MoveNthTo (int64 from_ind, int64 to_ind)
  {
    OB_CHECK_INDEX (from_ind, >=, OB_BAD_INDEX);
    OB_CHECK_INDEX (to_ind, >=, OB_BAD_INDEX);
    if (from_ind == to_ind)
      return OB_NOTHING_TO_DO;
    STOREDType tempoid = pees[from_ind];
    for (; from_ind < to_ind; from_ind++)
      pees[from_ind] = pees[from_ind + 1];
    for (; from_ind > to_ind; from_ind--)
      pees[from_ind] = pees[from_ind - 1];
    pees[to_ind] = tempoid;
    return OB_OK;
  }


  /**
   * Returns true if Nullify() or NullifyNth() has been successfully
   * called, without CompactNulls() having been subsequently called.
   */
  bool IsHoldingNullifiedElements () const
  {
    return holding_nullified_elements;
  }

  /**
   * Synonym for IsHoldingNullifiedElements().
   */
  inline bool HoldingNullifiedElements () const
  {
    return IsHoldingNullifiedElements ();
  }


  /**
   * returns number of NULLs removed, or -1 if not pointy
   */
  int64 CompactNulls ()
  {
    if (!Helpy::is_pointy)
      return -1;
    holding_nullified_elements = false;
    return RemoveEvery (NULL);
  }


  /**
   * Removes all elements from the trove.
   */
  void Empty ()
  {
    for (int64 q = num_p - 1; q >= 0; q--)
      pees[q].~STOREDType ();  // that's the destructor, you sweet thing.
    num_p = 0;
  }


  /**
   * whitesort (n-squared swap-based selection sort)
   * note: this is a placeholder for a better search in the future
   * FYI: http://en.wikipedia.org/wiki/Selection_sort
   *   insertion sort or selection sort are both typically faster for small
   *   arrays (i.e. fewer than 10-20 elements). A useful optimization in
   *   practice for the recursive algorithms is to switch to insertion sort
   *   or selection sort for "small enough" sublists.
   * the "left" and "right" are used to sort subsections of a trove,
   * and to bottom out a faster recursive algorithm
   */
  template <typename CMPFUNQ>
  void Sort (CMPFUNQ cmp, int64 left = 0, int64 right = -1)
  {
    if (0 > right)
      right = Count ();
    for (int i = left; i < right; ++i)
      for (int j = i + 1; j < right; ++j)
        if (0 < cmp (Viz::access (pees[i]), Viz::access (pees[j])))
          SwapElemsAt (i, j);
  }


  /**
   * \cond INTERNAL
   * A small bit for use by Quicksort.
   */
  template <typename CMPFUNQ>
  int64 Partition (CMPFUNQ &cmp, int64 left, int64 right)
  {
    int64 pivot = left + (right - left) / 2;
    ELEMType pval = Viz::access (pees[pivot]);
    int64 stored = left;
    SwapElemsAt (pivot, right);
    for (int64 i = left; i < right; ++i)
      if (0 >= cmp (Viz::access (pees[i]), pval))
        SwapElemsAt (i, stored++);
    SwapElemsAt (stored, right);
    return stored;
  }
  /** \endcond */

  /**
   * quicksort (http://en.wikipedia.org/wiki/Quicksort)
   * Falls back on Sort for small-ish arrays.  Not a stable sort.
   */
  template <typename CMPFUNQ>
  void Quicksort (CMPFUNQ cmp, int64 left = 0, int64 right = -1)
  {
    static const int64 RECURSIVE_SORT_SMALL_ENOUGH = 7;
    if (-1 == right)
      right = Count ();
    if (RECURSIVE_SORT_SMALL_ENOUGH > (right - left))
      Sort (cmp, left, right);
    else
      {
        int64 pivot = Partition (cmp, left, right - 1);
        Quicksort (cmp, left, pivot);
        Quicksort (cmp, pivot, right);
      }
  }


  /**
   * Return the additive amount by which the trove would grow if it
   * were necessary (and if that amount exceeds the multiplicative
   * growth's).
   */
  unt32 ArithmeticGrowthFactor () const { return inc_size; }

  /**
   * Return the multiplicative factor by which the trove will grow when
   * necessary (and if that size exceeds the additive growth version).
   */
  float64 GeometricGrowthFactor () const { return growth_factor; }

  /**
   * Return the trove's current capacity.
   */
  int64 Capacity () const { return capac; }

  /**
   * Return the future capacity of the trove (upon next enlargement).
   */
  int64 NextLargerCapacity () const { return Bigger (); }

  /**
   * Configures how this trove will expand.  Every time the trove
   * must expand, the new capacity is set to either capacity times
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
    if (arith_incr < 1)
      return OB_INVALID_ARGUMENT;

    growth_factor = geom_mult;
    inc_size = arith_incr;
    return OB_OK;
  }

  /**
   * A synonym for SetGrowthFactors(); this is the historical form...
   */
  ObRetort SetIncSize (unt32 s, float64 multiplier = 1.0)
  {
    return SetGrowthFactors (s, multiplier);
  }


  /**
   * Makes sure the capacity of the trove is at least \a num.  This
   * can be used in conjunction with SetIncSize() to ensure good
   * performance, if you have some idea how many elements your trove
   * will eventually contain.  Throws std::bad_alloc if memory could
   * not be allocated.  Returns OB_NOTHING_TO_DO if \a num is
   * strictly less than the current number of elements; otherwise
   * returns OB_OK.  (So if OB_OK is returned, then allocation might
   * or might not have occurred; is this really what we want?)
   */
  ObRetort EnsureRoomFor (int64 num)
  {
    if (num < num_p)
      return OB_NOTHING_TO_DO;
    if (num > capac)
      {
        STOREDType *kyooz = Viz::expand (pees, num_p, num);
        if (!kyooz)
          {
            OB_LOG_ERROR_CODE (0x11050003, "true horror: malloc failed in "
                                           "ObTrove::EnsureRoomFor...\n");
            throw std::bad_alloc ();
          }
        pees = kyooz;
        capac = num;
      }
    return OB_OK;
  }

  /**
   * Increases the Count() to \a num, filling in with default
   * elements at the end, and returns OB_OK, if \a num is greater
   * than the existing Count().  Returns OB_NOTHING_TO_DO if \a num
   * is less than or equal to the existing Count().  Throws
   * std::bad_alloc if memory cannot be allocated.
   */
  virtual ObRetort ExpandUsingDefaultValTo (int64 num)
  {
    if (num <= num_p)
      return OB_NOTHING_TO_DO;
    EnsureRoomFor (num);
    for (int64 q = num_p; q < num; q++)
      Viz::assign (pees, q, Viz::nullret ());
    num_p = num;
    return OB_OK;
  }


  template <typename OtherELEMType,
            template <typename DUMMY> class OTHER_MEM_MGR_CLASS>
  ObTrove<ELEMType, MEM_MGR_CLASS> &
  operator= (const ObTrove<OtherELEMType, OTHER_MEM_MGR_CLASS> &other)
  {
    Empty ();
    int64 count = other.Count ();
    EnsureRoomFor (count);
    for (int64 q = 0; q < count; q++)
      Append (other.Nth (q));
    return *this;
  }

  ObTrove &operator= (const ObTrove &other)
  {
    if (this == &other)
      return *this;

    Empty ();
    int64 count = other.Count ();
    EnsureRoomFor (count);
    for (int64 q = 0; q < count; q++)
      Append (other.Nth (q));
    return *this;
  }

  ObTrove &operator= (ObTrove &&other)
  {
    if (this == &other)
      return *this;

    ObTrove tmp;
    SwapInternals (tmp);
    SwapInternals (other);

    return *this;
  }

  ObTrove<ELEMType, MEM_MGR_CLASS> &CopyFrom (const ObTrove<ELEMType> &other)
  {
    Empty ();
    int64 count = other.Count ();
    EnsureRoomFor (count);
    for (int64 q = 0; q < count; q++)
      Append (other.Nth (q));
    return *this;
  }


  ObTrove<ELEMType, MEM_MGR_CLASS>
  Concat (const ObTrove<ELEMType, MEM_MGR_CLASS> &other) const
  {
    ObTrove<ELEMType, MEM_MGR_CLASS> result;

    result.EnsureRoomFor (Count () + other.Count ());
    result.Append (*this);
    result.Append (other);

    return result;
  }


  ObTrove<ELEMType, MEM_MGR_CLASS> Dup () const
  {
    return ObTrove<ELEMType, MEM_MGR_CLASS> (*this);
  }

  ObTrove<ELEMType, MEM_MGR_CLASS> *HeapyDup () const
  {
    return new ObTrove<ELEMType, MEM_MGR_CLASS> (*this);
  }

  ObTrove<ELEMType, WeakRef> WeakDup () const
  {
    return ObTrove<ELEMType, WeakRef> (*this);
  }

  ObTrove<ELEMType, WeakRef> *HeapyWeakDup () const
  {
    return new ObTrove<ELEMType, WeakRef> (*this);
  }


  /**
   * \cond INTERNAL
   * this next makes the trove a participating memeber of the ObCrawl world.
   */

  class OC_TroveGuts : public OC_Guts
  {
    PATELLA_SUBCLASS (OC_TroveGuts, OC_Guts);

   OB_PRIVATE:
    STOREDType *f, *ff, *a, *aa;

   private:
    OC_TroveGuts () {}
   public:
    //typedef ObCrawl<ELEMType>::OC_Guts super;
    OC_TroveGuts (STOREDType *fr, STOREDType *af)
        : f (fr), ff (fr - 1), a (af - 1), aa (af)
    {
    }
    OC_TroveGuts (STOREDType *eff, STOREDType *effeff, STOREDType *eigh,
                  STOREDType *eigheigh)
        : f (eff), ff (effeff), a (eigh), aa (eigheigh)
    {
    }
    bool IsEmpty () const override { return (f >= aa) || (a <= ff) || (f > a); }
    CRAWLRetType PopFore () override
    {
      assert (!IsEmpty ());
      return Viz::access (*f++);
    }
    CRAWLRetType PopAft () override
    {
      assert (!IsEmpty ());
      return Viz::access (*a--);
    }
    CRAWLRetType Fore () const override
    {
      assert (!IsEmpty ());
      return Viz::access (*f);
    }
    CRAWLRetType Aft () const override
    {
      assert (!IsEmpty ());
      return Viz::access (*a);
    }
    void Reload () override
    {
      f = ff + 1;
      a = aa - 1;
    }
    typename ObCrawl<ELEMType>::OC_Guts *Dup () const override
    {
      return new OC_TroveGuts (f, ff, a, aa);
    }
  };

 OB_PRIVATE:
  OC_TroveGuts *CrawlGuts () const
  {
    return (!pees) ? new OC_TroveGuts (NULL, NULL)
                   : new OC_TroveGuts (pees, pees + num_p);
  }
  /** \endcond */
 public:
  /**
   * Returns by value a Crawl that can iterate over this trove.
   */
  ObCrawl<ELEMType> Crawl () const { return ObCrawl<ELEMType> (CrawlGuts ()); }

  /**
   * Returns by pointer a Crawl that can iterate over this trove.
   */
  ObCrawl<ELEMType> *HeapyCrawl () const
  {
    return new ObCrawl<ELEMType> (CrawlGuts ());
  }

  /**
   * For compatibility with C++11 range-based for loop.  Beware that
   * what is returned is not really a proper STL iterator; it is just
   * good enough to make the for loop work.
   */
  const_iterator begin () const { return const_iterator (CrawlGuts ()); }

  /**
   * For compatibility with C++11 range-based for loop.
   */
  const_iterator end () const { return const_iterator (); }

  /**
   * \cond INTERNAL
   * So the following one accessor is not meant for public consumption
   * (although it is "public" in the C++ sense), and is just here
   * for a couple of weird special cases.  It has an intentionally
   * unpleasant name to discourage use.  The other option would be
   * to use "friend", but I couldn't figure out the syntax for friending
   * a templatized class, plus I don't think we're fans of "friend" here
   * at Oblong anyway, given the disparaging comments about Stroustrup
   * that seem to accompany uses of the "friend" keyword.
   *
   * Note: bug 2743 proposes that we should just make this public.
   */
  const STOREDType *_NaughtyXMXPVMFLNF_Pees_ForSlawTraits () const
  {
    return pees;
  }
  /** \endcond */
};



// oh the horror...

template <typename ELEMType>
ObTrove<ELEMType> ObCrawl<ELEMType>::trove ()
{
  ObTrove<ELEMType> tro (2.0);
  while (!isempty ())
    tro.Append (popfore ());
  // to reload() or not to reload()?
  return tro;
}

template <typename ELEMType>
ObTrove<ELEMType> *ObCrawl<ELEMType>::heapytrove ()
{
  ObTrove<ELEMType> *tro = new ObTrove<ELEMType> (2.0);
  while (!isempty ())
    tro->Append (popfore ());
  // to reload() or not to reload()?
  return tro;
}

template <typename ELEMType>
ObTrove<ELEMType, WeakRef> ObCrawl<ELEMType>::weaktrove ()
{
  ObTrove<ELEMType, WeakRef> tro (2.0);
  while (!isempty ())
    tro.Append (popfore ());
  // to reload() or not to reload()?
  return tro;
}

template <typename ELEMType>
ObTrove<ELEMType, WeakRef> *ObCrawl<ELEMType>::heapyweaktrove ()
{
  ObTrove<ELEMType, WeakRef> *tro = new ObTrove<ELEMType, WeakRef> (2.0);
  while (!isempty ())
    tro->Append (popfore ());
  // to reload() or not to reload()?
  return tro;
}
}
}  // end namespace loam? yes, loam. and oblong? yes, end of namespace oblong.


#undef OB_CHECK_INDEX
#undef OB_TR_MAX


#endif
