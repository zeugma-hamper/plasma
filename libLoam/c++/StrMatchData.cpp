
/* (c)  oblong industries */

#include <libLoam/c++/StrMatchData.h>
#include <libLoam/c++/Str.h>

#include <libLoam/c++/Preterite.h>

#include <stdio.h>
#include <assert.h>


using namespace oblong::loam;


static const UChar32 DUMMY_CHAR = 0x10FFFF;  // a noncharacter
static const UnicodeString DUMMY_CHAR_USTR (DUMMY_CHAR);
static const int32 DUMMY_CHAR_LEN = DUMMY_CHAR_USTR.length ();

static void ChompDummy (UnicodeString &u)
{
  if (u.endsWith (DUMMY_CHAR_USTR))
    u.truncate (u.length () - DUMMY_CHAR_LEN);
}


StrMatchData::StrMatchData ()
{
  Init ();
}

StrMatchData::StrMatchData (Str *str_backpointer, const Str &pattern)
{
  Init ();
  Init (str_backpointer, pattern);
}

// you know, two Init() methods, because that's an entertaining way to
// name things. The empty-signatured-one just gives us a nice clean
// slate. The double-str-reffed one will make us all our internal
// machinery. You can call one or both.
//
void StrMatchData::Init ()
{
  str = NULL;
  pattern_ustr = NULL;
  matcher = NULL;
  matched_state = false;
  error_state = U_ZERO_ERROR;
  error_str = NULL;
}
//
//
void StrMatchData::Init (Str *str_backpointer, const Str &pattern)
{
  str = str_backpointer;
  pattern_ustr = new UnicodeString (pattern.ICUUnicodeString ());

  matcher = new RegexMatcher (*pattern_ustr, 0, error_state);
  if (U_FAILURE (error_state))
    {
      Del_Ptr (pattern_ustr);
      Del_Ptr (matcher);
      error_str = new Str (u_errorName (error_state));
      return;
    }

  matcher->reset (str->ICUUnicodeString ());
}

StrMatchData::~StrMatchData ()
{
  Del_Ptr (pattern_ustr);
  Del_Ptr (matcher);
  Del_Ptr (error_str);
}

StrMatchData::StrMatchData (Str *str_backpointer, const StrMatchData &other)
{
  Init ();
  // Hey, other.pattern_ustr can be NULL, and it's a little bit sketchy to
  // dereference it in that case.  (Even though in practice it's not an
  // immediate crash, since the dereference is being passed as a reference
  // argument, so behind the scenes it's staying a pointer.)
  Init (str_backpointer,
        other.pattern_ustr ? Str (*other.pattern_ustr) : Str (""));
  int32_t start;

  if (!other.matcher)
    {
      // The other guy's matcher and pattern_ustr are NULL, so we should make
      // sure ours are, too.  (Init set ours to non-NULL; since the other
      // guy's pattern_ustr is NULL, that means we won't get the same error
      // the other guy did, so ironically our matcher won't be NULL.)
      Del_Ptr (pattern_ustr);
      Del_Ptr (matcher);
      goto dont_forget_you_still_need_to_copy_the_error_state;
    }

  start = other.matcher->start (error_state);
  error_state = U_ZERO_ERROR;

  if (start == -1)
    {
      // we weren't able to get a start position. this, as far as we
      // can tell, means the other matcher is in a match-failed
      // state. the only way we've been able to duplicate this is to
      // Match() and MatchAgain() until we are, too. We'll try to make
      // this faster by Reset()ing first.
      matcher->reset (str_backpointer->Length () - 1, error_state);
      while (matcher->find ())
        {
        }
      matched_state = other.matched_state;
    }
  else
    {
      matcher->reset (start, error_state);
      if (start || other.matched_state)
        {
          matched_state = matcher->find ();
          // we think we've perhaps covered the bug cases here, but
          // the internals of RegexMatch are a little beyond our
          // ken. So let's leave this warning/assert in place.
          if (matched_state != other.matched_state)
            {
              OB_FATAL_BUG_CODE (0x11060000,
                                 "bug in StrMatchData copy conceit\n"
                                 "start = %d\n"
                                 "matched_state = %d, other.matched_state = "
                                 "%d\n"
                                 "check out bug 949\n",
                                 start, matched_state, other.matched_state);
            }
        }
    }

dont_forget_you_still_need_to_copy_the_error_state:
  error_state = other.error_state;
  if (other.error_str)
    error_str = new Str (*other.error_str);
}


StrMatchData::operator bool () const
{
  return IsTrue ();
}

bool StrMatchData::IsTrue () const
{
  return (!U_FAILURE (error_state) && matched_state);
}

bool StrMatchData::IsInError () const
{
  return (U_FAILURE (error_state));
}

Str StrMatchData::ErrorStr () const
{
  if (!error_str)
    return Str ();
  return Str (*error_str);
}

StrMatchData &StrMatchData::Match ()
{
  if (matcher)
    matched_state = matcher->find ();
  return *this;
}


