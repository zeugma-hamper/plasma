
/* (c)  oblong industries */

#ifndef BASEMENT_STR_H
#define BASEMENT_STR_H


#include <libLoam/c/ob-attrs.h>
#include <libLoam/c/ob-types.h>
#include <libLoam/c/ob-api.h>

#include <string.h>

#include <unicode/uversion.h>

#include <libLoam/c++/ObTrove.h>
#include <libLoam/c++/StrIterator.h>


#define OB_NEWLINE_SEP "\n"

// forward-delcare UnicodeString, so we don't need <unicode/unistr.h> here
U_NAMESPACE_BEGIN
class UnicodeString;
U_NAMESPACE_END



namespace oblong {
namespace loam {

using icu::UnicodeString;

typedef int64 str_marker;
typedef ObTrove<Str> str_array;
class StrMatchData;

struct StrRange
{
  int64 idx;
  int64 len;
};


/**
 * A utility class to manage sequences of unicode characters. The API
 * supports values semantics, regular expressions, and (some, both
 * psychological and actual) compatibility with basic char pointers.
 *
 * The Str API is modeled on the string handling support provided in
 * perl and ruby, with method names and regular expression match
 * accessors leaning towards the ruby-ish.
 *
 * Putting NULL bytes in the middle of a Str is discouraged. The API
 * assumes that you're storing unicode text, and (at least some)
 * undefined behavior will result. (This wishy-washiness is on our
 * list of things to fix.)
 *
 * No methods are virtual; Str is not intended to be subclassed. The
 * API also makes no attempt to define thread safety.
 *
 * The Str class depends heavily on the ICU library for internal
 * unicode support:  http://site.icu-project.org/
 *
 * Users of the Str API need not worry about ICU unless (a) they are
 * interested in compatibility with other APIs using ICU (b) they
 * have relatively unusual and complex unicode processing needs, such
 * as conversion between character sets, or (c) they have specific
 * algorithm or performance constructs in mind.
 *
 * Regular expression patterns are quite perl compatible, with a few
 * unicode extensions and c++-isms.
 *
 *    http://userguide.icu-project.org/strings/regexp
 *
 * Here again, users of Str are insulated from the underlying ICU
 * mechanisms. Examples of simple use:
 *
 * \code
 *     str = "foo bar bash";
 *
 *     str.Match ("(\\w+) (\\w+) (\\w+)");   // true
 *     str.MatchNthCaptureSlice (1);         // "foo"
 *     str.MatchNthCaptureSlice (2);         // "bar"
 *     str.MatchNthCaptureSlice (3);         // "bash"
 *
 *     str.Match ("(\\w+)");                 // true
 *     str.MatchSlice ();                    // "foo"
 *     str.MatchAgain ();                    // true
 *     str.MatchAgain ();                    // true
 *     str.MatchAgain ();                    // false
 *     str.MatchHasMatched ();               // false
 *
 *     str.Match ("(\\w+");                  // false
 *     str.MatchHasErred ();                 // true
 *     str.MatchErrorStr ();                 // "U_REGEX_MISMATCHED_PAREN"
 * \endcode
 *
 * Two kinds of iteration scaffolding are supported. Minimal
 * iterators modeled on those provided by the STL, and a set of
 * lower-level building blocks with which it is possible to write
 * equivalent loops. This iteration support is worth using in
 * production code, as stateful traversal of a Str containing
 * multi-byte characters can be faster than simply looping and using
 * operator[].
 *
 * \code
 *   str = "foo";
 *
 *   StrIterator i = str.begin (), iend = str.end ();
 *   while (i < iend)
 *     printf ("%c", *i++);     // 'f' then 'o' then 'o'
 * \endcode
 *
 * And.
 *
 * \code
 *   str = "foo";
 *   str_marker m = str.begin_marker ();
 *   while (m < str.end_bounds_marker ())
 *     printf ("%c", str.FetchAndMoveMarker (m, 1);  // 'f' then 'o' then 'o'
 * \endcode
 *
 * If you need to deal with strings that are coming at you from
 * library code, you'll probably need to dig down into the various ICU
 * conversion functions. For example, a wchar_t array holding onto
 * some UTF16-encoded bytes might be handled like so:
 *
 * \code
 *   #include "unicode/ustring.h"
 *
 *   wchar_t* string_from_outside = GetMeThatStringPlease_LegacyLibrary ();
 *
 *   UChar uchar_buf[1024];
 *   u_strFromWCS (uchar_name, 1023, NULL, string_from_outside, 512, NULL);
 *
 *   Str loam_str (uchar_buf);
 * \endcode
 *
 * See the ustring.h documentation for more information.
 *
 */
class OB_LOAMXX_API Str
{
 OB_PRIVATE:
  mutable char *u8;
  mutable bool u8IsStale;
  // length without terminating NUL (i. e. u8ByteLength = strlen (u8))
  mutable int64 u8ByteLength;
  // capacity without terminating NUL (so u8 = malloc (u8ByteCapacity + 1))
  mutable int64 u8ByteCapacity;

