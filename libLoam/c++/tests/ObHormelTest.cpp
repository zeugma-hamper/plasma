
/* (c)  oblong industries */

#include <gtest/gtest.h>
#include <libLoam/c++/ObHormel.h>
#include <libLoam/c++/Str.h>
#include <libLoam/c/ob-coretypes.h>
#include <libLoam/c++/AnkleObject.h>
#include <libLoam/c++/ObAcacia.h>

using namespace oblong::loam;

TEST (ObHormelTest, EntryCount)
{
  ObHormel<unt32> horm;
  unsigned int b = 23;
  horm.Append (b);
  EXPECT_EQ (7, horm.BucketCount ());
  EXPECT_EQ (1, horm.EntryCount ());

  horm.Append (57);
  EXPECT_EQ (7, horm.BucketCount ());
  EXPECT_EQ (2, horm.EntryCount ());
  horm.Append (57);
  EXPECT_EQ (2, horm.EntryCount ());
  horm.Remove (23);
  EXPECT_EQ (1, horm.EntryCount ());
}

TEST (ObHormelTest, AnklesEmpty)
{
  ObHormel<AnkleObject *> ankle_hash;
  EXPECT_EQ (0, ankle_hash.Count ());

  ObCrawl<AnkleObject *> cr (ankle_hash.Crawl ());
  EXPECT_TRUE (cr.isempty ());
}

TEST (ObHormelTest, Ankles)
{
  AnkleObject *ao = new AnkleObject ();
  ObWeakRef<AnkleObject *> wao (ao);
  {
    ObHormel<AnkleObject *> ankle_hash;
    ankle_hash.Append (ao);

    EXPECT_EQ (true, ankle_hash.Contains (ao));
  }

  EXPECT_EQ (NULL, ~wao);
}

TEST (ObHormelTest, Remove)
{
  AnkleObject *ao = new AnkleObject ();
  ObWeakRef<AnkleObject *> wao (ao);
  ObHormel<AnkleObject *> ankle_hash;
  ankle_hash.Append (ao);

  EXPECT_EQ (true, ankle_hash.Contains (ao));
  EXPECT_EQ (OB_OK, ankle_hash.Remove (ao));

  EXPECT_EQ (NULL, ~wao);
}

TEST (ObHormelTest, Stacking)
{
  ObHormel<unt32> horm;

  EXPECT_EQ (OB_OK, horm.Append (4));
  EXPECT_EQ (OB_OK, horm.Append (11));
  EXPECT_EQ (OB_OK, horm.Append (18));
  EXPECT_EQ (OB_OK, horm.Append (25));

  EXPECT_EQ (true, horm.Contains (4));
  EXPECT_EQ (true, horm.Contains (11));
  EXPECT_EQ (true, horm.Contains (18));
  EXPECT_EQ (true, horm.Contains (25));
}

TEST (ObHormelTest, Contains)
{
  ObHormel<unt32> horm;

  EXPECT_FALSE (horm.Contains (0));

  EXPECT_EQ (OB_OK, horm.Append (4));
  EXPECT_EQ (OB_OK, horm.Append (5));
  EXPECT_EQ (OB_OK, horm.Append (6));
  EXPECT_EQ (OB_OK, horm.Append (7));

  EXPECT_TRUE (horm.Contains (4));
  EXPECT_TRUE (horm.Contains (5));
  EXPECT_TRUE (horm.Contains (6));
  EXPECT_TRUE (horm.Contains (7));

  EXPECT_FALSE (horm.Contains (0));
  EXPECT_FALSE (horm.Contains (1));
  EXPECT_FALSE (horm.Contains (2));
  EXPECT_FALSE (horm.Contains (3));
}

// default size is 7 and default max load factor is .75
// we'll upsize by filling up the set and check again for membership
TEST (ObHormelTest, Upsize)
{
  ObHormel<unt32> horm;

  EXPECT_EQ (OB_OK, horm.Append (20));
  EXPECT_EQ (OB_OK, horm.Append (25));
  EXPECT_EQ (OB_OK, horm.Append (49));
  EXPECT_EQ (OB_OK, horm.Append (35));
  EXPECT_EQ (OB_OK, horm.Append (2));
  EXPECT_EQ (OB_OK, horm.Append (5));
  EXPECT_EQ (OB_OK, horm.Append (271));
  EXPECT_EQ (OB_OK, horm.Append (90));
  EXPECT_EQ (OB_OK, horm.Append (101));

  EXPECT_TRUE (horm.BucketCount () > 7);
  EXPECT_EQ (horm.EntryCount (), 9);

  EXPECT_EQ (true, horm.Contains (20));
  EXPECT_EQ (true, horm.Contains (25));
  EXPECT_EQ (true, horm.Contains (49));
  EXPECT_EQ (true, horm.Contains (35));
  EXPECT_EQ (true, horm.Contains (2));
  EXPECT_EQ (true, horm.Contains (5));
  EXPECT_EQ (true, horm.Contains (271));
  EXPECT_EQ (true, horm.Contains (90));
  EXPECT_EQ (true, horm.Contains (101));
}

