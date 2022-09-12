
/* (c)  oblong industries */

#include <gtest/gtest.h>

#include <libLoam/c++/AnkleObject.h>
#include <libLoam/c++/ObAcacia.h>
#include <libLoam/c++/ObCons.h>
#include <libLoam/c++/ObHormel.h>
#include <libLoam/c++/ObMap.h>
#include <libLoam/c++/ObTrove.h>
#include <libLoam/c++/ObUniqueTrove.h>
#include <libLoam/c++/Str.h>

#include <utility>

using namespace oblong::loam;

TEST (MoveTest, StrMoveAssign)
{
  const char *data = "this is the base case.";
  Str target;
  ASSERT_TRUE (target.IsEmpty ());

  {
    Str st (data);
    target = std::move (st);
  }

  ASSERT_EQ (data, target);
}

TEST (MoveTest, StrMoveCtor)
{
  const char *data = "this is the base case.";

  Str st (data);
  Str target (std::move (st));

  ASSERT_EQ (data, target);
}

TEST (MoveTest, RegexStrMoveAssign)
{
  Str target ("blah blah");

  {
    Str str = "foo bar bash";
    EXPECT_TRUE (str.Match ("bar"));

    target = std::move (str);
  }


  Str substr = target.MatchSlice ();
  EXPECT_STREQ ("bar", substr);

  EXPECT_STREQ ("foo ", target.MatchPreSlice ());
  EXPECT_STREQ (" bash", target.MatchPostSlice ());
}

TEST (MoveTest, RegexStrMoveCtor)
{
  Str str = "foo bar bash";
  EXPECT_TRUE (str.Match ("bar"));

  Str target (std::move (str));

  Str substr = target.MatchSlice ();
  EXPECT_STREQ ("bar", substr);

  EXPECT_STREQ ("foo ", target.MatchPreSlice ());
  EXPECT_STREQ (" bash", target.MatchPostSlice ());
}

TEST (MoveTest, ObTroveMoveCtorIsPointyToBeWrapped)
{
  ObTrove<AnkleObject *> ao_trove;

  AnkleObject *ao1 = new AnkleObject;
  AnkleObject *ao2 = new AnkleObject;
  AnkleObject *ao3 = new AnkleObject;

  ao_trove.Append (ao1);
  ao_trove.Append (ao2);
  ao_trove.Append (ao3);


  ObTrove<AnkleObject *> ao_moved (std::move (ao_trove));

  EXPECT_EQ (3, ao_moved.Count ());
  EXPECT_EQ (0, ao_moved.Find (ao1));
  EXPECT_EQ (1, ao_moved.Find (ao2));
  EXPECT_EQ (2, ao_moved.Find (ao3));
}

TEST (MoveTest, ObTroveMoveAssignIsPointyToBeWrapped)
{
  ObTrove<AnkleObject *> ao_moved;
  AnkleObject *ao1 = new AnkleObject;
  AnkleObject *ao2 = new AnkleObject;
  AnkleObject *ao3 = new AnkleObject;

  {
    ObTrove<AnkleObject *> ao_trove;
    ao_trove.Append (ao1);
    ao_trove.Append (ao2);
    ao_trove.Append (ao3);

    ao_moved = std::move (ao_trove);
  }

  EXPECT_EQ (3, ao_moved.Count ());
  EXPECT_EQ (0, ao_moved.Find (ao1));
  EXPECT_EQ (1, ao_moved.Find (ao2));
  EXPECT_EQ (2, ao_moved.Find (ao3));
}

TEST (MoveTest, ObTroveMoveCtorIsPointyNotToBeWrapped)
{
  ObTrove<AnkleObject *> ao_trove;

  AnkleObject *ao1 = new AnkleObject;
  AnkleObject *ao2 = new AnkleObject;
  AnkleObject *ao3 = new AnkleObject;

  ao_trove.Append (ao1);
  ao_trove.Append (ao2);
  ao_trove.Append (ao3);

  ObTrove<AnkleObject *> ao_moved (std::move (ao_trove));

  for (AnkleObject *ao : ao_trove)
    ao->Delete ();

  ao_trove.Empty ();


  EXPECT_EQ (3, ao_moved.Count ());
  EXPECT_EQ (0, ao_moved.Find (ao1));
  EXPECT_EQ (1, ao_moved.Find (ao2));
  EXPECT_EQ (2, ao_moved.Find (ao3));

  for (AnkleObject *ao : ao_moved)
    ao->Delete ();

  ao_moved.Empty ();
}

