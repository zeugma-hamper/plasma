
/* (c)  oblong industries */

#include <libPlasma/c++/Pool.h>
#include <libLoam/c++/ObRetort.h>

#include <libPlasma/c/pool_options.h>
#include <libPlasma/c/pool.h>
#include <libPlasma/c/slaw.h>

#include <gtest/gtest.h>

#include "libLoam/c/ob-sys.h"
#include "libLoam/c/ob-util.h"

using namespace oblong::plasma;

namespace {

class PoolTestBase : public ::testing::Test
{
 public:
  static ::std::string PoolName (const char *suffix, const char *sp = "-")
  {
    return BASE_NAME + sp + suffix;
  }

 protected:
  static const ::std::string BASE_NAME;
  static const ::std::string POOL_NAME;
  static const ::std::string EPOOL_NAME;
  static const char *ENAME, *PNAME;

  // static Pool existing_pool_;

  static void SetUpTestCase ()
  {
    if (getenv ("OB_POOLS_DIR_FOR_SPANNING_TESTS"))
      {
        EXPECT_EQ (OB_OK,
                   ob_setenv ("OB_POOLS_DIR",
                              getenv ("OB_POOLS_DIR_FOR_SPANNING_TESTS")));
      }

    protein p (small_mmap_pool_options ());
    ObRetort ret (pool_create (PNAME, "mmap", p));
    slaw_free (p);
    EXPECT_EQ (OB_OK, ret.Code ()) << ret;
    ret = Pool::Create (ENAME, Pool::MMAP_SMALL, false);
    EXPECT_EQ (OB_OK, ret.Code ()) << ret;
  }

  static void TearDownTestCase ()
  {
    pool_dispose (ENAME);
    pool_dispose (PNAME);
  }
};

const ::std::string PoolTestBase::BASE_NAME (getenv ("TEST_POOL")
                                               ? getenv ("TEST_POOL")
                                               : "test-pool");
const ::std::string PoolTestBase::POOL_NAME (PoolName ("first-pool"));
const ::std::string PoolTestBase::EPOOL_NAME (PoolName ("second-pool"));
const char *PoolTestBase::ENAME = EPOOL_NAME.c_str ();
const char *PoolTestBase::PNAME = POOL_NAME.c_str ();


}  // namespace