TEST (ObHormelTest, PointerTypes)
{
  Str *pa = new Str ("alpha");
  Str *pb = new Str ("bravo");
  Str *pc = new Str ("charlie");

  ObHormel<Str *> horm_ptrs;

  EXPECT_FALSE (horm_ptrs.Contains (NULL));
  EXPECT_FALSE (horm_ptrs.Contains (pa));
  EXPECT_FALSE (horm_ptrs.Contains (pb));
  EXPECT_FALSE (horm_ptrs.Contains (pc));

  EXPECT_EQ (OB_OK, horm_ptrs.Append (pa));
  EXPECT_EQ (OB_OK, horm_ptrs.Append (pb));

  EXPECT_EQ (2, horm_ptrs.EntryCount ());
  EXPECT_FALSE (horm_ptrs.Contains (NULL));
  EXPECT_TRUE (horm_ptrs.Contains (pa));
  EXPECT_TRUE (horm_ptrs.Contains (pb));
  EXPECT_FALSE (horm_ptrs.Contains (pc));

  EXPECT_EQ (OB_OK, horm_ptrs.Append (NULL));

  EXPECT_EQ (3, horm_ptrs.EntryCount ());
  EXPECT_TRUE (horm_ptrs.Contains (NULL));
  EXPECT_TRUE (horm_ptrs.Contains (pa));
  EXPECT_TRUE (horm_ptrs.Contains (pb));
  EXPECT_FALSE (horm_ptrs.Contains (pc));

  delete pc;
}

class CountingAnkleObject : public AnkleObject
{
  PATELLA_SUBCLASS (CountingAnkleObject, AnkleObject);

 public:
  static int count;

  CountingAnkleObject () : AnkleObject () { ++count; }
  ~CountingAnkleObject () override { --count; }

  static void ResetCount () { count = 0; }

  static int Count () { return count; }
};
int CountingAnkleObject::count = 0;

TEST (ObHormelTest, UpsizeAnkleObject)
{
  CountingAnkleObject::ResetCount ();

  {
    ObHormel<AnkleObject *> horm;

    for (int32 i = 0; i < 9; i++)
      EXPECT_EQ (OB_OK, horm.Append (new CountingAnkleObject ()));

    EXPECT_TRUE (horm.BucketCount () > 7);
    EXPECT_EQ (horm.EntryCount (), 9);
    EXPECT_EQ (CountingAnkleObject::Count (), 9);
  }

  EXPECT_EQ (CountingAnkleObject::Count (), 0);
}

TEST (ObHormelTest, EmptyAnkleObject)
{
  CountingAnkleObject::ResetCount ();

  ObHormel<AnkleObject *> horm;

  for (int32 i = 0; i < 9; i++)
    EXPECT_EQ (OB_OK, horm.Append (new CountingAnkleObject ()));

  EXPECT_TRUE (horm.BucketCount () > 7);
  EXPECT_EQ (horm.EntryCount (), 9);
  EXPECT_EQ (CountingAnkleObject::Count (), 9);

  EXPECT_EQ (OB_OK, horm.Empty ());
  EXPECT_EQ (CountingAnkleObject::Count (), 0);
}

#include <libLoam/c++/Str.h>
TEST (ObHormelTest, CharStar)
{
  ObHormel<Str> horm;
  EXPECT_EQ (OB_OK, horm.Append ("hormel"));
}

