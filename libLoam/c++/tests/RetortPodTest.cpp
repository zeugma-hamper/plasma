
/* (c)  oblong industries */

#include "libLoam/c/ob-sys.h"

#include <libLoam/c++/ObRetort.h>
#include <libLoam/c++/Str.h>

#include <libLoam/c/ob-pthread.h>

#include <gtest/gtest.h>

#include <errno.h>


using namespace oblong::loam;


namespace {



static int64 whack_count = 0;
static Str whack_phrase;

void ResetWhackage ()
{
  whack_count = 0;
  whack_phrase = "";
}


class TestyPod : public ObRetortPod
{
  PATELLA_SUBCLASS (TestyPod, ObRetortPod);

 public:
  Str nomer;
  TestyPod (const Str &nm) : ObRetortPod (), nomer (nm) {}
  ~TestyPod () override
  {
    whack_phrase = "dead to me: " + nomer;
    whack_count++;
  }
};


static ObRetortPod *norp = NULL;


ob_retort torts[] = {OB_OK, OB_BAD_INDEX, OB_NO_MEM, 123, OB_ARGUMENT_WAS_NULL};

Str nomers[] = {"little billy", "aunt hildegard", "mr. skibbs",
                "lucifer mcsamples", "lady lintbatten"};

const int64 TCOUNT (sizeof (torts) / sizeof (torts[0]));



ObRetort ServeUpRetort (ob_retort tort, const Str &nm = "")
{
  ObRetort ort (tort);
  ort.SetRetortPod (new TestyPod (nm));
  return ort;
}


TEST (RetortPodTest, BasicCareAndFeeding)
{
  ResetWhackage ();
  {
    ObRetort ort = ServeUpRetort (torts[0], nomers[0]);
    EXPECT_EQ (whack_count, 0);
    EXPECT_STREQ ("", whack_phrase);
    ObRetortPod *orp = ort.RetortPod ();
    ASSERT_NE (orp, norp);
    EXPECT_EQ (orp->NumericRetort (), torts[0]);
  }
  EXPECT_EQ (whack_count, 1);
  EXPECT_STREQ ("dead to me: little billy", whack_phrase);
}



ObRetort PresentRecursiveRetort (int64 lvl)
{
  ObRetort ort (torts[lvl]);
  ort.SetRetortPod (new TestyPod (nomers[lvl]));
  if (lvl + 1 < TCOUNT)
    {
      ObRetort up_tort = PresentRecursiveRetort (lvl + 1);
      ort.RetortPod<TestyPod> ()->SetAntecedent (up_tort.RetortPod ());
    }
  return ort;
}


TEST (RetortPodTest, PodAccretion)
{
  ResetWhackage ();
  {
    ObRetort ort = PresentRecursiveRetort (0);
    EXPECT_EQ (whack_count, 0);
    EXPECT_STREQ ("", whack_phrase);
    TestyPod *tp = ort.RetortPod<TestyPod> ();
    for (int64 q = 0; q < TCOUNT; q++)
      {
        ASSERT_NE (tp, norp);
        EXPECT_EQ (torts[q], tp->NumericRetort ());
        EXPECT_STREQ (nomers[q], tp->nomer);
        tp = tp->Antecedent<TestyPod> ();
      }
    ASSERT_EQ (tp, norp);
  }
  EXPECT_EQ (whack_count, TCOUNT);
  EXPECT_STREQ ("dead to me: lady lintbatten", whack_phrase);
}


}  // namespace