  mutable UnicodeString *uString;
  mutable bool uStringIsStale;

  mutable int64 codePointLength;

  StrMatchData *match;

 public:
  typedef StrIterator iterator;

  /**
   * maximum legal length of a Str, measured in unicode code
   * points. The size here is governed by various boring
   * implementation details. Exceeding it will produce undefined
   * behavior for a particular Str object (though we hope not for the
   * rest of your code).
   */
  // (Intentionally non-Doxygen comment, since this explanation probably
  // isn't worth putting in the docs.)
  // Here is a guess at what the "boring implementation details" may
  // have been.  (Since they are not boring for someone who needs to
  // understand or maintain the Str class.)  In _FreshenU8(), it calls
  // u_strToUTF8(), which represents the length of the UTF-8 string
  // as a signed 32-bit integer.  Since it takes four bytes of UTF-8
  // to represent the maximum Unicode character, U+10FFFF, then if the
  // entire string was made of such characters, the maximum we could
  // support would be (2 ** 31) / 4 = 536870912.  Or technically, it
  // seems that it would really be ((2 ** 31) - 1) / 4 = 536870911,
  // which suggests this constant is one larger than it should be.
  static const int64 MAX_STR_LENGTH = 536870912;

  /**
   * constructors. The bare constructor Str() is equivalent Str(NULL)
   * and creates an object for which IsNull() and IsEmpty() are both
   * true, but Compare("") is -1.  On the other hand, Str("") creates
   * an object for which IsEmpty() is true and Compare("") is 0 but
   * IsNull() is false.  When a char* is used to create a Str, the
   * data is assumed to be utf8. If no length argument is provided,
   * the char* is also assumed to be null terminated.  When a non-null
   * char* is provided, the Str object makes an internal copy of the
   * memory, meaning that it is still the caller's responsibility to
   * manage the original memory. The wchar_t constructors are only
   * available on Windows.
   */
  //@{
  Str () noexcept;
  Str (std::nullptr_t) noexcept;
  Str (const char *text);
  Str (const char *text, int64 length);
#if defined __linux__ && U_ICU_VERSION_MAJOR_NUM >= 60
  Str (const char16_t *text);
  Str (const char16_t *text, int64 length);
#endif
  Str (const UnicodeString &text);
  Str (const UnicodeString &text, int64 start_marker, int64 length);
  explicit Str (const UChar32 code_point);
#if defined _MSC_VER  //technically could that sizeof wchar_t == 2
  Str (const wchar_t *text);
  Str (const wchar_t *text, int64 length);
#endif
  //@}

  ~Str ();
  void Delete () { delete this; }

  /**
   * copy constructor and assignment operators. A copy of a Str is a
   * copy of all of its data and regular expression match state.
   */
  //@{
  Str (const Str &text);
  Str &operator= (const Str &text);
  Str &operator= (const char *text) { return Set (text); }
  Str &operator= (const UnicodeString &text) { return Set (text); }
  //@}