#include <libLoam/c++/ObTrove.h>
#include <libLoam/c++/LoamStreams.h>
TEST (ObHormelTest, CrawlFore)
{
  ObHormel<unt32> horm;

  EXPECT_EQ (OB_OK, horm.Append (20));
  EXPECT_EQ (OB_OK, horm.Append (25));
  EXPECT_EQ (OB_OK, horm.Append (49));
  EXPECT_EQ (OB_OK, horm.Append (35));
  EXPECT_EQ (OB_OK, horm.Append (2));
  EXPECT_EQ (OB_OK, horm.Append (5));
  EXPECT_EQ (OB_OK, horm.Append (271));
  EXPECT_EQ (OB_OK, horm.Append (90));
  EXPECT_EQ (OB_OK, horm.Append (101));

  ObTrove<unt32> trove;
  trove.Append (20);
  trove.Append (25);
  trove.Append (49);
  trove.Append (35);
  trove.Append (2);
  trove.Append (5);
  trove.Append (271);
  trove.Append (90);
  trove.Append (101);

  ObCrawl<unt32> cr = horm.Crawl ();
  while (!cr.isempty ())
    {
      unt32 i = cr.popfore ();
      EXPECT_EQ (OB_OK, trove.Remove (i));
    }

  EXPECT_EQ (0, trove.Count ());
}

#include <libLoam/c++/ObTrove.h>
#include <libLoam/c++/LoamStreams.h>
TEST (ObHormelTest, CrawlAft)
{
  ObHormel<unt32> horm;

  EXPECT_EQ (OB_OK, horm.Append (20));
  EXPECT_EQ (OB_OK, horm.Append (25));
  EXPECT_EQ (OB_OK, horm.Append (49));
  EXPECT_EQ (OB_OK, horm.Append (35));
  EXPECT_EQ (OB_OK, horm.Append (2));
  EXPECT_EQ (OB_OK, horm.Append (5));
  EXPECT_EQ (OB_OK, horm.Append (271));
  EXPECT_EQ (OB_OK, horm.Append (90));
  EXPECT_EQ (OB_OK, horm.Append (101));

  ObTrove<unt32> trove;
  trove.Append (20);
  trove.Append (25);
  trove.Append (49);
  trove.Append (35);
  trove.Append (2);
  trove.Append (5);
  trove.Append (271);
  trove.Append (90);
  trove.Append (101);

  ObCrawl<unt32> cr = horm.Crawl ();
  while (!cr.isempty ())
    {
      unt32 i = cr.popaft ();
      EXPECT_EQ (OB_OK, trove.Remove (i));
    }

  EXPECT_EQ (0, trove.Count ());
}

TEST (ObHormelTest, CrawlStr)
{
  ObHormel<Str> horm;
  ObTrove<Str> trove;

  EXPECT_EQ (OB_OK, horm.Append ("one"));
  EXPECT_EQ (OB_OK, horm.Append ("two"));
  EXPECT_EQ (OB_OK, horm.Append ("three"));
  EXPECT_EQ (OB_OK, horm.Append ("four"));
  EXPECT_EQ (OB_OK, horm.Append ("five"));
  EXPECT_EQ (OB_OK, horm.Append ("six"));
  EXPECT_EQ (OB_OK, horm.Append ("seven"));
  EXPECT_EQ (OB_OK, horm.Append ("eight"));
  EXPECT_EQ (OB_OK, horm.Append ("nine"));
  EXPECT_EQ (OB_OK, horm.Append ("ten"));

  trove.Append ("one");
  trove.Append ("two");
  trove.Append ("three");
  trove.Append ("four");
  trove.Append ("five");
  trove.Append ("six");
  trove.Append ("seven");
  trove.Append ("eight");
  trove.Append ("nine");
  trove.Append ("ten");

  ObCrawl<Str> cr = horm.Crawl ();
  while (!cr.isempty ())
    {
      Str blorp = cr.popfore ();
      EXPECT_EQ (OB_OK, trove.Remove (blorp));
      if (!cr.isempty ())
        {
          blorp = cr.popaft ();
          EXPECT_EQ (OB_OK, trove.Remove (blorp));
        }
    }

  EXPECT_EQ (0, trove.Count ());
}

TEST (ObHormelTest, CompactNullsAnkleObject)
{
  ObHormel<AnkleObject *, WeakRef> horm;
  ObTrove<AnkleObject *> outer;
  int64 inner_count = 0;

  {
    ObTrove<AnkleObject *> inner;
    AnkleObject *ao = new AnkleObject ();
    inner.Append (ao);
    horm.Append (ao);

    ao = new AnkleObject ();
    outer.Append (ao);
    horm.Append (ao);

    ao = new AnkleObject ();
    inner.Append (ao);
    horm.Append (ao);

    ao = new AnkleObject ();
    outer.Append (ao);
    horm.Append (ao);

    ao = new AnkleObject ();
    inner.Append (ao);
    horm.Append (ao);

    ao = new AnkleObject ();
    outer.Append (ao);
    horm.Append (ao);

    ao = new AnkleObject ();
    inner.Append (ao);
    horm.Append (ao);

    ao = new AnkleObject ();
    outer.Append (ao);
    horm.Append (ao);

    ao = new AnkleObject ();
    inner.Append (ao);
    horm.Append (ao);

    ao = new AnkleObject ();
    outer.Append (ao);
    horm.Append (ao);

    inner_count = inner.Count ();
  }

  EXPECT_EQ (inner_count, horm.CompactNulls ());
}

