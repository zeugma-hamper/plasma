
/* (c)  oblong industries */

#include "libLoam/c/ob-string.h"
#include <string.h>

size_t ob_safe_copy_string (char *buf, size_t capacity, const char *str)
{
  char *orig = buf;
  while (capacity > 1 && *str)
    capacity--, *buf++ = *str++;
  *buf = 0;
  return (buf - orig);
}

size_t ob_safe_append_string (char *buf, size_t capacity, const char *str)
{
  char *orig = buf;
  while (capacity > 1 && *buf)
    capacity--, buf++;
  while (capacity > 1 && *str)
    capacity--, *buf++ = *str++;
  *buf = 0;
  return (buf - orig);
}

size_t ob_safe_append_int64 (char *buf, size_t capacity, int64 n)
{
  char stuff[32];
  char *p = stuff + sizeof (stuff);
  bool neg = false;
  if (n < 0)
    {
      neg = true;
      n = -n;
    }
  *--p = 0;
  do
    {
      int64 rem = n % 10;
      n /= 10;
      if (p > stuff)
        *--p = '0' + (char) rem;
    }
  while (n > 0);
  if (neg && p > stuff)
    *--p = '-';
  return ob_safe_append_string (buf, capacity, p);
}

size_t ob_safe_append_float64 (char *buf, size_t capacity, float64 f,
                               int digits_after_decimal)
{
  char stuff[32];
  char *p = stuff + sizeof (stuff);
  bool neg = false;
  int i;
  for (i = 0; i < digits_after_decimal; i++)
    f *= 10;
  int64 n = (int64) f;
  if (n < 0)
    {
      neg = true;
      n = -n;
    }
  *--p = 0;
  i = 0;
  do
    {
      int64 rem = n % 10;
      n /= 10;
      if (i == digits_after_decimal && p > stuff)
        *--p = '.';
      if (p > stuff)
        *--p = '0' + (char) rem;
    }
  while (i++ < digits_after_decimal || n > 0);
  if (neg && p > stuff)
    *--p = '-';
  return ob_safe_append_string (buf, capacity, p);
}

void ob_chomp (char *s)
{
  size_t len;
  if (s && (len = strlen (s)) > 0 && s[len - 1] == '\n')
    {
      s[--len] = 0;
      if (len > 0 && s[len - 1] == '\r')
        s[--len] = 0;
    }
}

/* Needed glob pattern matching to implement bug 1021.
 * (And since it has to be done in C, bug 1008 won't help me.)
 *
 * fnmatch() would do the trick on Linux and OS X, but there is
 * no fnmatch() on Windows.
 *
 * (Yeah, there's PathMatchSpec() on Windows, but it almost
 * certainly behaves differently than fnmatch() in some cases,
 * so I'd rather have a single cross-platform implementation.)
 *
 * glib has glob-matching functions, but we're not allowed to use
 * glib, so I have to implement my own globbing function here in
 * libLoam/c, which is basically turning into our own clone of
 * half of glib's functionality.
 *
 * However, rather than writing a globbing function myself, and
 * all the testing and debugging that entails, I stole one from
 * here:
 *
 * http://www.whoow.org/public_domain/glob.html
 *
 * Since this is truly in the public domain, rather than an
 * open-source license, there should be no licensing issue at
 * all with including it directly in libLoam.  And since it's
 * very nice, short, and self-contained, there shouldn't be any
 * concern about code bloat or bringing more functionality than
 * we need.  So I can't see anyone objecting to this any more than
 * if I just wrote it myself, except that I didn't have to write it.
 *
 * So, a big thank-you to Ozan S. Yigit, whoever he is!
 *
 * The only downside is that this only supports ASCII, and not UTF-8.
 * But we'll live with that for now.
 *
 * I've done some light editing, reformatting it to Oblong
 * indentation standards, and Oblong types.
 *
 * Also, documented a case fall-thru, as discussed in bug 1045.
 */