Str StrMatchData::MatchSlice () const
{
  return NthCaptureSlice (0);
}

Str StrMatchData::PreMatchSlice () const
{
  if (!(matcher && matched_state) || U_FAILURE (error_state))
    return Str ();

  UErrorCode local_error = U_ZERO_ERROR;
  int32_t start = matcher->start (local_error);
  if (U_FAILURE (local_error))
    return Str ();
  return Str (str->ICUUnicodeString (), 0, start);
}

Str StrMatchData::PostMatchSlice () const
{
  if (!(matcher && matched_state) || U_FAILURE (error_state))
    return Str ();

  UErrorCode local_error = U_ZERO_ERROR;
  int32_t end = matcher->end (local_error);
  if (U_FAILURE (local_error))
    return Str ();
  UnicodeString ustr = str->ICUUnicodeString ();
  return Str (ustr, end, ustr.length () - end);
}

int StrMatchData::NumCaptures () const
{
  if (!(matcher && matched_state) || U_FAILURE (error_state))
    return 0;
  return matcher->groupCount ();
}

Str StrMatchData::NthCaptureSlice (int which_capture) const
{
  if (!(matcher && matched_state) || U_FAILURE (error_state))
    return Str ();

  UErrorCode local_error = U_ZERO_ERROR;
  UnicodeString slice = matcher->group (which_capture, local_error);
  if (U_FAILURE (local_error))
    return Str ();
  return Str (slice);
}

StrRange StrMatchData::NthCaptureRange (int which_capture) const
{
  StrRange ret;
  ret.idx = -1;
  ret.len = -1;

  if (!(matcher && matched_state) || U_FAILURE (error_state))
    return ret;

  UErrorCode local_error = U_ZERO_ERROR;
  int32 start = matcher->start (which_capture, local_error);
  int32 end = matcher->end (which_capture, local_error);
  if (!U_FAILURE (local_error) && start >= 0 && end >= start)
    {
      UnicodeString ustr = str->ICUUnicodeString ();
      ret.idx = ustr.countChar32 (0, start);
      ret.len = ustr.countChar32 (start, end - start);
    }
  return ret;
}

void StrMatchData::ReplaceAll (const Str &replacement)
{
  Replace (replacement, &RegexMatcher::replaceAll);
}

void StrMatchData::ReplaceFirst (const Str &replacement)
{
  Replace (replacement, &RegexMatcher::replaceFirst);
}

void StrMatchData::Replace (const Str &replacement, usRepFunc func)
{
  if (!matcher)
    return;
  str->Set ((matcher->*func) (replacement.ICUUnicodeString (), error_state));
  // matched_state = false;
}

str_array StrMatchData::Split ()
{
  if (!matcher)
    return str_array ();
  // count the occurrences of our separator pattern, starting the
  // count at '1' as any non-empty string will at least split into
  // itself
  unt64 count = 1;
  while (matcher->find ())
    ++count;
  // multiply our count by the number of capture groups (plus one to
  // account for the words themselves) to get an (over-) approximation
  // of the number of slots we'll need
  count *= matcher->groupCount () + 1;

  matcher->reset ();

  UnicodeString *words = new UnicodeString[count];
  UnicodeString &u = const_cast<UnicodeString &> (str->ICUUnicodeString ());

  /* So the reason we append a dummy character is so that the last field
   * will never be empty.  This lets us defeat the "feature" of some versions
   * of ICU, which will delete an empty trailing field. */
  const int32 save_len_1 = u.length ();
  u.append (DUMMY_CHAR);

  /* The reason for appending a second dummy character is more obscure.  It's
   * because the version of ICU I'm using appears to read beyond the end of
   * the string, and causes a valgrind error for using uninitialized data.
   * So, the point of the second dummy character is to initialize the data
   * immediately after the string. */
  const int32 save_len_2 = u.length ();
  u.append (DUMMY_CHAR);

  /* Since the only point of that second dummy character was to initialize
   * beyond the end of the string, we immediately revert it.  Now the end
   * of the string is between the first and second dummy characters. */
  u.truncate (save_len_2);

  int num_words = matcher->split (u, words, count, error_state);

  /* Now revert the first dummy character, as well.  This ensures the
   * Str is the same when we leave this method as when we entered it.
   * Therefore, the Str is logically const, even though we've been doing
   * evil things with it behind the user's back.  Since Str is not
   * guaranteed to be threadsafe, it doesn't matter that it's had a
   * different value while this method's been executing. */
  u.truncate (save_len_1);

  if (U_FAILURE (error_state))
    return str_array ();

  if (num_words > 0)
    ChompDummy (words[num_words - 1]);

  str_array trove = str_array ();
  trove.EnsureRoomFor (num_words);
  for (int q = 0; q < num_words; ++q)
    trove.Append (words[q]);

  delete[] words;
  return trove;
}

void StrMatchData::SetBackPointer (Str *str_backpointer)
{
  str = str_backpointer;
}
