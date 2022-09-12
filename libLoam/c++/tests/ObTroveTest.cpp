
/* (c)  oblong industries */

#include <gtest/gtest.h>
#include "libLoam/c/ob-rand.h"
#include "libLoam/c++/LoamStreams.h"
#include "libLoam/c++/ObTrove.h"
#include "libLoam/c++/ObUniqueTrove.h"
#include "libLoam/c++/Str.h"

using namespace oblong::loam;

#define LEATHER_SIZE 5

static const Str leather[LEATHER_SIZE] = {"red leather", "yellow leather",
                                          "red leather", "yellow leather",
                                          "red leather"};

typedef ObTrove<Str> StrTrove;
typedef ObUniqueTrove<Str> UniqueStrTrove;

TEST (ObTroveTest, Find)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (0, t.Find ("red leather"));
  EXPECT_EQ (1, t.Find ("yellow leather"));
  EXPECT_EQ (-1, t.Find ("green leather"));
}

TEST (ObTroveTest, Nth)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_STREQ ("yellow leather", t.Nth (3));
  EXPECT_STREQ ("", t.Nth (5));
  EXPECT_STREQ ("", t.Nth (-6));
  EXPECT_STREQ ("red leather", t.Nth (0));
  EXPECT_STREQ ("yellow leather", t.Nth (-2));
  EXPECT_STREQ ("red leather", t.Nth (-1));
}

TEST (ObTroveTest, SafeNth)
{
  StrTrove t (leather, LEATHER_SIZE);
  Str into;
  EXPECT_EQ (OB_OK, t.SafeNth (3, into));
  EXPECT_STREQ ("yellow leather", into);
  EXPECT_EQ (OB_BAD_INDEX, t.SafeNth (5, into));
  EXPECT_STREQ ("yellow leather", into);
  into = "green leather";
  EXPECT_EQ (OB_BAD_INDEX, t.SafeNth (-6, into));
  EXPECT_STREQ ("green leather", into);
  EXPECT_EQ (OB_OK, t.SafeNth (0, into));
  EXPECT_STREQ ("red leather", into);
  EXPECT_EQ (OB_OK, t.SafeNth (-2, into));
  EXPECT_STREQ ("yellow leather", into);
  EXPECT_EQ (OB_OK, t.SafeNth (-1, into));
  EXPECT_STREQ ("red leather", into);
}

TEST (ObTroveTest, Insert)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_BAD_INDEX, t.Insert ("invisible leather", 6));
  EXPECT_EQ (OB_OK, t.Insert ("green leather", 5));
  EXPECT_EQ (OB_OK, t.Insert ("blue leather", -1));
  EXPECT_EQ (OB_OK, t.Insert ("purple leather", 0));
  EXPECT_EQ (8, t.Count ());
  EXPECT_STREQ ("purple leather", t.Nth (0));
  EXPECT_STREQ ("red leather", t.Nth (1));
  EXPECT_STREQ ("yellow leather", t.Nth (2));
  EXPECT_STREQ ("red leather", t.Nth (3));
  EXPECT_STREQ ("yellow leather", t.Nth (4));
  EXPECT_STREQ ("red leather", t.Nth (5));
  EXPECT_STREQ ("blue leather", t.Nth (6));
  EXPECT_STREQ ("green leather", t.Nth (7));
}

TEST (ObTroveTest, Remove)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_NOT_FOUND, t.Remove ("green leather"));
  // Formerly, unlike Find(), which finds the first, Remove() removed the last.
  // Why, I don't know.
  // but now (2 jan 2012) Remove() has been revised to 'go forward', just
  // like Find() and kin.
  EXPECT_EQ (OB_OK, t.Remove ("yellow leather"));
  EXPECT_EQ (4, t.Count ());
  EXPECT_STREQ ("red leather", t.Nth (0));
  EXPECT_STREQ ("red leather", t.Nth (1));
  EXPECT_STREQ ("yellow leather", t.Nth (2));
  EXPECT_STREQ ("red leather", t.Nth (3));
}

TEST (ObTroveTest, RemovalOrdering)
{
  ObTrove<int64> tro;
  tro.Append (1);
  tro.Append (2);
  tro.Append (3);
  tro.Append (4);
  tro.Append (5);
  EXPECT_EQ (-1, tro.Find (88));

  tro.Insert (88, 3);
  EXPECT_EQ (3, tro.Find (88));

  tro.Insert (88, 2);
  EXPECT_EQ (2, tro.Find (88));
  EXPECT_EQ (88, tro.Nth (4));

  EXPECT_EQ (OB_NOT_FOUND, tro.Remove (99));
  EXPECT_EQ (OB_OK, tro.Remove (88));
  EXPECT_EQ (3, tro.Find (88));

  EXPECT_EQ (OB_OK, tro.Remove (88));
  EXPECT_EQ (-1, tro.Find (88));

  EXPECT_EQ (OB_NOT_FOUND, tro.Remove (88));
}

TEST (ObTroveTest, RemoveEvery)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (0, t.RemoveEvery ("green leather"));
  EXPECT_EQ (3, t.RemoveEvery ("red leather"));
  EXPECT_EQ (2, t.Count ());
  EXPECT_STREQ ("yellow leather", t.Nth (0));
  EXPECT_STREQ ("yellow leather", t.Nth (1));
}

TEST (ObTroveTest, ReplaceNth)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_OK, t.ReplaceNth (1, "blue leather"));
  EXPECT_EQ (OB_OK, t.ReplaceNth (-1, "pink leather"));
  EXPECT_EQ (OB_BAD_INDEX, t.ReplaceNth (8, "green leather"));
  EXPECT_EQ (5, t.Count ());
  EXPECT_STREQ ("red leather", t.Nth (0));
  EXPECT_STREQ ("blue leather", t.Nth (1));
  EXPECT_STREQ ("red leather", t.Nth (2));
  EXPECT_STREQ ("yellow leather", t.Nth (3));
  EXPECT_STREQ ("pink leather", t.Nth (4));
}

