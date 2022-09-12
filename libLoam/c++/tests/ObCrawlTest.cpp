
/* (c)  oblong industries */

#include <gtest/gtest.h>

#include "libLoam/c++/ObTrove.h"
#include "libLoam/c++/Str.h"

using namespace oblong::loam;


static const Str poe[3] = {"darkness", "decay", "the red death"};
static const Str poem[3] = {"lovely", "as a", "tree"};
static const Str pug[2] = {"jowls", "slobber"};

TEST (ObCrawlTest, Zippitudinousness)
{
  ObTrove<Str> tro_a (poe, 3);
  ObTrove<Str> tro_b (poem, 3);
  ObTrove<Str> tro_c (pug, 2);

  ObCrawl<Str> cr = tro_a.Crawl ().zip (tro_b.Crawl ());
  EXPECT_STREQ (cr.popfore (), "darkness");
  EXPECT_STREQ (cr.popfore (), "lovely");
  EXPECT_STREQ (cr.popfore (), "decay");
  EXPECT_STREQ (cr.popfore (), "as a");
  EXPECT_STREQ (cr.popfore (), "the red death");
  EXPECT_STREQ (cr.popfore (), "tree");
  EXPECT_TRUE (cr.isempty ());

  cr = tro_a.Crawl ().zip (tro_b.Crawl (), tro_c.Crawl ());
  EXPECT_STREQ (cr.popfore (), "darkness");
  EXPECT_STREQ (cr.popfore (), "lovely");
  EXPECT_STREQ (cr.popfore (), "jowls");
  EXPECT_STREQ (cr.popfore (), "decay");
  EXPECT_STREQ (cr.popfore (), "as a");
  EXPECT_STREQ (cr.popfore (), "slobber");
  EXPECT_TRUE (cr.isempty ());
}

TEST (ObCrawlTest, SelfCopy)
{
  ObTrove<Str> str_trove;
  str_trove.Append ("one");
  str_trove.Append ("two");
  str_trove.Append ("three");

  ObCrawl<Str> str_crawl_1 = str_trove.Crawl ();
  str_crawl_1 =
    *&str_crawl_1;  // annotate with *& to tell clang we intend self-assignment
  EXPECT_EQ (str_trove.Nth (0), str_crawl_1.popfore ());
  EXPECT_EQ (str_trove.Nth (1), str_crawl_1.popfore ());
  EXPECT_EQ (str_trove.Nth (2), str_crawl_1.popfore ());
}

TEST (ObCrawlTest, CopyCopy)
{
  ObTrove<Str> str_trove;
  str_trove.Append ("one");
  str_trove.Append ("two");
  str_trove.Append ("three");

  ObCrawl<Str> str_crawl_1 = str_trove.Crawl ();
  ObCrawl<Str> str_crawl_2;
  str_crawl_2 = str_crawl_1;

  EXPECT_EQ (str_trove.Nth (0), str_crawl_1.popfore ());
  EXPECT_EQ (str_trove.Nth (1), str_crawl_1.popfore ());
  EXPECT_EQ (str_trove.Nth (2), str_crawl_1.popfore ());

  EXPECT_EQ (str_trove.Nth (0), str_crawl_2.popfore ());
  EXPECT_EQ (str_trove.Nth (1), str_crawl_2.popfore ());
  EXPECT_EQ (str_trove.Nth (2), str_crawl_2.popfore ());
}