  /**
   * move constructor and move-assignment operator.
   */
  //@{
  Str (Str &&text) noexcept;
  Str &operator= (Str &&text) noexcept;
  //@}

  /**
   * set the content of the Str. Equivalent to assignment for char*
   * and UnicodeString arguments. From a Str argument, sets the
   * content of our Str but does not copy regular expression match
   * state.
   */
  //@{
  Str &Set (const Str &text);
  Str &Set (const char *text);
  Str &Set (const char *text, int64 length);
  Str &Set (const UnicodeString &text);
  //@}

  /**
   * a variant of Set. Takes a traditional printf format string and
   * arguments list. For example, to create a new Str and sprintf it,
   * in one expression, you could do the following:
   *
   * \code
   *   Str s = Str().Sprintf ("-> %2.2f", 23.45678);
   * \endcode
   *
   * Please note that the format string you pass probably ought to be
   * ascii, as we depend on the underlying c library vasprintf
   * function to generate our output string. And both because of that
   * and because Sprintf takes a variable number of arguments, you'll
   * need to explicitly convert to utf8() Str objects that you pass in
   * to be formatted:
   *
   * \code
   *   Str str1 = "foo"; Str str2 = "bar";
   *   s.Sprintf ("%s -> %s", str1.utf8(), str2.utf8());
   * \endcode
   */
  Str &Sprintf (const char *format, ...) OB_FORMAT (printf, 2, 3);

  /**
   * duplicates a Str, returning a new Str object. Useful to those of
   * us who like to chain simple utility methods, and because most of
   * the Str modifier methods are available only in destructive
   * incarnations. Here's how you might process a line read in from a
   * file, while keeping the original version around for a little longer:
   *
   * \code
   *   Str line     = my_read_line_function ();
   *   Str fixed_up = line.Dup().Strip().Downcase();
   * \endcode
   */
  Str Dup () const;

  /**
   * conversion to null-terminated const char buffer. Here,
   * "conversion" means that you get back a pointer to an array of
   * utf8-encoded characters representing the value of the Str
   * object. This pointer (and its data) will become invalid as soon
   * as any method other than Length() is called on the object. If you
   * want to keep the data around, you'll need to strdup it (or
   * similar). Automatic conversion is provided via 'operator const
   * char*'. Asking for the conversion explicitly, via the utf8()
   * method call, is often good self-documenting style (reminding
   * everyone reading the code that Str's assumption is that the
   * buffer contains utf8-encoded characters). Sadly, the explicit
   * call (or a cast) is required when passing a Str object to printf
   * or any other method that uses varargs.
   */
  operator const char * () const { return utf8 (); }
  const char *utf8 () const;

  /**
   * conversion to an ICU UnicodeString object. As with utf8(), this
   * method returns a reference to an internally managed object which
   * becomes invalid as soon as any method other than Length() is
   * called on the object. If you want to keep the UnicodeString
   * around, copy it.
   */
  const UnicodeString &ICUUnicodeString () const;

  /**
   * the length of the Str in unicode characters (code points)
   */
  int64 Length () const;
  /**
   * the length of the Str in bytes
   */
  int64 ByteLength () const;

  /**
   * single-character access, returns a 32-bit unicode character
   * point. str.At(idx) and str[idx] are equivalent. Negative indexes
   * wrap backwards (so -1 is a request for the next-to-last character
   * in the Str). Out-of-range accesses return 0.
   */
  //@{
  UChar32 At (int64 index) const;
  UChar32 operator[] (int64 index) const { return At (index); }
  UChar32 operator[] (int index) const { return At (index); }
  //@}