TEST (ObTroveTest, PopAft)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_OK, t.PopAft ());
  EXPECT_EQ (4, t.Count ());
  EXPECT_EQ (OB_OK, t.PopAft ());
  EXPECT_EQ (3, t.Count ());
  EXPECT_EQ (OB_OK, t.PopAft ());
  EXPECT_EQ (2, t.Count ());
  // after 3 removals we should have red,yellow
  // (a hypothetical 'popfore' would be yellow, red)
  EXPECT_STREQ ("red leather", t.Nth (0));
  EXPECT_STREQ ("yellow leather", t.Nth (1));
  EXPECT_EQ (OB_OK, t.PopAft ());
  EXPECT_EQ (1, t.Count ());
  EXPECT_EQ (OB_OK, t.PopAft ());
  EXPECT_EQ (0, t.Count ());
  EXPECT_EQ (OB_EMPTY, t.PopAft ());
  EXPECT_EQ (0, t.Count ());
  EXPECT_EQ (OB_EMPTY, t.PopAft ());
  EXPECT_EQ (0, t.Count ());
}

TEST (ObTroveTest, RemoveNth)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_OK, t.RemoveNth (0));
  EXPECT_EQ (OB_OK, t.RemoveNth (1));
  EXPECT_EQ (OB_OK, t.RemoveNth (-1));
  EXPECT_EQ (OB_BAD_INDEX, t.RemoveNth (2));
  EXPECT_EQ (2, t.Count ());
  EXPECT_STREQ ("yellow leather", t.Nth (0));
  EXPECT_STREQ ("yellow leather", t.Nth (1));
}

class StrWrapper : public AnkleObject
{
  PATELLA_SUBCLASS (StrWrapper, AnkleObject);

 public:
  StrWrapper (Str theString) { payload = theString; }
  Str payload;

  static ObTrove<StrWrapper *> FromStrTrove (StrTrove strove)
  {
    ObTrove<StrWrapper *> theWrap;
    int count = strove.Count ();
    for (int i = 0; i < count; i++)
      theWrap.Append (new StrWrapper (strove.Nth (i)));
    return theWrap;
  }
};

TEST (ObTroveTest, UnlikeMemoryManagementCopy)
{
  AnkleObject *a = new AnkleObject ();
  AnkleObject *b = new AnkleObject ();
  AnkleObject *c = new AnkleObject ();

  ObTrove<AnkleObject *> managed;
  managed.Append (a);
  managed.Append (b);
  managed.Append (c);

  ObTrove<AnkleObject *, NoMemMgmt> unmanaged (managed);
  ASSERT_EQ (managed.Count (), unmanaged.Count ());
  for (int64 i = 0; i < managed.Count (); ++i)
    EXPECT_EQ (managed.Nth (i), unmanaged.Nth (i));

  ObTrove<AnkleObject *, NoMemMgmt> unmanaged_assign = managed;
  ASSERT_EQ (managed.Count (), unmanaged_assign.Count ());
  for (int64 i = 0; i < managed.Count (); ++i)
    EXPECT_EQ (managed.Nth (i), unmanaged_assign.Nth (i));

  ObTrove<AnkleObject *> other_managed (unmanaged);
  ASSERT_EQ (other_managed.Count (), unmanaged.Count ());
  for (int64 i = 0; i < managed.Count (); ++i)
    EXPECT_EQ (other_managed.Nth (i), unmanaged.Nth (i));

  ObTrove<AnkleObject *> other_managed_assign = unmanaged;
  ASSERT_EQ (other_managed_assign.Count (), unmanaged.Count ());
  for (int64 i = 0; i < managed.Count (); ++i)
    EXPECT_EQ (other_managed_assign.Nth (i), unmanaged.Nth (i));
}

TEST (ObTroveTest, UnmanagedStrSplit)
{
  Str s ("hello world");
  ObTrove<Str, NoMemMgmt> split = s.Split (" ");
  ASSERT_EQ (2, split.Count ());
  EXPECT_EQ ("hello", split.Nth (0));
  EXPECT_EQ ("world", split.Nth (1));
}

TEST (ObTroveTest, NullifyNth)
{
  StrTrove t (leather, LEATHER_SIZE);
  ObTrove<StrWrapper *> w = StrWrapper::FromStrTrove (t);
  EXPECT_EQ (OB_OK, w.NullifyNth (0));
  EXPECT_EQ (OB_OK, w.NullifyNth (1));
  EXPECT_EQ (OB_OK, w.NullifyNth (-1));
  EXPECT_EQ (OB_BAD_INDEX, w.NullifyNth (5));
  EXPECT_EQ (5, w.Count ());
  // oh google, stop whining and support EXPECT_NE(NULL, ptr).
  EXPECT_TRUE (w.Nth (0) == NULL);
  EXPECT_TRUE (w.Nth (1) == NULL);
  EXPECT_TRUE (w.Nth (2) != NULL);
  EXPECT_TRUE (w.Nth (3) != NULL);
  EXPECT_TRUE (w.Nth (4) == NULL);
  EXPECT_STREQ ("red leather", w.Nth (2)->payload);
  EXPECT_STREQ ("yellow leather", w.Nth (3)->payload);
}

TEST (ObTroveTest, Nullify)
{
  StrTrove t (leather, LEATHER_SIZE);
  ObTrove<StrWrapper *> w = StrWrapper::FromStrTrove (t);
  // this ObRef will allow the targetPointer to be valid past the Nullify
  ObRef<StrWrapper *> targetPointer = w.Nth (0);
  // this object looks the same inside, but "pointy" ObTrove types
  // just check for pointer equality, so this will not match
  ObRef<StrWrapper *> doppelgangerPointer = new StrWrapper ("red leather");
  EXPECT_EQ (OB_NOT_FOUND, w.Nullify (~doppelgangerPointer));
  EXPECT_EQ (5, w.Count ());
  EXPECT_EQ (OB_OK, w.Nullify (~targetPointer));
  EXPECT_EQ (5, w.Count ());
  EXPECT_TRUE (w.Nth (0) == NULL);
  EXPECT_TRUE (w.Nth (1) != NULL);
  EXPECT_TRUE (w.Nth (2) != NULL);
  EXPECT_TRUE (w.Nth (3) != NULL);
  EXPECT_TRUE (w.Nth (4) != NULL);
  // this checks that the Trove does not have the pointer, but that
  // it can still happily exist elsewhere through Nullification
  EXPECT_STREQ ("red leather", (~targetPointer)->payload);
  EXPECT_STREQ ("yellow leather", w.Nth (1)->payload);
  EXPECT_STREQ ("red leather", w.Nth (2)->payload);
  EXPECT_STREQ ("yellow leather", w.Nth (3)->payload);
  EXPECT_STREQ ("red leather", w.Nth (4)->payload);
}