TEST (ObHormelTest, TroveTest)
{
  ObTrove<int64> five;
  five.Append (1);
  five.Append (2);
  five.Append (3);
  five.Append (4);
  five.Append (5);

  ObTrove<int64> fifteen;
  fifteen.Append (11);
  fifteen.Append (12);
  fifteen.Append (13);
  fifteen.Append (14);
  fifteen.Append (15);

  ObHasher<ObTrove<int64>> h;
  h (five);
  h (fifteen);
}

TEST (ObHormelTest, AcaciaTest)
{
  ObAcacia<int64> five;
  five.Append (1);
  five.Append (2);
  five.Append (3);
  five.Append (4);
  five.Append (5);

  ObAcacia<int64> fifteen;
  fifteen.Append (11);
  fifteen.Append (12);
  fifteen.Append (13);
  fifteen.Append (14);
  fifteen.Append (15);

  ObHasher<ObAcacia<int64>> h;
  h (five);
  h (fifteen);
}

TEST (ObHormelTest, MapTest)
{
  ObMap<int64, int64> five;
  ;
  five.Put (1, 11);
  five.Put (2, 12);
  five.Put (3, 13);
  five.Put (4, 14);
  five.Put (5, 15);

  ObHasher<ObMap<int64, int64>> h;
  h (five);
}

TEST (ObHormelTest, HormelTest)
{
  ObHormel<int64> five;
  five.Append (1);
  five.Append (2);
  five.Append (3);
  five.Append (4);
  five.Append (5);

  ObHormel<int64> fifteen;
  fifteen.Append (11);
  fifteen.Append (12);
  fifteen.Append (13);
  fifteen.Append (14);
  fifteen.Append (15);

  ObHasher<ObHormel<int64>> h;
  h (five);
  h (fifteen);
}

TEST (ObHormelTest, CrawlTest)
{
  ObHormel<int64> horm;
  horm.Append (1);
  horm.Append (2);
  horm.Append (3);
  horm.Append (4);
  horm.Append (5);

  ObTrove<int64> trov;
  trov.Append (1);
  trov.Append (2);
  trov.Append (3);
  trov.Append (4);
  trov.Append (5);

  ObCrawl<int64> cr = horm.Crawl ();
  EXPECT_TRUE (trov.Contains (cr.fore ()));
  EXPECT_TRUE (trov.Contains (cr.aft ()));

  trov.Remove (cr.popfore ());
  trov.Remove (cr.popfore ());
  trov.Remove (cr.popfore ());
  trov.Remove (cr.popfore ());
  trov.Remove (cr.popfore ());

  EXPECT_TRUE (cr.isempty ());
  EXPECT_EQ (trov.Count (), 0);
}

TEST (ObHormelTest, CopyCtorIntTest)
{
  ObHormel<int64> horm;
  horm.Append (1);
  horm.Append (2);
  horm.Append (3);
  horm.Append (4);
  horm.Append (5);

  ObHormel<int64> horm_dup (horm);
  EXPECT_TRUE (horm_dup.Contains (1));
  EXPECT_TRUE (horm_dup.Contains (2));
  EXPECT_TRUE (horm_dup.Contains (3));
  EXPECT_TRUE (horm_dup.Contains (4));
  EXPECT_TRUE (horm_dup.Contains (5));
  EXPECT_EQ (horm.Count (), horm_dup.Count ());
}

TEST (ObHormelTest, CopyCtorAOTest)
{
  AnkleObject *ao1 = new AnkleObject ();
  AnkleObject *ao2 = new AnkleObject ();
  AnkleObject *ao3 = new AnkleObject ();
  AnkleObject *ao4 = new AnkleObject ();
  AnkleObject *ao5 = new AnkleObject ();


  ObHormel<AnkleObject *> horm;
  horm.Append (ao1);
  horm.Append (ao2);
  horm.Append (ao3);
  horm.Append (ao4);
  horm.Append (ao5);

  ObHormel<AnkleObject *> horm_dup (horm);
  EXPECT_TRUE (horm_dup.Contains (ao1));
  EXPECT_TRUE (horm_dup.Contains (ao2));
  EXPECT_TRUE (horm_dup.Contains (ao3));
  EXPECT_TRUE (horm_dup.Contains (ao4));
  EXPECT_TRUE (horm_dup.Contains (ao5));
  EXPECT_EQ (horm.Count (), horm_dup.Count ());
}

