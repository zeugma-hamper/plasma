
/* (c)  oblong industries */

// Unit tests for RefCounted

#include <RefCounted.h>

#include <gtest/gtest.h>

namespace {

class IntTraits
{
 public:
  static int deletions;
  static int def;
  static int Default () { return def; }
  static void Destroy (int k) { ++deletions; }
  static bool Equals (int i, int j) { return i == j; }
};
int IntTraits::deletions;
int IntTraits::def = 42;

typedef ::oblong::plasma::detail::RefCounted<int, IntTraits> RCInt;

class RefCountedTest : public ::testing::Test
{
 protected:
  void SetUp () override { IntTraits::deletions = 0; }
};

TEST_F (RefCountedTest, SingleObject)
{
  {
    RCInt i (2);
    EXPECT_EQ (2, i.Value ());
  }
  EXPECT_EQ (1, IntTraits::deletions);
}

TEST_F (RefCountedTest, DefaultValue)
{
  {
    RCInt i;
    EXPECT_EQ (IntTraits::def, i.Value ());
  }
  EXPECT_EQ (0, IntTraits::deletions);
}

TEST_F (RefCountedTest, CopyConstructor)
{
  {
    RCInt i (5);
    {
      RCInt j (i);
      EXPECT_EQ (i.Value (), j.Value ());
      EXPECT_EQ (0, IntTraits::deletions);
    }
    EXPECT_EQ (0, IntTraits::deletions);
  }
  EXPECT_EQ (1, IntTraits::deletions);
}

TEST_F (RefCountedTest, Assignment)
{
  {
    RCInt i (3);
    RCInt j (4);
    i = j;
    EXPECT_EQ (i.Value (), j.Value ());
    EXPECT_EQ (1, IntTraits::deletions);
    RCInt k (3);
    k = i;
    EXPECT_EQ (2, IntTraits::deletions);
    for (unsigned int n = 0; n < 10; ++n)
      {
        {
          RCInt m (k);
          RCInt l (m);
        }
        EXPECT_EQ (2, IntTraits::deletions);
      }
    EXPECT_EQ (2, IntTraits::deletions);
  }
  EXPECT_EQ (3, IntTraits::deletions);
}

TEST_F (RefCountedTest, SelfAssignment)
{
  {
    RCInt i (3);
    i = *&i;  // annotate with *& to tell clang we intend self-assignment
    EXPECT_EQ (0, IntTraits::deletions);
    i = *&i = *&i;  // annotate with *& to tell clang we intend self-assignment
    EXPECT_EQ (0, IntTraits::deletions);
  }
  EXPECT_EQ (1, IntTraits::deletions);
}

TEST_F (RefCountedTest, Equality)
{
  RCInt i (3);
  RCInt j (i);
  RCInt k (3);
  EXPECT_EQ (i, j);
  EXPECT_EQ (i, k);
  EXPECT_EQ (j, k);
  k = 8;
  EXPECT_NE (i, k);
  EXPECT_NE (j, k);
}

}  // namespace