TEST (ObTroveTest, SwapElems)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_OK, t.SwapElems ("red leather", "yellow leather"));
  EXPECT_EQ (OB_NOT_FOUND, t.SwapElems ("blue leather", "yellow leather"));
  EXPECT_EQ (OB_NOT_FOUND, t.SwapElems ("red leather", "green leather"));
  EXPECT_EQ (OB_NOT_FOUND, t.SwapElems ("blue leather", "green leather"));
  EXPECT_EQ (5, t.Count ());
  EXPECT_STREQ ("yellow leather", t.Nth (0));
  EXPECT_STREQ ("red leather", t.Nth (1));
  EXPECT_STREQ ("red leather", t.Nth (2));
  EXPECT_STREQ ("yellow leather", t.Nth (3));
  EXPECT_STREQ ("red leather", t.Nth (4));
}

TEST (ObTroveTest, SwapElemsAt)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_OK, t.SwapElemsAt (0, 1));
  EXPECT_EQ (OB_BAD_INDEX, t.SwapElemsAt (5, -1));
  EXPECT_EQ (OB_BAD_INDEX, t.SwapElemsAt (-1, 5));
  EXPECT_EQ (OB_OK, t.SwapElemsAt (3, -2));
  EXPECT_EQ (5, t.Count ());
  EXPECT_STREQ ("yellow leather", t.Nth (0));
  EXPECT_STREQ ("red leather", t.Nth (1));
  EXPECT_STREQ ("red leather", t.Nth (2));
  EXPECT_STREQ ("yellow leather", t.Nth (3));
  EXPECT_STREQ ("red leather", t.Nth (4));
}

TEST (ObTroveTest, MoveToBack)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_OK, t.Remove ("yellow leather"));
  EXPECT_EQ (OB_OK, t.MoveToBack ("yellow leather"));
  EXPECT_EQ (OB_NOT_FOUND, t.MoveToBack ("green leather"));
  EXPECT_EQ (OB_NOTHING_TO_DO, t.MoveToBack ("yellow leather"));
  EXPECT_EQ (4, t.Count ());
  EXPECT_STREQ ("red leather", t.Nth (0));
  EXPECT_STREQ ("red leather", t.Nth (1));
  EXPECT_STREQ ("red leather", t.Nth (2));
  EXPECT_STREQ ("yellow leather", t.Nth (3));
}

TEST (ObTroveTest, MoveNthToBack)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_NOTHING_TO_DO, t.MoveNthToBack (-1));
  EXPECT_EQ (OB_OK, t.MoveNthToBack (-2));
  EXPECT_EQ (OB_OK, t.MoveNthToBack (1));
  EXPECT_EQ (OB_BAD_INDEX, t.MoveNthToBack (5));
  EXPECT_EQ (OB_BAD_INDEX, t.MoveNthToBack (-6));
  EXPECT_EQ (5, t.Count ());
  EXPECT_STREQ ("red leather", t.Nth (0));
  EXPECT_STREQ ("red leather", t.Nth (1));
  EXPECT_STREQ ("red leather", t.Nth (2));
  EXPECT_STREQ ("yellow leather", t.Nth (3));
  EXPECT_STREQ ("yellow leather", t.Nth (4));
}

TEST (ObTroveTest, MoveToFront)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_OK, t.Remove ("yellow leather"));
  EXPECT_EQ (OB_OK, t.MoveToFront ("yellow leather"));
  EXPECT_EQ (OB_NOT_FOUND, t.MoveToFront ("green leather"));
  EXPECT_EQ (OB_NOTHING_TO_DO, t.MoveToFront ("yellow leather"));
  EXPECT_EQ (4, t.Count ());
  EXPECT_STREQ ("yellow leather", t.Nth (0));
  EXPECT_STREQ ("red leather", t.Nth (1));
  EXPECT_STREQ ("red leather", t.Nth (2));
  EXPECT_STREQ ("red leather", t.Nth (3));
}

TEST (ObTroveTest, MoveNthToFront)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_OK, t.MoveNthToFront (-1));
  EXPECT_EQ (OB_OK, t.MoveNthToFront (-2));
  EXPECT_EQ (OB_OK, t.MoveNthToFront (1));
  EXPECT_EQ (OB_BAD_INDEX, t.MoveNthToFront (5));
  EXPECT_EQ (OB_BAD_INDEX, t.MoveNthToFront (-6));
  EXPECT_EQ (5, t.Count ());
  EXPECT_STREQ ("red leather", t.Nth (0));
  EXPECT_STREQ ("red leather", t.Nth (1));
  EXPECT_STREQ ("red leather", t.Nth (2));
  EXPECT_STREQ ("yellow leather", t.Nth (3));
  EXPECT_STREQ ("yellow leather", t.Nth (4));
}

TEST (ObTroveTest, MoveNthTo)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_NOTHING_TO_DO, t.MoveNthTo (-1, 4));
  EXPECT_EQ (OB_BAD_INDEX, t.MoveNthTo (-6, 0));
  EXPECT_EQ (OB_OK, t.MoveNthTo (-5, -4));
  EXPECT_EQ (5, t.Count ());
  EXPECT_STREQ ("yellow leather", t.Nth (0));
  EXPECT_STREQ ("red leather", t.Nth (1));
  EXPECT_STREQ ("red leather", t.Nth (2));
  EXPECT_STREQ ("yellow leather", t.Nth (3));
  EXPECT_STREQ ("red leather", t.Nth (4));
}

TEST (ObTroveTest, SetIncSize)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_INVALID_ARGUMENT, t.SetIncSize (0));
  EXPECT_EQ (OB_OK, t.SetIncSize (1));
}

