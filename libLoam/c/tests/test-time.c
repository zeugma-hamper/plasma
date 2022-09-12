
/* (c)  oblong industries */

// test ob_strptime () && ob_format_time_f

#include "libLoam/c/ob-sys.h"
#include "libLoam/c/ob-time.h"
#include "libLoam/c/ob-util.h"
#include "libLoam/c/ob-log.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

/* oddly, ob_format_time outputs a single trailing space. */
static const char beg_of_time_s[] = "Jan 1, 1970 00:00:00.00 ";
static float64 beg_of_time_sec = 0.0;
struct timeval beg_of_time_tv (void)
{
  struct timeval tv;
  tv.tv_sec = 0;
  tv.tv_usec = 0;
  return tv;
}

static const char rand_test_time_s[] = "Oct 31, 2016 16:20:42.09 ";
static float64 rand_test_time_sec = 1477930842.099525;
struct timeval rand_test_time_tv (void)
{
  struct timeval tv;
  tv.tv_sec = 1477930842;
  tv.tv_usec = 99525;
  return tv;
}

static void test_ob_format_time (void)
{
  char buf[80];
  size_t expected_len = 24;

  /* Test the beginning of time */
  struct timeval tv = beg_of_time_tv ();
  memset (buf, 1, sizeof (buf));
  ob_format_time (buf, sizeof (buf), &tv);
  if (strncmp (buf, beg_of_time_s, sizeof (beg_of_time_s)))
    error_exit ("test_ob_format_time (beginning of time): '%s' != '%s'\n", buf,
                beg_of_time_s);
  if (strlen (buf) != expected_len)
    error_exit ("test_ob_format_time (beginning of time): strlen('%s') != %zu",
                buf, expected_len);

  /* Test a specific non-zero time */
  expected_len = 25;
  tv = rand_test_time_tv ();
  memset (buf, 1, sizeof (buf));
  ob_format_time (buf, sizeof (buf), &tv);
  if (strncmp (buf, rand_test_time_s, sizeof (rand_test_time_s)))
    error_exit ("test_ob_format_time (non-zero of time): '%s' != '%s'\n", buf,
                rand_test_time_s);
  if (strlen (buf) != expected_len)
    error_exit ("test_ob_format_time (non-zero of time): strlen('%s') != %zu",
                buf, expected_len);

  /* Test a buffer JUST big enough to fit strftime + ms ("%02d ") plus one */
  memset (buf, 1, sizeof (buf));
  ob_format_time (buf, expected_len + 2, &tv);
  if (strncmp (buf, rand_test_time_s, sizeof (rand_test_time_s)))
    error_exit ("test_ob_format_time (exact buffer size): '%s' != '%s'\n", buf,
                rand_test_time_s);
  if (strlen (buf) != expected_len)
    error_exit ("test_ob_format_time (exact buffer size): strlen('%s') != %zu",
                buf, expected_len);

#if 0
  /* FIXME: ob_format_time behavior for short buffers needs to be
   * better defined before we can test it
   */

  /* Test a buffer big enough to fit strftime, but not ms ("%02d ") */
  memset(buf, 1, sizeof(buf));
  ob_format_time (buf, expected_len, &tv);
  if (strncmp(buf, rand_test_time_s, sizeof(rand_test_time_s)))
    error_exit ("test_ob_format_time (mid buffer size): '%s' != '%s'\n",
                buf, rand_test_time_s);
  if (strlen(buf) != expected_len)
    error_exit ("test_ob_format_time (mid buffer size): strlen('%s') != %zu",
                buf, expected_len);

  /* Test a buffer insufficient to even fit strftime */
  memset(buf, 1, sizeof(buf));
  ob_format_time (buf, expected_len - 4, &tv);
  if (buf != NULL)
    error_exit ("test_ob_format_time (insufficient buffer): '%s' != '%s'\n",
                buf, rand_test_time_s);
  if (strlen(buf) != expected_len)
    error_exit ("test_ob_format_time (insufficient buffer): strlen('%s') != %zu",
                buf, expected_len);
#endif
}

static void test_ob_format_time_f (void)
{
  char buf[80];
  size_t expected_len = 24;

  /* Test the beginning of time */
  memset (buf, 1, sizeof (buf));
  ob_format_time_f (buf, sizeof (buf), beg_of_time_sec);
  if (strncmp (buf, beg_of_time_s, sizeof (beg_of_time_s)))
    error_exit ("test_ob_format_time_f (beginning of time): '%s' != '%s'\n",
                buf, beg_of_time_s);
  if (strlen (buf) != expected_len)
    error_exit (
      "test_ob_format_time_f (beginning of time): strlen('%s') != %zu", buf,
      expected_len);

  /* Test a specific non-zero time */
  expected_len = 25;
  memset (buf, 1, sizeof (buf));
  ob_format_time_f (buf, sizeof (buf), rand_test_time_sec);
  if (strncmp (buf, rand_test_time_s, sizeof (rand_test_time_s)))
    error_exit ("test_ob_format_time_f (non-zero of time): '%s' != '%s'\n", buf,
                rand_test_time_s);
  if (strlen (buf) != expected_len)
    error_exit ("test_ob_format_time_f (non-zero of time): strlen('%s') != %zu",
                buf, expected_len);
}

static void test_ob_strptime (void)
{
  char ctime1[80];
  float64 cur_time, echo_time;

  cur_time = ob_current_time ();
  ob_format_time_f (ctime1, sizeof (ctime1), cur_time);
  printf ("test_ob_strptime: cur_time = %f = %s\n", cur_time, ctime1);

  OB_DIE_ON_ERROR (ob_strptime (ctime1, &echo_time));

  if ((cur_time - echo_time) > 0.01)
    error_exit ("test_ob_strptime: %f != %f\n", cur_time, echo_time);
}

int main (int argc, char **argv)
{
  /* Set timezone for reproducibility */
  ob_setenv ("TZ", "UTC");
  tzset ();

  test_ob_format_time ();
  test_ob_format_time_f ();
  test_ob_strptime ();

  return EXIT_SUCCESS;
}