  /**
   * substring access, returns a new Str object. Negative indexes wrap
   * backwards; negative (or 0) lengths produce an empty
   * Str(). str.Slice (4,20) and str(4, 20) are equivalent.
   */
  //@{
  Str Slice (int64 index) const;
  Str Slice (int64 index, int64 length) const;
  Str operator() (int64 index, int64 length) const
  {
    return Slice (index, length);
  }
  //@}

  /**
   * concatenation, leaving the object unmodified. Returns a new
   * Str. str.Concat (other_str) and str + other_str are equivalent.
   */
  //@{
  Str Concat (const Str &other) const;
  Str Concat (const UChar32 code_point) const;
  Str operator+ (const Str &other) const { return Concat (other); }
  Str operator+ (const UChar32 code_point) const { return Concat (code_point); }
  //@}

  /**
   * append, returning *this. str.Append (other_str) and str +=
   * other_str are equivalent.
   */
  //@{
  Str &Append (const Str &other);
  Str &Append (const UChar32 code_point);
  Str &operator+= (const Str &other) { return Append (other); }
  Str &operator+= (const UChar32 code_point) { return Append (code_point); }
  //@}

  /**
   * insert other's characters into this Str after index, returning
   * *this. After the following:
   *
   * \code
   *   str = Str("onethree").insert (3, "two");
   * \endcode
   *
   * str looks like "onetwothree".
   */
  //@{
  Str &Insert (int64 index, const Str &other);
  Str &Insert (int64 index, const UChar32 code_point);
  //@}

  /**
   * comparison and equality testing. Compare() returns 0 if the
   * strings contain the same characters, -1 if the first different
   * character is numerically lower in the object string than in the
   * argument, 1 if the opposite is true. This is a fine
   * computer-centric stable sort, but won't give you meaningful
   * lexical comparison. CaseCmp() has similar semantics but full
   * unicode case-folding is done before the comparison. That's more
   * useful (but likely slower). The equality operators return boolean
   * values, as you would expect. They actually use Compare()
   * internally (so, as you also probably expect, they test the
   * equality of the characters in the Str, but not the regular
   * expression match state).
   *
   * The null string compares less than any other string (except
   * itself), including the empty string.
   */
  //@{
  int Compare (const Str &other) const;
  int CaseCmp (const Str &other) const;
  bool operator< (const Str &other) const;
  bool operator> (const Str &other) const;
  //@}

  inline bool operator== (const Str &other) const
  {
    if (u8IsStale || other.u8IsStale)
      return !Compare (other);
    if (u8ByteLength != other.u8ByteLength)
      return false;
    return !memcmp (u8, other.u8, u8ByteLength);
  }

  inline bool operator== (const char *other) const
  {
    if (!other)
      return IsNull ();
    if (u8IsStale)
      return !Compare (Str (other));

    // compare u8ByteLength+1 (note the "+1") to catch the terminating null
    return !strncmp (u8, other, u8ByteLength + 1);
  }

  inline bool operator!= (const Str &other) const { return !(*this == other); }

  inline bool operator!= (const char *other) const { return !(*this == other); }

  /**
   * case transformation, returning *this. We thank (and place great
   * faith in) the many people who have worked hard on the unicode
   * specification.
   */
  //@{
  Str &Upcase ();
  Str &Downcase ();
  //@}

  /**
   * whitespace trimming and newline removal, returning *this in each
   * case. Strip () removes whitespace from the front and back of a
   * Str. Whitespace is defined as [\\t\\n\\f\\r\\p{Z}]. Chomp ()
   * removes the final characters of a Str if those characters match
   * its separator argument. The default value of separator is "\n".
   */
  //@{
  Str &Strip ();
  Str &Chomp (const Str &separator = OB_NEWLINE_SEP);
  //@}

  /**
   * character order inversalization, returning \*this. (Or, if you
   * prefer, returning siht*.)
   */
  Str &Reverse ();

  /**
   * search for the first or final occurence of a substring within a
   * Str, returning the start index of the substring if found, -1
   * otherwise.
   */
  //@{
  int64 Index (const Str &substring) const;
  int64 Rindex (const Str &substring) const;
  //@}