TEST (MoveTest, ObTroveMoveAssignIsPointyNotToBeWrapped)
{
  ObTrove<AnkleObject *> ao_moved;
  AnkleObject *ao1 = new AnkleObject;
  AnkleObject *ao2 = new AnkleObject;
  AnkleObject *ao3 = new AnkleObject;

  {
    ObTrove<AnkleObject *> ao_trove;
    ao_trove.Append (ao1);
    ao_trove.Append (ao2);
    ao_trove.Append (ao3);

    ao_moved = std::move (ao_trove);
    for (AnkleObject *ao : ao_trove)
      ao->Delete ();

    ao_trove.Empty ();
  }

  EXPECT_EQ (3, ao_moved.Count ());
  EXPECT_EQ (0, ao_moved.Find (ao1));
  EXPECT_EQ (1, ao_moved.Find (ao2));
  EXPECT_EQ (2, ao_moved.Find (ao3));

  for (AnkleObject *ao : ao_moved)
    ao->Delete ();

  ao_moved.Empty ();
}

TEST (MoveTest, ObTroveMoveCtorNotPointy)
{
  static_assert (noexcept (Str ()), "Str's ctor should be noexcept");

  ObTrove<Str> source;
  source.Append ("one string");
  source.Append ("two string");
  source.Append ("three string");

  ObTrove<Str> dest (std::move (source));

  EXPECT_EQ (dest.Count (), 3);
  EXPECT_EQ (dest.Nth (0), "one string");
  EXPECT_EQ (dest.Nth (1), "two string");
  EXPECT_EQ (dest.Nth (2), "three string");
}

TEST (MoveTest, ObTroveMoveAssignNotPointy)
{
  static_assert (noexcept (Str ()), "Str's ctor should be noexcept");
  ObTrove<Str> dest;

  {
    ObTrove<Str> source;
    source.Append ("one string");
    source.Append ("two string");
    source.Append ("three string");

    dest = std::move (source);
  }


  EXPECT_EQ (dest.Count (), 3);
  EXPECT_EQ (dest.Nth (0), "one string");
  EXPECT_EQ (dest.Nth (1), "two string");
  EXPECT_EQ (dest.Nth (2), "three string");
}

TEST (MoveTest, ObMapMoveCtor)
{
  Str one ("one");
  Str two ("two");
  Str three ("three");

  AnkleObject *ao_one = new AnkleObject ();
  AnkleObject *ao_two = new AnkleObject ();
  AnkleObject *ao_three = new AnkleObject ();

  auto dup = ObMap<Str, AnkleObject *>::Privilege_Earliest;

  ObMap<Str, AnkleObject *> mappy;
  mappy.SetDupKeyBehavior (dup);
  mappy.Put (one, ao_one);
  mappy.Put (two, ao_two);
  mappy.Put (three, ao_three);

  ObMap<Str, AnkleObject *> dest (std::move (mappy));
  EXPECT_EQ (dest.DupKeyBehavior (), dup);
  EXPECT_EQ (dest.Find (one), ao_one);
  EXPECT_EQ (dest.Find (two), ao_two);
  EXPECT_EQ (dest.Find (three), ao_three);

  EXPECT_EQ (one, dest.KeyFromVal (ao_one));
  EXPECT_EQ (two, dest.KeyFromVal (ao_two));
  EXPECT_EQ (three, dest.KeyFromVal (ao_three));
}