TEST (ObTroveTest, EnsureRoomFor)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_NOTHING_TO_DO, t.EnsureRoomFor (4));
  // XXX: currently this returns OB_OK, but should it return OB_NOTHING_TO_DO,
  // since we did nothing?
  EXPECT_EQ (OB_OK, t.EnsureRoomFor (5));
  EXPECT_EQ (OB_OK, t.EnsureRoomFor (6));
  EXPECT_EQ (5, t.Count ());
}

TEST (ObTroveTest, ExpandUsingDefaultValTo)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_EQ (OB_NOTHING_TO_DO, t.ExpandUsingDefaultValTo (4));
  EXPECT_EQ (OB_NOTHING_TO_DO, t.ExpandUsingDefaultValTo (5));
  EXPECT_EQ (OB_OK, t.ExpandUsingDefaultValTo (6));
  EXPECT_EQ (6, t.Count ());
}

TEST (ObTroveTest, GrowthFactors)
{
  ObTrove<int32> trA;
  ObTrove<int32> trB;
  trB.SetGrowthFactors (15, 1.0);
  ObTrove<int32> trC (1.6);

  EXPECT_EQ (8u, trA.ArithmeticGrowthFactor ());
  EXPECT_EQ (1.0, trA.GeometricGrowthFactor ());
  EXPECT_EQ (0, trA.Capacity ());
  EXPECT_EQ (4, trA.NextLargerCapacity ());
  trA.Append (665);
  EXPECT_EQ (4, trA.Capacity ());
  EXPECT_EQ (12, trA.NextLargerCapacity ());

  EXPECT_EQ (15u, trB.ArithmeticGrowthFactor ());
  EXPECT_EQ (1.0, trB.GeometricGrowthFactor ());
  EXPECT_EQ (0, trB.Capacity ());
  EXPECT_EQ (4, trB.NextLargerCapacity ());
  trB.Append (667);
  EXPECT_EQ (4, trB.Capacity ());
  EXPECT_EQ (19, trB.NextLargerCapacity ());

  EXPECT_EQ (8u, trC.ArithmeticGrowthFactor ());
  EXPECT_EQ (1.6, trC.GeometricGrowthFactor ());
  EXPECT_EQ (0, trC.Capacity ());
  EXPECT_EQ (4, trC.NextLargerCapacity ());
  trC.Append (999);
  EXPECT_EQ (4, trC.Capacity ());
  EXPECT_EQ (12, trC.NextLargerCapacity ());  // hah!
  trC.Append (1661);
  trC.Append (6116);
  trC.Append (9339);
  trC.Append (6776);
  EXPECT_EQ (12, trC.Capacity ());
  EXPECT_EQ (20, trC.NextLargerCapacity ());  // again with the hah...
  for (int32 q = 0; q < 8; q++)
    trC.Append (q + 20202);
  EXPECT_EQ (20, trC.Capacity ());
  EXPECT_EQ (32, trC.NextLargerCapacity ());
}

TEST (ObTroveTest, AssignToNth)
{
  StrTrove t (leather, LEATHER_SIZE);
  t.Nth (1) = "brown leather";
  EXPECT_EQ (1, t.Find ("brown leather"));
}

// demonstrates that bug 1302 has been fixed
TEST (ObTroveTest, AssignToOutOfRange)
{
  StrTrove t (leather, LEATHER_SIZE);
  EXPECT_TRUE (t.Nth (111).IsEmpty ());
  t.Nth (111) = "brown leather";
  EXPECT_TRUE (t.Nth (112).IsEmpty ());
}

TEST (ObTroveTest, TroveOTroves)
{
  ObTrove<StrTrove> tot;
  StrTrove t (leather, LEATHER_SIZE);
  tot.Append (t);
  t.Append ("orange leather");
  tot.Append (t);
  t.Append ("blue leather");
  tot.Insert (t, 1);
  tot.ExpandUsingDefaultValTo (8);
  tot.Nth (3).Append ("purple leather");
  t.RemoveNth (0);
  tot.Append (t);
  tot.RemoveNth (4);
  EXPECT_EQ (5, tot.Nth (0).Count ());
  EXPECT_EQ (6, tot.Nth (1).Find ("blue leather"));
  EXPECT_EQ (5, tot.Nth (2).Find ("orange leather"));
  EXPECT_EQ (0, tot.Nth (3).Find ("purple leather"));
  EXPECT_EQ (0, tot.Nth (7).Find ("yellow leather"));
}

TEST (ObTroveTest, AppendingTroveAddsItems)
{
  ObTrove<int> t;
  t.Append (1);
  t.Append (2);

  ObTrove<int> u;
  u.Append (3);
  u.Append (4);

  t.Append (u);

  EXPECT_EQ (4, t.Count ());
  EXPECT_EQ (1, t.Nth (0));
  EXPECT_EQ (2, t.Nth (1));
  EXPECT_EQ (3, t.Nth (2));
  EXPECT_EQ (4, t.Nth (3));
}

TEST (ObTroveTest, AppendingTroveDoesntAlterArgument)
{
  ObTrove<int> t;
  t.Append (1);
  t.Append (2);

  ObTrove<int> u;
  u.Append (3);
  u.Append (4);

  t.Append (u);


  EXPECT_EQ (2, u.Count ());
  EXPECT_EQ (3, u.Nth (0));
  EXPECT_EQ (4, u.Nth (1));
}

TEST (ObTroveTest, ConcatAddsItems)
{
  ObTrove<int> t;
  t.Append (1);
  t.Append (2);

  ObTrove<int> u;
  u.Append (3);
  u.Append (4);

  const ObTrove<int> v = t.Concat (u);

  EXPECT_EQ (4, v.Count ());
  EXPECT_EQ (1, v.Nth (0));
  EXPECT_EQ (2, v.Nth (1));
  EXPECT_EQ (3, v.Nth (2));
  EXPECT_EQ (4, v.Nth (3));
}

