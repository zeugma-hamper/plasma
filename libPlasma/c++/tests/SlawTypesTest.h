
/* (c)  oblong industries */

#include "Callbacks.h"

#include <gtest/gtest.h>

class SlawTypesTest : public ::testing::Test
{
 public:
  void SetUp () override {}
  void TearDown () override {}
  static void SetUpTestCase () {}
  static void TearDownTestCase () {}

  virtual void wrapped_slaw_test (const Callbacks &);

  void TestBoolean ();
  void TestStrings ();
  void TestNumeric ();
  void TestVectorClasses ();
  void TestVect ();
  void TestVect4 ();
  void TestQuat ();
  void TestMatrix44 ();
};

#define DEFINE_SLAW_TESTS_FOR_CLASS(CLASS_NAME)                                \
  TEST_F (CLASS_NAME, Strings) { TestStrings (); }                             \
  TEST_F (CLASS_NAME, Boolean) { TestBoolean (); }                             \
  TEST_F (CLASS_NAME, Numeric) { TestNumeric (); }                             \
  TEST_F (CLASS_NAME, VectorClasses) { TestVectorClasses (); }                 \
  TEST_F (CLASS_NAME, Vect) { TestVect (); }                                   \
  TEST_F (CLASS_NAME, Vect4) { TestVect4 (); }                                 \
  TEST_F (CLASS_NAME, Quat) { TestQuat (); }