TEST (MoveTest, ObMapMoveAssign)
{
  ObMap<Str, AnkleObject *> dest;

  Str one ("one");
  Str two ("two");
  Str three ("three");

  AnkleObject *ao_one = new AnkleObject ();
  AnkleObject *ao_two = new AnkleObject ();
  AnkleObject *ao_three = new AnkleObject ();

  auto dup = ObMap<Str, AnkleObject *>::Privilege_Earliest;

  {
    ObMap<Str, AnkleObject *> mappy;
    mappy.SetDupKeyBehavior (dup);
    mappy.Put (one, ao_one);
    mappy.Put (two, ao_two);
    mappy.Put (three, ao_three);

    dest = std::move (mappy);
  }

  EXPECT_EQ (dest.DupKeyBehavior (), dup);
  EXPECT_EQ (dest.Find (one), ao_one);
  EXPECT_EQ (dest.Find (two), ao_two);
  EXPECT_EQ (dest.Find (three), ao_three);

  EXPECT_EQ (one, dest.KeyFromVal (ao_one));
  EXPECT_EQ (two, dest.KeyFromVal (ao_two));
  EXPECT_EQ (three, dest.KeyFromVal (ao_three));
}

TEST (MoveTest, ObUniqueTroveMoveCtorIsPointyToBeWrapped)
{
  ObUniqueTrove<AnkleObject *> ao_trove;

  AnkleObject *ao1 = new AnkleObject;
  AnkleObject *ao2 = new AnkleObject;
  AnkleObject *ao3 = new AnkleObject;

  ao_trove.Append (ao1);
  ao_trove.Append (ao2);
  ao_trove.Append (ao3);


  ObUniqueTrove<AnkleObject *> ao_moved (std::move (ao_trove));

  EXPECT_EQ (3, ao_moved.Count ());
  EXPECT_EQ (0, ao_moved.Find (ao1));
  EXPECT_EQ (1, ao_moved.Find (ao2));
  EXPECT_EQ (2, ao_moved.Find (ao3));
  EXPECT_EQ (OB_ALREADY_PRESENT, ao_moved.Append (ao1));
}

TEST (MoveTest, ObUniqueTroveMoveAssignIsPointyToBeWrapped)
{
  ObUniqueTrove<AnkleObject *> ao_moved;
  AnkleObject *ao1 = new AnkleObject;
  AnkleObject *ao2 = new AnkleObject;
  AnkleObject *ao3 = new AnkleObject;

  {
    ObUniqueTrove<AnkleObject *> ao_trove;
    ao_trove.Append (ao1);
    ao_trove.Append (ao2);
    ao_trove.Append (ao3);

    ao_moved = std::move (ao_trove);
  }

  EXPECT_EQ (3, ao_moved.Count ());
  EXPECT_EQ (0, ao_moved.Find (ao1));
  EXPECT_EQ (1, ao_moved.Find (ao2));
  EXPECT_EQ (2, ao_moved.Find (ao3));
  EXPECT_EQ (OB_ALREADY_PRESENT, ao_moved.Append (ao1));
}

TEST (MoveTest, ObUniqueTroveMoveCtorIsPointyNotToBeWrapped)
{
  ObUniqueTrove<AnkleObject *> ao_trove;

  AnkleObject *ao1 = new AnkleObject;
  AnkleObject *ao2 = new AnkleObject;
  AnkleObject *ao3 = new AnkleObject;

  ao_trove.Append (ao1);
  ao_trove.Append (ao2);
  ao_trove.Append (ao3);

  ObUniqueTrove<AnkleObject *> ao_moved (std::move (ao_trove));

  for (AnkleObject *ao : ao_trove)
    ao->Delete ();

  ao_trove.Empty ();


  EXPECT_EQ (3, ao_moved.Count ());
  EXPECT_EQ (0, ao_moved.Find (ao1));
  EXPECT_EQ (1, ao_moved.Find (ao2));
  EXPECT_EQ (2, ao_moved.Find (ao3));
  EXPECT_EQ (OB_ALREADY_PRESENT, ao_moved.Append (ao1));

  for (AnkleObject *ao : ao_moved)
    ao->Delete ();

  ao_moved.Empty ();
}