TEST (ObTroveTest, ConcatDoesntChangeReceiver)
{
  ObTrove<int> t;
  t.Append (1);
  t.Append (2);

  ObTrove<int> u;
  u.Append (3);
  u.Append (4);

  const ObTrove<int> v = t.Concat (u);

  EXPECT_EQ (2, t.Count ());
  EXPECT_EQ (1, t.Nth (0));
  EXPECT_EQ (2, t.Nth (1));
}

TEST (ObTroveTest, ConcatDoesntChangeArgument)
{
  ObTrove<int> t;
  t.Append (1);
  t.Append (2);

  ObTrove<int> u;
  u.Append (3);
  u.Append (4);

  const ObTrove<int> v = t.Concat (u);

  EXPECT_EQ (2, u.Count ());
  EXPECT_EQ (3, u.Nth (0));
  EXPECT_EQ (4, u.Nth (1));
}


TEST (ObTroveTest, Iterator)
{
  StrTrove t (leather, LEATHER_SIZE);
  size_t i = 0;
  for (StrTrove::const_iterator it = t.begin (); it != t.end (); ++it)
    EXPECT_STREQ (leather[i++], *it);

  EXPECT_EQ (size_t (LEATHER_SIZE), i);
}


struct Glorp : public AnkleObject
{
  int nub;
  Glorp (int nb) : AnkleObject (), nub (nb) {}
};

TEST (ObTroveTest, SafeDup)
{
  Glorp *g1 = new Glorp (1);
  Glorp *g2 = new Glorp (2);
  Glorp *g3 = new Glorp (3);

  ObTrove<Glorp *> gtro;
  gtro.Append (g1);
  gtro.Append (g2);
  gtro.Append (g3);

  ObTrove<Glorp *> gdup = gtro.Dup ();
  gtro.Remove (g2);
  EXPECT_EQ (3, gdup.Count ());
  EXPECT_EQ (g1, gdup.Nth (0));
  EXPECT_EQ (g2, gdup.Nth (1));
  EXPECT_EQ (g3, gdup.Nth (2));

  gtro.Empty ();

  ObTrove<Glorp *, WeakRef> gwea = gdup.WeakDup ();
  gdup.Remove (g2);
  EXPECT_EQ (2, gdup.Count ());
  EXPECT_EQ (3, gwea.Count ());
  EXPECT_EQ (g1, gwea.Nth (0));
  EXPECT_EQ (NULL, gwea.Nth (1));
  EXPECT_EQ (g3, gwea.Nth (2));
}

struct AOCounter : public AnkleObject
{
  static int64 counter;

  AOCounter () { ++counter; }

  ~AOCounter () override { --counter; }
};

int64 AOCounter::counter = 0;

TEST (ObTroveTest, RetainMemMgr)
{
  EXPECT_EQ (AOCounter::counter, 0);

  AOCounter *aoc = new AOCounter ();

  EXPECT_EQ (AOCounter::counter, 1);

  ObTrove<AnkleObject *, NoMemMgmt> tro;
  tro.Append (aoc);

  EXPECT_EQ (AOCounter::counter, 1);

  tro.Dup ();

  EXPECT_EQ (AOCounter::counter, 1);

  if (AOCounter::counter > 0)
    aoc->Delete ();
}

TEST (ObTroveTest, DupFromCrawl)
{
  ObTrove<Glorp *> ga;
  Glorp *ded = NULL;
  ga.Append (new Glorp (1));
  ga.Append (ded = new Glorp (2));
  ga.Append (new Glorp (3));

  ObTrove<Glorp *> gb;
  gb.Append (new Glorp (4));
  gb.Append (new Glorp (5));
  gb.Append (new Glorp (6));

  ObTrove<Glorp *> gc;
  gc.Append (new Glorp (7));
  gc.Append (new Glorp (8));

  ObCrawl<Glorp *> cr =
    (((gb.Crawl ()).chain (ga.Crawl ())).retro ()).chain (gc.Crawl ()).retro ();

  ObTrove<Glorp *> strong = cr.trove ();
  cr.reload ();
  ObTrove<Glorp *, WeakRef> weak = cr.weaktrove ();

  EXPECT_EQ (8, strong.Count ());
  EXPECT_EQ (8, weak.Count ());

  EXPECT_EQ (8, strong.Nth (0)->nub);
  EXPECT_EQ (7, strong.Nth (1)->nub);
  EXPECT_EQ (4, strong.Nth (2)->nub);
  EXPECT_EQ (5, strong.Nth (3)->nub);
  EXPECT_EQ (6, strong.Nth (4)->nub);
  EXPECT_EQ (1, strong.Nth (5)->nub);
  EXPECT_EQ (2, strong.Nth (6)->nub);
  EXPECT_EQ (3, strong.Nth (7)->nub);

  strong.Empty ();
  ga.Remove (ded);

  EXPECT_EQ (0, strong.Count ());
  EXPECT_EQ (8, weak.Count ());

  EXPECT_EQ (8, weak.Nth (0)->nub);
  EXPECT_EQ (7, weak.Nth (1)->nub);
  EXPECT_EQ (4, weak.Nth (2)->nub);
  EXPECT_EQ (5, weak.Nth (3)->nub);
  EXPECT_EQ (6, weak.Nth (4)->nub);
  EXPECT_EQ (1, weak.Nth (5)->nub);
  EXPECT_EQ (NULL, weak.Nth (6));
  EXPECT_EQ (3, weak.Nth (7)->nub);
}

TEST (ObTroveTest, SelfCopy)
{
  ObTrove<AOCounter *> counter_trove;
  counter_trove.Append (new AOCounter ());
  counter_trove.Append (new AOCounter ());
  counter_trove.Append (new AOCounter ());
  counter_trove.Append (new AOCounter ());
  counter_trove.Append (new AOCounter ());

  ASSERT_EQ (5, counter_trove.Count ());
  ASSERT_EQ (counter_trove.Count (), AOCounter::counter);

  counter_trove =
    *&counter_trove;  // annotate with *& to tell clang we intend self-assignment
  ASSERT_EQ (5, counter_trove.Count ());
  ASSERT_EQ (counter_trove.Count (), AOCounter::counter);
}