  /**
   * Returns true if the string contains substring at least once. If substring
   * is the empty string, always returns true.
   */
  //@{
  bool Contains (const Str &substring) const;
  //@}

  /**
   * a Str created with the default constructor or by assignment from
   * NULL is uninitialized. The IsNull() method returns true for such
   * a string. SetToNull() is equivalent to "= NULL" and Set(NULL) and
   * puts a string into an uninitialized state. An uninitialized Str
   * is equal only to another uninitialized Str (and not to an empty
   * -- "" -- Str). However, a call to utf() returns "" for an
   * uninitialized Str. And more generally, any non-const method
   * called on an uninitialized Str will treat the Str as equivalent
   * to empty.
   */
  //@{
  bool IsNull () const;
  void SetToNull ();
  //@}

  /**
   * and a global Null Str to be used anywhere you might want to
   * return an uninitialized value from an accessor.
   */
  static const Str Null;

  /**
   * emptiness - returns true for Str objects that are uninitialized
   * or that contain ""
   */
  bool IsEmpty () const;

  /**
   * Equivalent to Set("").
   */
  void SetToEmpty ();

  /**
   * Returns true iff the regular expression \a pattern matches
   * the Str.  Does not modify the Str's internal match state
   * in any way.  Therefore, no additional information, such as
   * match positions, is available.
   *
   * \note If you are using a pattern which does not have any
   * regular expression meta-characters, and you have inexplicably
   * chosen to do string matching in tight loops in performance-critical
   * code, you might want to consider using Index() instead, which
   * should be faster.
   */
  bool Matches (const Str &pattern) const;

  /**
   * regular expression match and match again. str.Match(pattern)
   * kicks off a regular expression match, starting at the beginning
   * of the Str and returning true if the pattern match was
   * successful. MatchAgain() reruns the match, starting at the tail
   * index of the previous match substring. If there was no preceding,
   * successful match, MatchAgain() returns false and does nothing.
   * Match() with no argument clears and nullifies the match state.
   */
  //@{
  bool Match (const Str &pattern);
  bool MatchAgain ();
  bool Match ();
  //@}

  /**
   * regular expression match status accessors. MatchHasMatched()
   * returns true if the immediately previous Match() or MatchAgain()
   * call returned true. MatchHasErred() returns true if the
   * immediately previous Match() or MatchAgain() call resulted in an
   * error condition -- most commonly a badly formed regular
   * expression pattern. MatchErrorStr() returns a Str object
   * describing the current error condition, or the empty Str if there
   * is no extant error.
   */
  //@{
  bool MatchHasMatched () const;
  bool MatchHasErred () const;
  Str MatchErrorStr () const;
  //@}

  /**
   * regular expression match substring accessors. MatchSlice()
   * returns the text matched by the immediately previous Match() or
   * MatchAgain() call. (Or an empty Str, in the absence of proximal
   * matching success.) MatchPreSlice() returns all of the Str's text
   * preceding the matched region; MatchPostSlice() returns all the
   * Str's text following it. (And both of those also return the empty
   * Str if there's no current match to work with.)
   */
  //@{
  Str MatchSlice () const;
  Str MatchPreSlice () const;
  Str MatchPostSlice () const;
  //@}

  /**
   * regular expression match capture group access. In keeping with
   * the perl tradition, MatchNthCaptureSlice(0) is equivalent to
   * MatchSlice(). MatchNumCaptures() returns the number of capture
   * groups currently accessible via MatchNthCaptureSlice().
   */
  //@{
  int MatchNumCaptures () const;
  Str MatchNthCaptureSlice (int which_capture) const;
  //@}

  /**
   * Return matches as a range of character positions, rather than as
   * a string.  This makes it easy to do search-and-replace where the
   * replacement text is computed based on the matched text.
   */
  //@{
  StrRange MatchRange () const;
  StrRange MatchNthCaptureRange (int which_capture) const;
  //@}