TEST (MoveTest, ObUniqueTroveMoveAssignIsPointyNotToBeWrapped)
{
  ObUniqueTrove<AnkleObject *> ao_moved;
  AnkleObject *ao1 = new AnkleObject;
  AnkleObject *ao2 = new AnkleObject;
  AnkleObject *ao3 = new AnkleObject;

  {
    ObUniqueTrove<AnkleObject *> ao_trove;
    ao_trove.Append (ao1);
    ao_trove.Append (ao2);
    ao_trove.Append (ao3);

    ao_moved = std::move (ao_trove);
    for (AnkleObject *ao : ao_trove)
      ao->Delete ();

    ao_trove.Empty ();
  }

  EXPECT_EQ (3, ao_moved.Count ());
  EXPECT_EQ (0, ao_moved.Find (ao1));
  EXPECT_EQ (1, ao_moved.Find (ao2));
  EXPECT_EQ (2, ao_moved.Find (ao3));
  EXPECT_EQ (OB_ALREADY_PRESENT, ao_moved.Append (ao1));

  for (AnkleObject *ao : ao_moved)
    ao->Delete ();

  ao_moved.Empty ();
}

TEST (MoveTest, ObUniqueTroveMoveCtorNotPointy)
{
  static_assert (noexcept (Str ()), "Str's ctor should be noexcept");

  ObUniqueTrove<Str> source;
  source.Append ("one string");
  source.Append ("two string");
  source.Append ("three string");

  ObUniqueTrove<Str> dest (std::move (source));

  EXPECT_EQ (dest.Count (), 3);
  EXPECT_EQ (dest.Nth (0), "one string");
  EXPECT_EQ (dest.Nth (1), "two string");
  EXPECT_EQ (dest.Nth (2), "three string");
  EXPECT_EQ (OB_ALREADY_PRESENT, dest.Append ("one string"));
}

TEST (MoveTest, ObUniqueTroveMoveAssignNotPointy)
{
  static_assert (noexcept (Str ()), "Str's ctor should be noexcept");
  ObUniqueTrove<Str> dest;

  {
    ObUniqueTrove<Str> source;
    source.Append ("one string");
    source.Append ("two string");
    source.Append ("three string");

    dest = std::move (source);
  }


  EXPECT_EQ (dest.Count (), 3);
  EXPECT_EQ (dest.Nth (0), "one string");
  EXPECT_EQ (dest.Nth (1), "two string");
  EXPECT_EQ (dest.Nth (2), "three string");
  EXPECT_EQ (OB_ALREADY_PRESENT, dest.Append ("one string"));
}

TEST (MoveTest, ObAcaciaCopy)
{
  ObAcacia<Str> set;
  set.Append ("thingy");
  ObAcacia<Str> set2 (set);
}

TEST (MoveTest, ObAcaciaMoveCtorNotPointy)
{
  ObAcacia<Str> set;
  set.Append ("one");
  set.Append ("two");
  set.Append ("three");


  ObAcacia<Str> dest (std::move (set));
  EXPECT_EQ (dest.Count (), 3);
  EXPECT_TRUE (dest.Contains ("one"));
  EXPECT_TRUE (dest.Contains ("two"));
  EXPECT_TRUE (dest.Contains ("three"));
  EXPECT_FALSE (dest.Contains ("four"));
}

TEST (MoveTest, ObAcaciaMoveAssignNotPointy)
{
  ObAcacia<Str> dest;

  {
    ObAcacia<Str> set;
    set.Append ("one");
    set.Append ("two");
    set.Append ("three");

    dest = std::move (set);
  }

  EXPECT_EQ (dest.Count (), 3);
  EXPECT_TRUE (dest.Contains ("one"));
  EXPECT_TRUE (dest.Contains ("two"));
  EXPECT_TRUE (dest.Contains ("three"));
  EXPECT_FALSE (dest.Contains ("four"));
}

TEST (MoveTest, ObAcaciaMoveCtorPointy)
{
  AnkleObject *ao1 = new AnkleObject;
  AnkleObject *ao2 = new AnkleObject;
  AnkleObject *ao3 = new AnkleObject;

  ObAcacia<AnkleObject *> set;
  set.Append (ao1);
  set.Append (ao2);
  set.Append (ao3);

  ObRef<AnkleObject *> reffy = new AnkleObject;

  ObAcacia<AnkleObject *> dest (std::move (set));
  EXPECT_EQ (dest.Count (), 3);
  EXPECT_TRUE (dest.Contains (ao1));
  EXPECT_TRUE (dest.Contains (ao2));
  EXPECT_TRUE (dest.Contains (ao3));
  EXPECT_FALSE (dest.Contains (~reffy));
}

