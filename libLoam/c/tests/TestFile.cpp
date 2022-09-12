
/* (c)  oblong industries */

#include <gtest/gtest.h>
#include "libLoam/c/ob-file.h"
#include "libLoam/c/ob-log.h"
#include "libLoam/c/ob-string.h"

class TestFile : public ::testing::Test
{
 public:
  unt8 *buf;
  unt64 len;

  static void SetUpTestCase ()
  {
    const char *srcdir = ABS_TOP_SRCDIR "/libLoam/c/tests";
    if (srcdir)
      CHECK_POSIX_ERROR (chdir (srcdir));
    CHECK_POSIX_ERROR (chdir ("sniff"));
    OB_DIE_ON_ERROR (ob_suppress_message (OBLV_ERROR, 0x10020005));
  }
};

TEST_F (TestFile, TextNonexistent)
{
  EXPECT_TRUE (NULL == ob_read_file ("does/not/exist"));
}

TEST_F (TestFile, TextZero)
{
  char *s = ob_read_file ("dir1/111");
  EXPECT_EQ (size_t (0), strlen (s));
  free (s);
}

TEST_F (TestFile, TextString)
{
  char *s = ob_read_file ("dir2/923");
  ob_chomp (s);
  EXPECT_STREQ ("East 3rd Street", s);
  free (s);
}

TEST_F (TestFile, TextDirectory)
{
  EXPECT_TRUE (NULL == ob_read_file ("dir1/923"));
}

TEST_F (TestFile, BinaryNonexistent)
{
  EXPECT_EQ (ob_errno_to_retort (ENOENT),
             ob_read_binary_file ("does/not/exist", &buf, &len));
}

TEST_F (TestFile, BinaryZero)
{
  EXPECT_EQ (OB_OK, ob_read_binary_file ("dir1/111", &buf, &len));
  EXPECT_EQ (OB_CONST_U64 (0), len);
  free (buf);
}

TEST_F (TestFile, BinaryData)
{
  EXPECT_EQ (OB_OK, ob_read_binary_file ("dir2/hello", &buf, &len));
  EXPECT_EQ (OB_CONST_U64 (633), len);
  // compute 64-bit FNV hash
  unt64 h = OB_CONST_U64 (14695981039346656037);
  for (unt64 i = 0; i < len; i++)
    h = (h * OB_CONST_U64 (1099511628211)) ^ buf[i];
  EXPECT_EQ (OB_CONST_U64 (4676634622844942640), h);
  free (buf);
}

TEST_F (TestFile, BinaryDirectory)
{
  /* Local UNIX filesystems say EISDIR, NFS says EINVAL,
   * and Windows says EACCES, so we need to accept all three. */
  ob_retort tort = ob_read_binary_file ("dir1/923", &buf, &len);
  EXPECT_TRUE (tort == ob_errno_to_retort (EISDIR)
               || tort == ob_errno_to_retort (EINVAL)
               || tort == ob_errno_to_retort (EACCES))
    << ob_error_string (tort);
}

TEST_F (TestFile, RealPath)
{
  char *real = NULL;
  ASSERT_EQ (OB_OK, ob_realpath ("dir2/hello", &real));
  ASSERT_TRUE (real != NULL);
  EXPECT_EQ (OB_OK, ob_read_binary_file (real, &buf, &len));
  EXPECT_EQ (OB_CONST_U64 (633), len);
  // compute 64-bit FNV hash
  unt64 h = OB_CONST_U64 (14695981039346656037);
  for (unt64 i = 0; i < len; i++)
    h = (h * OB_CONST_U64 (1099511628211)) ^ buf[i];
  EXPECT_EQ (OB_CONST_U64 (4676634622844942640), h);
  free (buf);
  free (real);
}

TEST_F (TestFile, Mkstemp)
{
  int fd = -1;
  char *name = NULL;
  ASSERT_EQ (OB_OK, ob_mkstemp ("TestFile.Mkstemp.", &name, &fd, false));
  EXPECT_EQ (12, write (fd, "hello\nworld\n", 12));
  EXPECT_EQ (0, close (fd));
  FILE *f = fopen (name, "r");
  ASSERT_TRUE (f != NULL);
  char mybuf[20];
  EXPECT_TRUE (NULL != fgets (mybuf, sizeof (mybuf), f));
  EXPECT_STREQ ("hello\n", mybuf);
  EXPECT_TRUE (NULL != fgets (mybuf, sizeof (mybuf), f));
  EXPECT_STREQ ("world\n", mybuf);
  EXPECT_EQ (0, fclose (f));
  info_report ("filename is '%s'\n", name);
  EXPECT_EQ (0, unlink (name));
  free (name);
}