TEST (ObTroveTest, CopyCopy)
{
  ObTrove<AOCounter *> counter_trove;
  counter_trove.Append (new AOCounter ());
  counter_trove.Append (new AOCounter ());
  counter_trove.Append (new AOCounter ());
  counter_trove.Append (new AOCounter ());
  counter_trove.Append (new AOCounter ());

  ASSERT_EQ (5, counter_trove.Count ());
  ASSERT_EQ (counter_trove.Count (), AOCounter::counter);

  ObTrove<AOCounter *> counter_copy;
  counter_copy = counter_trove;
  ASSERT_EQ (5, counter_copy.Count ());
  ASSERT_EQ (counter_copy.Count (), AOCounter::counter);
}

int int_cmp_count = 0;
static int IntCmp (const int64 &a, const int64 &b)
{
  ++int_cmp_count;
  return a - b;
}

template <typename T>
static bool ordered_test (ObTrove<T> &a)
{
  if (0 == a.Count ())
    return true;
  T last = a.Nth (0);
  for (int64 i = 1; i < a.Count (); ++i)
    if (a.Nth (i) < last)
      return false;
    else
      last = a.Nth (i);
  return true;
}

TEST (ObTroveSortingTest, QuicksortZeroSize)
{
  ObTrove<int64> a;
  a.Quicksort (IntCmp);
  EXPECT_EQ (0, a.Count ());
  EXPECT_TRUE (ordered_test (a));
}

TEST (ObTroveSortingTest, QuicksortJustOne)
{
  ObTrove<int64> a;
  a.Append (3721);
  a.Quicksort (IntCmp);
  EXPECT_EQ (1, a.Count ());
  EXPECT_EQ (3721, a.Nth (0));
  EXPECT_TRUE (ordered_test (a));
}

TEST (ObTroveSortingTest, QuicksortTwoElts)
{
  ObTrove<int64> a;
  a.Append (400000);
  a.Append (3721);
  EXPECT_EQ (2, a.Count ());
  EXPECT_EQ (400000, a.Nth (0));
  EXPECT_EQ (3721, a.Nth (1));
  a.Quicksort (IntCmp);
  EXPECT_EQ (2, a.Count ());
  EXPECT_EQ (3721, a.Nth (0));
  EXPECT_EQ (400000, a.Nth (1));
}


TEST (ObTroveSortingTest, SortTwoElts)
{
  ObTrove<int64> a;
  a.Append (400000);
  a.Append (3721);
  EXPECT_EQ (2, a.Count ());
  EXPECT_EQ (400000, a.Nth (0));
  EXPECT_EQ (3721, a.Nth (1));
  a.Sort (IntCmp);
  EXPECT_EQ (2, a.Count ());
  EXPECT_EQ (3721, a.Nth (0));
  EXPECT_EQ (400000, a.Nth (1));
}

TEST (ObTroveSortingTest, QuicksortSortFewElements)
{
  int_cmp_count = 0;
  ObTrove<int64> a;
  for (int64 x = 0; x < 10; ++x)
    a.Append (10 - x);
  EXPECT_FALSE (ordered_test (a));
  a.Quicksort (IntCmp);
  EXPECT_TRUE (ordered_test (a));
}

TEST (ObTroveSortingTest, Quicksort1000ElementsReversed)
{
  int_cmp_count = 0;
  ObTrove<int64> a (2.0);
  for (int64 x = 0; x < 1000; ++x)
    a.Append (1000 - x);
  EXPECT_FALSE (ordered_test (a));
  a.Quicksort (IntCmp);
  EXPECT_TRUE (ordered_test (a));
  ASSERT_LT (int_cmp_count, 20000);
}

TEST (ObTroveSortingTest, Quicksort1000ElementsOrdered)
{
  int_cmp_count = 0;
  ObTrove<int64> a (1.9);
  for (int64 x = 0; x < 1000; ++x)
    a.Append (x);
  EXPECT_TRUE (ordered_test (a));
  a.Quicksort (IntCmp);
  EXPECT_TRUE (ordered_test (a));
  ASSERT_LT (int_cmp_count, 20000);
}

TEST (ObTroveSortingTest, Sort1000ElementsReversed)
{
  int_cmp_count = 0;
  ObTrove<int64> a (1.8);
  for (int64 x = 0; x < 1000; ++x)
    a.Append (1000 - x);
  EXPECT_FALSE (ordered_test (a));
  a.Sort (IntCmp);
  EXPECT_TRUE (ordered_test (a));
  ASSERT_GT (int_cmp_count, 20000);
  ASSERT_LT (int_cmp_count, 1000000);
}


TEST (ObTroveSortingTest, Sort1000ElementsOrdered)
{
  int_cmp_count = 0;
  ObTrove<int64> a (1.7);
  for (int64 x = 0; x < 1000; ++x)
    a.Append (x);
  EXPECT_TRUE (ordered_test (a));
  a.Sort (IntCmp);
  EXPECT_TRUE (ordered_test (a));
  ASSERT_GT (int_cmp_count, 20000);
  ASSERT_LT (int_cmp_count, 1000000);
}


TEST (ObTroveSortingTest, Quicksort1000ElementsRandom)
{
  int_cmp_count = 0;
  ObTrove<int64> a (1.6);
  // Note: Just to make sure that we're actually not ordered,
  //       put a couple of starter elements in at the beginning
  a.Append (1);
  a.Append (0);
  for (int64 x = 0; x < 998; ++x)
    a.Append (ob_rand_int32 (0, OB_INT32_MAX));
  ObTrove<int64> b (a);

  EXPECT_EQ (a.Count (), b.Count ());
  for (int64 i = 0; i < a.Count (); ++i)
    EXPECT_EQ (a.Nth (i), b.Nth (i));

  EXPECT_FALSE (ordered_test (a));
  a.Quicksort (IntCmp);
  EXPECT_TRUE (ordered_test (a));
  ASSERT_LT (int_cmp_count, 20000);
  ASSERT_GT (int_cmp_count, 1000);

  // Now make sure that normal sort is N^2
  EXPECT_EQ (1000, b.Count ());
  EXPECT_FALSE (ordered_test (b));
  int_cmp_count = 0;
  EXPECT_EQ (0, int_cmp_count);
  b.Sort (IntCmp);
  EXPECT_TRUE (ordered_test (b));
  EXPECT_GT (int_cmp_count, 400000);
  EXPECT_LT (int_cmp_count, 1000000);
}