TEST (MoveTest, ObAcaciaMoveAssignPointy)
{
  ObAcacia<AnkleObject *> dest;

  AnkleObject *ao1 = new AnkleObject;
  AnkleObject *ao2 = new AnkleObject;
  AnkleObject *ao3 = new AnkleObject;

  {
    ObAcacia<AnkleObject *> set;
    set.Append (ao1);
    set.Append (ao2);
    set.Append (ao3);

    dest = std::move (set);
  }

  ObRef<AnkleObject *> reffy = new AnkleObject;

  EXPECT_EQ (dest.Count (), 3);
  EXPECT_TRUE (dest.Contains (ao1));
  EXPECT_TRUE (dest.Contains (ao2));
  EXPECT_TRUE (dest.Contains (ao3));
  EXPECT_FALSE (dest.Contains (~reffy));
}

TEST (MoveTest, ObTroveObCrawlMoveCtor)
{
  ObTrove<Str> strs;

  Str one ("one");
  Str two ("two");
  Str three ("three");

  strs.Append (one);
  strs.Append (two);
  strs.Append (three);

  ObCrawl<Str> oc = strs.Crawl ();

  ObCrawl<Str> dest (std::move (oc));

  EXPECT_FALSE (dest.isempty ());
  EXPECT_EQ (dest.popfore (), one);
  EXPECT_EQ (dest.popfore (), two);
  EXPECT_EQ (dest.popfore (), three);
  EXPECT_TRUE (dest.isempty ());
}

TEST (MoveTest, ObTroveObCrawlMoveAssign)
{
  ObTrove<Str> strs;
  ObCrawl<Str> dest;

  Str one ("one");
  Str two ("two");
  Str three ("three");

  strs.Append (one);
  strs.Append (two);
  strs.Append (three);

  {
    ObCrawl<Str> oc = strs.Crawl ();
    dest = std::move (oc);
  }


  EXPECT_FALSE (dest.isempty ());
  EXPECT_EQ (dest.popfore (), one);
  EXPECT_EQ (dest.popfore (), two);
  EXPECT_EQ (dest.popfore (), three);
  EXPECT_TRUE (dest.isempty ());
}

TEST (MoveTest, ObHormelMoveCtor)
{
  ObHormel<Str> strs;

  Str elephant ("elephant");
  Str giraffe ("giraffe");
  Str rhino ("rhino");
  Str tiger ("tiger");
  Str bear ("bear");
  Str ohmy ("oh my");

  strs.Append (elephant);
  strs.Append (giraffe);
  strs.Append (rhino);
  strs.Append (tiger);
  strs.Append (bear);
  strs.Append (ohmy);

  int64 nentries = strs.Count ();
  int64 nbuckets = strs.BucketCount ();
  float64 load = strs.LoadFactor ();

  ObHormel<Str> dest (std::move (strs));

  strs.Append ("one");

  EXPECT_EQ (nentries, dest.Count ());
  EXPECT_EQ (nbuckets, dest.BucketCount ());
  EXPECT_EQ (load, dest.LoadFactor ());
  EXPECT_TRUE (dest.Contains (elephant));
  EXPECT_TRUE (dest.Contains (giraffe));
  EXPECT_TRUE (dest.Contains (rhino));
  EXPECT_TRUE (dest.Contains (tiger));
  EXPECT_TRUE (dest.Contains (bear));
  EXPECT_TRUE (dest.Contains (ohmy));
  EXPECT_FALSE (dest.Contains ("whatever"));
}