  /**
   * one-time positional, and multi-shot regexp, search and
   * replace. Both methods return *this. Replace() substitutes
   * replacement's text for the characters between index and
   * index+length. As usual, negative indexes wrap backwards. Passing
   * Replace() a length of 0 is equivalent to calling
   * Insert(). ReplaceAll() initiates a Match(), substituting the
   * replacement text for the match text on success, then
   * MatchAgain()s and substitutes again until there are no more
   * matches. If there is a regular expression error, MatchHasErred()
   * and MatchErrorStr() will be set appropriately. Error or no,
   * ReplaceAll() clobbers any existing match state.
   */
  //@{
  Str &Replace (int64 index, int64 length, const Str &replacement);
  Str &Replace (int64 index, const UChar32 replacement);
  Str &ReplaceAll (const Str &pattern, const Str &replacement);
  Str &ReplaceFirst (const Str &pattern, const Str &replacement);
  //@}

  /**
   * split Str into an array of substrings. This is very much ICU's
   * split: the pattern is taken as a delimiter; the Str is cut into
   * pieces at each occurence of the delimiter.  However, unlike Perl's
   * split, and unlike some versions of ICU, blank trailing fields are
   * not deleted.  (Although the number of fields will be zero when
   * splitting the empty string.)
   *
   * Further elaboration on this concept is available in the ICU docs:
   * http://userguide.icu-project.org/strings/regexp#TOC-Using-split-
   *
   * In regular expressions, a number of metacharacters are used to
   * denote actions or delimit groups:
   * http://en.wikipedia.org/wiki/Regular_expression#Syntax
   *
   * This means that when these metacharacters need to be matched
   * literally, they must be escaped with a backslash:
   * http://www.regular-expressions.info/characters.html#special
   *
   * Since Str offers no interface for precompiling regular
   * expressions the way many regular expression libraries do, the
   * regular expression must be recompiled every time Split() is
   * called.  Furthermore, since the str_array is passed by value, and
   * the Strs within it are contained by value, that means that the
   * result Strs will be copied multiple times on the return path, and
   * since Str does not implement either reference counting or the
   * short string optimization, copying Strs is expensive.  For
   * example, a typical Split may take 18 microseconds, which might be
   * too slow for use in tight loops.
   */
  str_array Split (const Str &pattern) const;

  /**
   * Returns a hash of the string.
   */
  unt64 Hash () const;


  /**
   * iterators pointing at the first character of a Str and just
   * beyond the last character, such that on the empty string,
   * str.begin() == str.end(). StrIterator is a utility class that
   * supports a minimal set of operations: dereference (operator *),
   * pre- and post-increment and -decrement, ==/< comparison, and copy
   * construction and assignment. Like At(), StrIterator dereference
   * returns 0 on an out-of-bounds access attempt. After creating an
   * iterator, any subsequent operation on the Str itself invalidates
   * that iterator (further calls on which yield undefined return
   * values).
   */
  //@{
  StrIterator begin () const { return StrIterator (this, begin_marker ()); }
  StrIterator end () const { return StrIterator (this, end_bounds_marker ()); }
  //@}

  /**
   * unpacked iteration operations. begin_marker() returns an opaque
   * "marker" (an index of sorts) pointing at the first character of
   * the string. end_marker() returns one pointing at the last
   * character of the string. begin_bounds_marker() returns a marker
   * pointing just prior to the first characer of the
   * string. end_bounds_marker() returns one pointing just after the
   * last character. These methods are used to set up and terminate
   * iterator loops. MoveMarker() advances or rewinds an interator an
   * integral number of characters (that's the distance
   * argument). Fetch() gets the character pointed to by
   * marker. Fetch() can be a good deal faster than At() for some
   * multi-byte character sequences. Like At() Fetch() returns 0 on an
   * out-of-bounds access attempt. FetchAndMoveMarker() combines its
   * two eponymous operations into a single call. Any operation other
   * than these methods invalidates any extant markers (further use of
   * which in these calls yields undefined return values).
   */
  //@{
  str_marker begin_marker () const;
  str_marker end_marker () const;
  str_marker begin_bounds_marker () const;
  str_marker end_bounds_marker () const;
  str_marker MoveMarker (str_marker marker, int64 distance) const;
  UChar32 Fetch (str_marker marker) const;
  UChar32 FetchAndMoveMarker (str_marker *marker, int64 distance) const;
  //@}