TEST (ObHormelTest, OpEqIntTest)
{
  ObHormel<int64> horm;
  horm.Append (1);
  horm.Append (2);
  horm.Append (3);
  horm.Append (4);
  horm.Append (5);

  ObHormel<int64> horm_dup;
  horm_dup.Append (11);
  horm_dup.Append (12);
  horm_dup.Append (13);
  horm_dup.Append (14);
  horm_dup.Append (15);
  horm_dup.Append (16);

  horm_dup = horm;
  EXPECT_TRUE (horm_dup.Contains (1));
  EXPECT_TRUE (horm_dup.Contains (2));
  EXPECT_TRUE (horm_dup.Contains (3));
  EXPECT_TRUE (horm_dup.Contains (4));
  EXPECT_TRUE (horm_dup.Contains (5));
  EXPECT_EQ (horm.Count (), horm_dup.Count ());
}

TEST (ObHormelTest, OpEqAOTest)
{
  AnkleObject *ao1 = new AnkleObject ();
  AnkleObject *ao2 = new AnkleObject ();
  AnkleObject *ao3 = new AnkleObject ();
  AnkleObject *ao4 = new AnkleObject ();
  AnkleObject *ao5 = new AnkleObject ();
  AnkleObject *ao6 = new AnkleObject ();
  AnkleObject *ao7 = new AnkleObject ();
  AnkleObject *ao8 = new AnkleObject ();
  AnkleObject *ao9 = new AnkleObject ();
  AnkleObject *ao10 = new AnkleObject ();
  AnkleObject *ao11 = new AnkleObject ();


  ObHormel<AnkleObject *> horm;
  horm.Append (ao1);
  horm.Append (ao2);
  horm.Append (ao3);
  horm.Append (ao4);
  horm.Append (ao5);

  ObHormel<AnkleObject *> horm_dup;
  horm_dup.Append (ao6);
  horm_dup.Append (ao7);
  horm_dup.Append (ao8);
  horm_dup.Append (ao9);
  horm_dup.Append (ao10);
  horm_dup.Append (ao11);

  horm_dup = horm;

  EXPECT_TRUE (horm_dup.Contains (ao1));
  EXPECT_TRUE (horm_dup.Contains (ao2));
  EXPECT_TRUE (horm_dup.Contains (ao3));
  EXPECT_TRUE (horm_dup.Contains (ao4));
  EXPECT_TRUE (horm_dup.Contains (ao5));
  EXPECT_TRUE (horm_dup.Find (ao5) >= 0);
  EXPECT_EQ (horm.Count (), horm_dup.Count ());
}

TEST (ObHormelTest, EnsureRoomTest)
{
  ObHormel<AnkleObject *> horm;
  horm.EnsureRoomFor (100);

  EXPECT_TRUE (horm.BucketCount () >= 100);
}

TEST (ObHormelTest, SizeCtorTest)
{
  ObHormel<AnkleObject *> horm (100);

  EXPECT_TRUE (horm.BucketCount () >= 100);
}

TEST (ObHormelTest, LoadFactorTest)
{
  ObHormel<AnkleObject *> horm (100);

  EXPECT_EQ (horm.LoadFactor (), 0.0);
}

static int livecount = 0;
class Dorf : public AnkleObject
{
  PATELLA_SUBCLASS (Dorf, AnkleObject);

 public:
  Str s;
  Dorf (Str s_) : s (s_) { livecount++; }
  ~Dorf () { livecount--; }
};

TEST (ObHormelTest, RangedForLoopInts)
{
  ObHormel<int64> intset;
  intset.Append (1);
  intset.Append (2);
  intset.Append (3);
  intset.Append (4);
  intset.Append (5);
  int sum = 0;
  for (auto i : intset)
    {
      static_assert (std::is_same<decltype (i), int64>::value,
                     "std::begin(ObHormel<int64>) should return an iterator "
                     "with elements of type int64");
      sum += i;
    }
  EXPECT_EQ (5, intset.Count ());
  EXPECT_EQ (15, sum);
}

TEST (ObHormelTest, RangedForLoopObRefs)
{
  ObHormel<Dorf *> dorves;
  ObHormel<Str> strings;
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
  EXPECT_TRUE (strings.Contains ("avert"));
  EXPECT_TRUE (strings.Contains ("bituminous"));
  EXPECT_TRUE (strings.Contains ("catastrophe"));
}