TEST (ObTroveSortingTest, PartialQuicksortTest)
{
  ObTrove<int64> a (1.5);
  for (int64 i = 0; i < 100; ++i)
    a.Append (100 - i);
  ObTrove<int64> b (a);

  EXPECT_EQ (a.Count (), b.Count ());
  for (int64 i = 0; i < a.Count (); ++i)
    EXPECT_EQ (a.Nth (i), b.Nth (i));

  EXPECT_FALSE (ordered_test (a));
  a.Quicksort (IntCmp, 0, 5);
  EXPECT_FALSE (ordered_test (a));

  for (int64 i = 0; i < 5; ++i)
    EXPECT_EQ (96 + i, a.Nth (i));
  // Now make sure that the rest of the trove isn't touched
  for (int64 i = 5; i < 100; ++i)
    EXPECT_EQ (a.Nth (i), b.Nth (i));
}

static int StrCmp (const Str &a, const Str &b)
{
  return a.Compare (b);
}

TEST (ObTroveSortingTest, QuicksortSomeStrs)
{
  Str text ("I am the very model of a modern Major-General, "
            "I've information vegetable, animal, and mineral, "
            "I know the kings of England, and I quote the fights historical "
            "From Marathon to Waterloo, in order categorical; "
            "I'm very well acquainted, too, with matters mathematical, "
            "I understand equations, both the simple and quadratical, "
            "About binomial theorem I'm teeming with a lot o' news, "
            "With many cheerful facts about the square of the hypotenuse.");
  Str categorical ("categorical;");
  ObTrove<Str> ss = text.Split (" ");
  EXPECT_EQ (70, ss.Count ());
  EXPECT_FALSE (ordered_test (ss));
  EXPECT_EQ (categorical, ss.Nth (33));
  ss.Quicksort (StrCmp);
  EXPECT_EQ (70, ss.Count ());
  EXPECT_TRUE (ordered_test (ss));
  EXPECT_EQ (categorical, ss.Nth (25));
}

struct holder
{
  int val;
  holder (const holder &other) : val (other.val) {}
  holder (int64 v) : val (v) {}
};

static int HolderPointerCmp (holder *a, holder *b)
{
  return a->val - b->val;
}

TEST (ObTroveSortingTest, SortPointersWithValsInside)
{
  ObTrove<holder *> a;
  a.Append (new holder (1));
  a.Append (new holder (0));
  for (int i = 2; i < 50; ++i)
    a.Append (new holder (ob_rand_int32 (0, OB_INT32_MAX)));
  a.Quicksort (HolderPointerCmp);
  EXPECT_EQ (50, a.Count ());
  int last = a.Nth (0)->val;
  for (int i = 1; i < 50; ++i)
    {
      const int next = a.Nth (i)->val;
      EXPECT_LE (last, next);
      last = next;
    }
}

TEST (ObTroveSortingTest, DISABLED_CopyAPointyButNotAnklyTrove)
{
  ObTrove<holder *> *a = new ObTrove<holder *>;
  a->Append (new holder (1));
  a->Append (new holder (0));
  for (int i = 2; i < 50; ++i)
    a->Append (new holder (ob_rand_int32 (0, OB_INT32_MAX)));
  ObTrove<holder *> b (*a);
  Del_Ptr (a);
  b.Quicksort (HolderPointerCmp);
  EXPECT_EQ (50, b.Count ());
  int last = b.Nth (0)->val;
  for (int i = 1; i < 50; ++i)
    {
      const int next = b.Nth (i)->val;
      EXPECT_LE (last, next);
      last = next;
    }
}

TEST (ObUniqueTroveTest, Append)
{
  UniqueStrTrove t;
  EXPECT_EQ (OB_OK, t.Append ("red leather"));
  EXPECT_EQ (OB_OK, t.Append ("yellow leather"));
  EXPECT_EQ (OB_ALREADY_PRESENT, t.Append ("red leather"));
  EXPECT_EQ (2, t.Count ());
  EXPECT_STREQ ("red leather", t.Nth (0));
  EXPECT_STREQ ("yellow leather", t.Nth (1));
}

TEST (ObUniqueTroveTest, Insert)
{
  UniqueStrTrove t;
  EXPECT_EQ (OB_OK, t.Append ("red leather"));
  EXPECT_EQ (OB_OK, t.Insert ("yellow leather", 0));
  EXPECT_EQ (OB_ALREADY_PRESENT, t.Insert ("red leather", 0));
  EXPECT_EQ (2, t.Count ());
  EXPECT_STREQ ("yellow leather", t.Nth (0));
  EXPECT_STREQ ("red leather", t.Nth (1));
}

TEST (ObUniqueTroveTest, ReplaceNth)
{
  UniqueStrTrove t;
  EXPECT_EQ (OB_OK, t.Append ("red leather"));
  EXPECT_EQ (OB_OK, t.Append ("yellow leather"));
  EXPECT_EQ (OB_ALREADY_PRESENT, t.ReplaceNth (0, "yellow leather"));
  EXPECT_EQ (OB_BAD_INDEX, t.ReplaceNth (77, "plaid leather"));
  EXPECT_EQ (2, t.Count ());
  EXPECT_STREQ ("red leather", t.Nth (0));
  EXPECT_STREQ ("yellow leather", t.Nth (1));
}

TEST (ObUniqueTroveTest, AssignToNth)
{
  UniqueStrTrove t;
  EXPECT_EQ (OB_OK, t.Append ("red leather"));
  EXPECT_EQ (OB_OK, t.Append ("yellow leather"));
  t.Nth (1) = "brown leather";
  EXPECT_STREQ ("red leather", t.Nth (0));
  EXPECT_STREQ ("brown leather", t.Nth (1));
}

// demonstrates bug 1348
TEST (ObUniqueTroveTest, AssignToNthNonUniquely)
{
  UniqueStrTrove t;
  EXPECT_EQ (OB_OK, t.Append ("red leather"));
  EXPECT_EQ (OB_OK, t.Append ("yellow leather"));
  t.Nth (1) = "red leather";
  EXPECT_STREQ ("red leather", t.Nth (0));
  EXPECT_STREQ ("red leather", t.Nth (1));
}

