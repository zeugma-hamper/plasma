
/* (c)  oblong industries */

#ifndef BASEMENT_STR_REGEXP_MATCH_H
#define BASEMENT_STR_REGEXP_MATCH_H


#include <unicode/unistr.h>
#include <unicode/regex.h>

#include <libLoam/c++/Str.h>


namespace oblong {
namespace loam {


using icu::RegexMatcher;


class StrMatchData
{
 OB_PRIVATE:
  Str *str;
  const UnicodeString *pattern_ustr;
  RegexMatcher *matcher;
  bool matched_state;

  UErrorCode error_state;
  Str *error_str;

 public:
  StrMatchData ();
  StrMatchData (Str *str_backpointer, const Str &pattern);

  // "copy constructor" of a sort, for use by Str
  StrMatchData (Str *str_backpointer, const StrMatchData &other);

  ~StrMatchData ();
  void Delete () { delete this; }

  StrMatchData &Match ();

  bool IsTrue () const;
  operator bool () const;

  bool IsInError () const;
  Str ErrorStr () const;

  Str MatchSlice () const;
  Str PreMatchSlice () const;
  Str PostMatchSlice () const;

  int NumCaptures () const;
  Str NthCaptureSlice (int which_capture) const;
  StrRange NthCaptureRange (int which_capture) const;

  void ReplaceAll (const Str &replacement);
  void ReplaceFirst (const Str &replacement);

  ObTrove<Str> Split ();

  void SetBackPointer (Str *str_backpointer);

 OB_PRIVATE:
  void Init ();
  void Init (Str *str_backpointer, const Str &pattern);

  typedef UnicodeString (RegexMatcher::*usRepFunc) (
    const UnicodeString &replacement, UErrorCode &status);

  void Replace (const Str &replacement, usRepFunc func);

  // operator= unimplemented
  StrMatchData &operator= (const StrMatchData &other);
  // copy constructor, unimplemented
  StrMatchData (const StrMatchData &other);
};
}
}  // "et tu?", say namespaces loam and oblong


#endif