  /**
   * static utility functions to determine the length, in unicode
   * characters (code points) of a utf8 byte sequence.
   */
  //@{
  static int64 UTF8_Length (const char *);
  static int64 UTF8_Length (const char *, int64 index, int64 byte_length);
  //@}

  /**
   * Format a string using an sprintf-style format specification, and
   * return the result as a Str.  This is just a static variant of
   * Str::Sprintf().
   */
  static Str Format (const char *format, ...) OB_FORMAT (printf, 1, 2);

  /**
   * Variant of Str::Format() that takes a va_list.
   */
  static Str VFormat (const char *format, va_list args);

 OB_PRIVATE:
  // common code shared by constructors
  void _Init () noexcept;
  // make sure u8ByteCapacity is at least capacity
  // (and therefore the allocated size of u8 is at least capacity + 1)
  void _EnsureU8Capacity (int64 capacity);
  // sets u8ByteLength to length, NUL-terminates u8, and sets u8IsStale to false
  // (basically, _EnsureU8Capacity does everything you need before copying
  // data to u8, and _SetU8Length does everything you need after copying
  // data to u8.
  void _SetU8Length (int64 length);
  // combines _EnsureU8Capacity, memcpy, and _SetU8Length into one
  // convenient operation.
  void _SetU8 (const char *str, int64 length);
  void _FreshenUString () const;
  void _FreshenU8 () const;
  void _CopyFieldsIntoThis (const Str &to_copy);
  void _CopyMatchDataIntoThis (const Str &to_copy);
  int64 _CodeUnitIndexToCodePointIndex (int32 cuidx) const;

  typedef void (StrMatchData::*ReplaceFunc) (const Str &replacement);

  Str &_Replace (const Str &pattern, const Str &replacement, ReplaceFunc func);
};

// Borrowed from boost, kind of
template <class T>
inline void ob_hash_combine (std::size_t &seed, const T &v)
{
  std::hash<T> hasher;
  seed ^= hasher (v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

// http://www.boost.org/doc/libs/1_47_0/doc/html/hash/custom.html
inline size_t hash_value (const Str &s)
{
  return static_cast<size_t> (s.Hash ());
}

/**
 * A hash function wrapper that makes Str easy to use in Boost or TR1
 * unordered maps.
 */
struct Str_hash
{
  std::size_t operator() (Str const &key) const { return key.Hash (); };
};

//
// must these following live inside the city limits of namespaceville?
//
// they must, for LLVM's sake, plus it is the right thing to do:
// http://stackoverflow.com/questions/171862/namespaces-and-operator-overloading-in-c
//

inline bool operator== (const char *str_c, const oblong::loam::Str &str)
{
  return (str == str_c);
}

inline bool operator!= (const char *str_c, const oblong::loam::Str &str)
{
  return (str != str_c);
}

OB_LOAMXX_API oblong::loam::Str operator+ (const char *str_c,
                                           const oblong::loam::Str &str);
OB_LOAMXX_API oblong::loam::Str operator+ (const UChar32 code_point,
                                           const oblong::loam::Str &str);
}
}  //   tears, eulogies for namespaces loam and oblong

#include <functional>
namespace std {

template <>
struct hash<oblong::loam::Str>
{
  typedef oblong::loam::Str argument_type;
  typedef std::size_t result_type;

  result_type operator() (argument_type const &a) const { return a.Hash (); }
};
}

#endif