class ObUniqueTroveCastTest : public ::testing::Test
{
 public:
  StrTrove *t;
  void SetUp () override { t = new UniqueStrTrove; }
  void TearDown () override { delete t; }
};

TEST_F (ObUniqueTroveCastTest, Append)
{
  EXPECT_EQ (OB_OK, t->Append ("red leather"));
  EXPECT_EQ (OB_OK, t->Append ("yellow leather"));
  EXPECT_EQ (OB_ALREADY_PRESENT, t->Append ("red leather"));
  EXPECT_EQ (2, t->Count ());
  EXPECT_STREQ ("red leather", t->Nth (0));
  EXPECT_STREQ ("yellow leather", t->Nth (1));
}

TEST_F (ObUniqueTroveCastTest, Insert)
{
  EXPECT_EQ (OB_OK, t->Append ("red leather"));
  EXPECT_EQ (OB_OK, t->Insert ("yellow leather", 0));
  EXPECT_EQ (OB_ALREADY_PRESENT, t->Insert ("red leather", 0));
  EXPECT_EQ (2, t->Count ());
  EXPECT_STREQ ("yellow leather", t->Nth (0));
  EXPECT_STREQ ("red leather", t->Nth (1));
}

TEST_F (ObUniqueTroveCastTest, ReplaceNth)
{
  EXPECT_EQ (OB_OK, t->Append ("red leather"));
  EXPECT_EQ (OB_OK, t->Append ("yellow leather"));
  EXPECT_EQ (OB_ALREADY_PRESENT, t->ReplaceNth (0, "yellow leather"));
  EXPECT_EQ (OB_BAD_INDEX, t->ReplaceNth (77, "plaid leather"));
  EXPECT_EQ (2, t->Count ());
  EXPECT_STREQ ("red leather", t->Nth (0));
  EXPECT_STREQ ("yellow leather", t->Nth (1));
}

class StrMatchingInATrove : public ::testing::Test
{
 public:
  StrTrove t;

  void SetUp () override;
  void Dog (int64 n);
  void Check ();
};

void StrMatchingInATrove::SetUp ()
{
  t.Append ("time flies like an arrow ; fruit flies like a banana");
  EXPECT_TRUE (t.Nth (0).Match (" ; "));
}

void StrMatchingInATrove::Dog (int64 n)
{
  for (int64 i = 0; i < n; i++)
    t.Append ("outside of a dog, man's best friend is a book ; "
              "inside of a dog it is too dark to read");
}

void StrMatchingInATrove::Check ()
{
  EXPECT_STREQ (" ; ", t.Nth (0).MatchSlice ());
  EXPECT_STREQ ("time flies like an arrow", t.Nth (0).MatchPreSlice ());
  EXPECT_STREQ ("fruit flies like a banana", t.Nth (0).MatchPostSlice ());
}

TEST_F (StrMatchingInATrove, Simple)
{
  Check ();
}

TEST_F (StrMatchingInATrove, Small)
{
  Dog (3);
  Check ();
}

TEST_F (StrMatchingInATrove, Large)
{
  Dog (50);
  Check ();
}

TEST_F (StrMatchingInATrove, Sort)
{
  Dog (3);
  t.Quicksort (StrCmp);
  EXPECT_EQ (3, t.RemoveEvery (t.Nth (0)));
  Check ();
}


static int livecount = 0;

class Dorf : public AnkleObject
{
  PATELLA_SUBCLASS (Dorf, AnkleObject);

 public:
  Str s;
  Dorf (Str s_) : s (s_) { livecount++; }
  ~Dorf () override { livecount--; }
};


TEST (ObCrawl, CrawlFromTrove)
{
  ObTrove<Dorf *> dorves;

  dorves.Append (new Dorf ("avert"));
  dorves.Append (new Dorf ("bituminous"));
  dorves.Append (new Dorf ("catastrophe"));

  ObCrawl<Dorf *> cr = dorves.Crawl ();

  EXPECT_EQ (3, livecount);

  EXPECT_STREQ ("avert", cr.fore ()->s);
  EXPECT_STREQ ("catastrophe", cr.aft ()->s);

  EXPECT_EQ (3, livecount);

  EXPECT_FALSE (cr.isempty ());
  EXPECT_STREQ ("avert", cr.popfore ()->s);
  EXPECT_FALSE (cr.isempty ());
  EXPECT_STREQ ("catastrophe", cr.popaft ()->s);
  EXPECT_FALSE (cr.isempty ());
  EXPECT_STREQ ("bituminous", cr.popfore ()->s);
  EXPECT_TRUE (cr.isempty ());
}

TEST (ObTroveTest, RangedForLoopInts)
{
  ObTrove<int64> inttrove;
  inttrove.Append (1);
  inttrove.Append (2);
  inttrove.Append (3);
  inttrove.Append (4);
  inttrove.Append (5);
  int sum = 0;
  for (auto i : inttrove)
    {
      static_assert (std::is_same<decltype (i), int64>::value,
                     "std::begin(ObTrove<int64>) should return an iterator "
                     "with elements of type int64");
      sum += i;
    }
  EXPECT_EQ (5, inttrove.Count ());
  EXPECT_EQ (15, sum);
}

TEST (ObTroveTest, RangedForLoopObRefs)
{
  ObTrove<Dorf *> dorves;
  ObTrove<Str> strings;
  dorves.Append (new Dorf ("avert"));
  dorves.Append (new Dorf ("bituminous"));
  dorves.Append (new Dorf ("catastrophe"));

  for (auto dorf : dorves)
    {
      static_assert (std::is_same<decltype (dorf), Dorf *>::value,
                     "std::begin(ObTrove<AnkleObject *>) should return an "
                     "iterator with elements of type AnkleObject*");
      strings.Append (dorf->s);
    }
  EXPECT_EQ (strings.Nth (0), "avert");
  EXPECT_EQ (strings.Nth (1), "bituminous");
  EXPECT_EQ (strings.Nth (2), "catastrophe");
}