/*
 * robust glob pattern matcher
 * ozan s. yigit/dec 1994
 * public domain
 *
 * glob patterns:
 *      *       matches zero or more characters
 *      ?       matches any single character
 *      [set]   matches any character in the set
 *      [^set]  matches any character NOT in the set
 *              where a set is a group of characters or ranges. a range
 *              is written as two characters seperated with a hyphen: a-z denotes
 *              all characters between a to z inclusive.
 *      [-set]  set matches a literal hypen and any character in the set
 *      []set]  matches a literal close bracket and any character in the set
 *
 *      char    matches itself except where char is '*' or '?' or '['
 *      \char   matches char, including any pattern character
 *
 * examples:
 *      a*c             ac abc abbc ...
 *      a?c             acc abc aXc ...
 *      a[a-z]c         aac abc acc ...
 *      a[-a-z]c        a-c aac abc ...
 *
 * $Log: glob.c,v $
 * Revision 1.3  1995/09/14  23:24:23  oz
 * removed boring test/main code.
 *
 * Revision 1.2  94/12/11  10:38:15  oz
 * cset code fixed. it is now robust and interprets all
 * variations of cset [i think] correctly, including [z-a] etc.
 *
 * Revision 1.1  94/12/08  12:45:23  oz
 * Initial revision
 */

#ifndef NEGATE
#define NEGATE '^' /* std cset negation char */
#endif

bool ob_match_glob (const char *str, const char *p)
{
  bool negate;
  bool match;
  int c;

  while (*p)
    {
      if (!*str && *p != '*')
        return false;

      switch (c = *p++)
        {
          case '*':
            while (*p == '*')
              p++;

            if (!*p)
              return true;

            if (*p != '?' && *p != '[' && *p != '\\')
              while (*str && *p != *str)
                str++;

            while (*str)
              {
                if (ob_match_glob (str, p))
                  return true;
                str++;
              }
            return false;

          case '?':
            if (*str)
              break;
            return false;
          /*
 * set specification is inclusive, that is [a-z] is a, z and
 * everything in between. this means [z-a] may be interpreted
 * as a set that contains z, a and nothing in between.
 */
          case '[':
            if (*p != NEGATE)
              negate = false;
            else
              {
                negate = true;
                p++;
              }

            match = false;

            while (!match && (c = *p++))
              {
                if (!*p)
                  return false;
                if (*p == '-') /* c-c */
                  {
                    if (!*++p)
                      return false;
                    if (*p != ']')
                      {
                        if (*str == c || *str == *p || (*str > c && *str < *p))
                          match = true;
                      }
                    else /* c-] */
                      {
                        if (*str >= c)
                          match = true;
                        break;
                      }
                  }
                else /* cc or c] */
                  {
                    if (c == *str)
                      match = true;
                    if (*p != ']')
                      {
                        if (*p == *str)
                          match = true;
                      }
                    else
                      break;
                  }
              }

            if (negate == match)
              return false;

            /* p has already been incremented, so if c is NUL,
           * we can't look at *p without being out of bounds.
           * Therefore, check if c is NUL here.
           * (But, note that p wasn't incremented if match
           * was true, so we have to check for that, too.) */
            if (!match && !c)
              return false;

            /*
 * if there is a match, skip past the cset and continue on
 */
            while (*p && *p != ']')
              p++;
            if (!*p++) /* oops! */
              return false;
            break;

          case '\\':
            if (*p)
              c = *p++;
          /* Since "default" is the case for characters that are not special,
           * and "\" is the case for an escape sequence, it makes sense that
           * "\" just moves to the next character and then falls through
           * to the case for non-special characters.  So, in other words,
           * this fall-thru is intentional, although naughty, naughty
           * Mr. Yigit didn't document it.
           *
           * One other thing worth noting here is that if the pattern
           * ends in "\", then it will effectively be treated as "\\".
           * That's probably fine, since I would consider a pattern ending
           * in the middle of an escape sequence to be illegal, so as
           * long as it doesn't crash or do something horrible, any
           * behavior is fine.
           *
           * The following comment tells gcc to suppress the warning (!):
           */
          /* fall through */
          default:
            if (c != *str)
              return false;
            break;
        }
      str++;
    }

  return !*str;
}