TEST (MoveTest, ObHormelMoveAssign)
{
  ObHormel<Str> dest;

  Str elephant ("elephant");
  Str giraffe ("giraffe");
  Str rhino ("rhino");
  Str tiger ("tiger");
  Str bear ("bear");
  Str ohmy ("oh my");

  int64 nentries = 0;
  int64 nbuckets = 0;
  float64 load = 0.0;

  {
    ObHormel<Str> strs;
    strs.Append (elephant);
    strs.Append (giraffe);
    strs.Append (rhino);
    strs.Append (tiger);
    strs.Append (bear);
    strs.Append (ohmy);

    nentries = strs.Count ();
    nbuckets = strs.BucketCount ();
    load = strs.LoadFactor ();

    dest = std::move (strs);
    strs.Append ("one");
  }


  EXPECT_EQ (nentries, dest.Count ());
  EXPECT_EQ (nbuckets, dest.BucketCount ());
  EXPECT_EQ (load, dest.LoadFactor ());
  EXPECT_TRUE (dest.Contains (elephant));
  EXPECT_TRUE (dest.Contains (giraffe));
  EXPECT_TRUE (dest.Contains (rhino));
  EXPECT_TRUE (dest.Contains (tiger));
  EXPECT_TRUE (dest.Contains (bear));
  EXPECT_TRUE (dest.Contains (ohmy));
  EXPECT_FALSE (dest.Contains ("whatever"));
}

class County : public AnkleObject
{
  PATELLA_SUBCLASS (County, AnkleObject);

 public:
  static int64 extant_count;

  County () : AnkleObject () { ++extant_count; }

  virtual ~County () { --extant_count; }
};

int64 County::extant_count = 0;

TEST (MoveTest, ObRefMoveCtor)
{
  ObRef<County *> c (new County);

  EXPECT_EQ (1, County::extant_count);

  {
    ObRef<County *> dest (std::move (c));
    EXPECT_EQ (1, County::extant_count);
  }

  EXPECT_EQ (0, County::extant_count);
}

TEST (MoveTest, ObRefMoveAssign)
{
  {
    ObRef<County *> dest;

    {
      ObRef<County *> c (new County);

      EXPECT_EQ (1, County::extant_count);

      dest = std::move (c);
      EXPECT_EQ (1, County::extant_count);
    }

    EXPECT_EQ (1, County::extant_count);
  }

  EXPECT_EQ (0, County::extant_count);
}

TEST (MoveTest, ObWeakRefMoveCtor)
{
  ObRef<County *> c (new County);

  ASSERT_EQ (1, County::extant_count);

  {
    ObWeakRef<County *> wc (c);
    auto *wgm =
      dynamic_cast<RefCountGripMinder<County>::RefCountWeakGripMinder *> (
        wc.weak_grip_mnd);
    EXPECT_EQ (1, wgm->wk_count);
    ObWeakRef<County *> dest (std::move (wc));
    wgm = dynamic_cast<RefCountGripMinder<County>::RefCountWeakGripMinder *> (
      dest.weak_grip_mnd);
    EXPECT_EQ (1, wgm->wk_count);
  }

  auto *gm =
    dynamic_cast<RefCountGripMinder<County> *> (c.guts.ItsGripMinder ());
  EXPECT_EQ (0, gm->weak_grip_mnd->wk_count);
}

TEST (MoveTest, ObWeakRefMoveAssign)
{
  ObWeakRef<County *> dest;
  {
    ObRef<County *> c (new County);

    ASSERT_EQ (1, County::extant_count);

    {
      ObWeakRef<County *> wc (c);
      auto *wgm =
        dynamic_cast<RefCountGripMinder<County>::RefCountWeakGripMinder *> (
          wc.weak_grip_mnd);
      EXPECT_EQ (1, wgm->wk_count);
      dest = std::move (wc);
      wgm = dynamic_cast<RefCountGripMinder<County>::RefCountWeakGripMinder *> (
        dest.weak_grip_mnd);
      EXPECT_EQ (1, wgm->wk_count);
    }

    auto *gm =
      dynamic_cast<RefCountGripMinder<County> *> (c.guts.ItsGripMinder ());
    EXPECT_EQ (1, gm->weak_grip_mnd->wk_count);
  }

  auto *wgm =
    dynamic_cast<RefCountGripMinder<County>::RefCountWeakGripMinder *> (
      dest.weak_grip_mnd);
  EXPECT_EQ (1, wgm->wk_count);
}
