
/* Simple argument-parsing class
 * Copyright (C) 2001 Patrick E. Pelletier <ppelleti@users.sourceforge.net>
 * Copyright (C) 2009-2010 Oblong Industries */

#ifndef ARG_PARSE_PHOENIX
#define ARG_PARSE_PHOENIX


#include <libLoam/c/ob-attrs.h>
#include <libLoam/c/ob-types.h>

#include <libLoam/c++/Str.h>

#include <libLoam/c++/LoamxxRetorts.h>
#include <boost/logic/tribool.hpp>


namespace oblong {
namespace loam {

class ArgParseInternalData;  // hide implementation details

/**
 * The ArgParse class allows you to specify names of options that you
 * want parsed, along with a usage message for them. Options come in
 * four flavors: flag, int, float, and string. Flags don't take
 * arguments, but the other kinds do. For an option that takes an
 * argument, it can be specified with an equals sign, with a colon, or
 * by putting it in the next element of argv.  ("--foo=stuff",
 * "--foo:stuff", or "--foo stuff", respectively)
 *
 * The flavors that take arguments also come in array flavors. With an
 * array, you specify a pointer to a vector of the basic type, instead
 * of just a pointer to a basic type. This allows the option to appear
 * more than once, and the new values are appended to the
 * array. Optionally, you can also specify a separator character, so
 * that multiple array elements can be parsed up from a single
 * instance of the option.
 *
 * Options can start with either a single dash or a double dash, but
 * see AllowOneCharOptionsToBeCombined() for more information.
 *
 * The argument "--" all by itself signals the end of the options, and
 * everything after it will go into the Leftovers (see Leftovers())
 * even if it starts with a dash.
 *
 * The usage text for an option gets appended immediately after the
 * name of the option and its aliases. The ASCII BEL character (\\a)
 * can be used as a special kind of tab, which moves the cursor to the
 * current indent level (see UsageHeader()), to make it easy to make
 * things line up in columns.
 *
 * Rationale for some design decisions:
 *
 * Things which need to be modified are passed as pointers, not as
 * references, because some people feel that non-const references make
 * what's going on less clear, and I kind of agree with them.
 *
 * ArgParse is not a KneeObject or even an AnkleObject, on the basis
 * of Kwin's comment in bug 507 that objects meant to be constructed
 * on the stack shouldn't be AnkleObjects.  And ArgParse is such a
 * thing that you ought to declare on the stack.  (See also bug 904
 * comment 4.)
 *
 * Indentation in usage messages is required to be explicitly inserted
 * with "\a", rather than inserted automatically at the beginning of
 * the line, in order to allow you to add documentation for the
 * argument, which is before the indent, like this:
 *
 * \code
 *   t2.UsageHeader ("Archive format selection:", 37);
 *   t2.ArgFlag ("V", "=NAME\acreate archive with volume name NAME\n      "
 *               "        PATTERN\aat list/extract time, a globbing PATTERN",
 *               &junkf);
 *   t2.Alias ("V", "label");
 *   t2.AllowOneCharOptionsToBeCombined();
 * \endcode
 *
 * which produces:
 *
 * \code
 * Archive format selection:
 *   -V, --label=NAME                   create archive with volume name NAME
 *               PATTERN                at list/extract time, a globbing PATTERN
 * \endcode
 */

class OB_LOAMXX_API ArgParse
{

 public:
  // Basic types: ArgParse always uses these typedefs, so that there's
  // just one place to change things if you want to use float32 instead
  // of float64, or std::string instead of Str, etc.
  // They're just here to make maintaining ArgParse easier.  You can
  // either use the typedefs or the underlying types they refer to
  // in your code that calls ArgParse... whichever makes you happier!
  typedef bool apflag;
  typedef int64 apint;
  typedef float64 apfloat;
  typedef Str apstring;

  // Array types: ditto
  typedef ObTrove<apint> apintvec;
  typedef ObTrove<apfloat> apfloatvec;
  typedef ObTrove<apstring> apstringvec;

  // These two constants are valid values for the "separator"
  // argument to argInts, argFloats, and argStrings.  (Any ASCII
  // character is also a valid argument.)  SEP_NONE means that the
  // only way to get more than one thing in the array is to specify
  // the option more than once.  (e. g. "--foo one --foo two")
  // SEP_ARGV means that each element of argv, up to the next option,
  // will be put into the array.  (e. g. "--foo one two")  Using an
  // ASCII character means that only one argv element is used, but
  // it can become multiple elements in the array by being split
  // on the given character.  (e. g. "--foo one,two")
  enum
  {
    SEP_NONE = -1,
    SEP_ARGV = -2
  };

  enum ArgType
  {
    TYPE_NONEXISTENT,
    TYPE_ALIAS,
    TYPE_NEGATED,
    TYPE_REAL_OPTION
  };

  struct ArgInfo
  {
    ArgType typ;
    /**
     * real name.  Only meaningful for TYPE_ALIAS.
     */
    apstring aliasFor;
    /**
     * all aliases for this option.
     */
    apstringvec aliases;
    /**
     * Usage string supplied when option was created.
     */
    apstring usage;

    ArgInfo () : typ (TYPE_NONEXISTENT) {}

    // relying on the compiler for copy and move constructors and assignments
  };

  /**
   * As many arguments as possible.
   */
  static const int UNLIMITED;

  /**
   * Default contructor; gives you the most flexibility.
   */
  ArgParse ();

  /**
   * "easy start" constructor.  Reduces the amount of boilerplate you
   * have to write, at the loss of some flexibility.  Calls
   * ob_set_program_arguments() with argc and argv, so they can be
   * used by EasyFinish(), as well as elsewhere in your program.  Note
   * that, unlike Parse(), the argv here should include the program
   * name in argv[0].  Adds an initial UsageHeader() of the form
   * "Usage: " + argv[0] + " [options] " + non_option_arg_description,
   * so \a non_option_arg_description should explain what arguments
   * your program takes apart from the options.  (e. g. if it takes a
   * filename.)  If you do not take any non-argument options, you can
   * let non_option_arg_description default to the empty string.  Also
   * adds a "-h" option for help, automatically.  Finally, has the
   * effect of calling AllowOneCharOptionsToBeCombined(), since that's
   * the most common form of option parsing today, and we might as
   * well standardize on it at Oblong.
   */
  ArgParse (int argc, const char *const *argv,
            const apstring &non_option_arg_description = "", int indent = 25);

  ~ArgParse ();

  /**
   * Calling this method means that "-bar" will be treated as if it
   * was "-b -a -r", and to specify a multi-character option, you need
   * to use a double dash.  (e. g. "--bar") If you don't call this
   * method, then single dash and double dash are treated the same.
   */
  void AllowOneCharOptionsToBeCombined ();  // long but descriptive :)

  /**
   * Opposite of AllowOneCharOptionsToBeCombined().
   */
  void DontAllowOneCharOptionsToBeCombined ();

  /**
   * Normally, Parse() will return an error if there are any
   * unrecognized options.  But if you call this method before calling
   * Parse(), then unrecognized options will go into the leftovers,
   * without causing an error.
   */
  void AllowUnrecognizedOptions ();

  /**
   * Opposite of AllowUnrecognizedOptions().
   */
  void DontAllowUnrecognizedOptions ();

  /**
   * "--foo" will set *value to true.  If allow_negation is true, then
   * "--nofoo" will set *value to false.
   */
  void ArgFlag (apstring name, apstring usage, apflag *value,
                bool allow_negation = true);

  /**
   * "--foo" will set *value to true, and "--nofoo" will set *value to
   * false.  If you initialize *value to indeterminate, then you can
   * tell whether it was set on the command line or not.
   */
  void ArgFlag (apstring name, apstring usage, boost::logic::tribool *value);

  /**
   * \a append_default will cause the current string representation of
   * \a value to be appended to \a usage.
   */
  void ArgInt (apstring name, apstring usage, apint *value,
               bool append_default = false);
  void ArgInts (apstring name, apstring usage, apintvec *values,
                int separator = SEP_NONE);

  void ArgFloat (apstring name, apstring usage, apfloat *value,
                 bool append_default = false);
  void ArgFloats (apstring name, apstring usage, apfloatvec *values,
                  int separator = SEP_NONE);

  void ArgString (apstring name, apstring usage, apstring *value,
                  bool append_default = false);
  void ArgStrings (apstring name, apstring usage, apstringvec *values,
                   int separator = SEP_NONE);

  /**
   * Makes "aliasname" work just like "realname".  Note that
   * "realname" can be a negated flag name (if the flag allows
   * negation), so you can make "--fooless" mean "--nofoo", for
   * example.
   *
   * If "override" is true, this alias will replace any existing alias
   * with "aliasname".  If "override" is false, an error message will
   * be printed if an alias named "aliasname" already exists.
   */
  void Alias (apstring realname, apstring aliasname, bool override = false);

  /**
   * This inserts literal text into the usage message.  The order is
   * significant with respect to calls to argFlag, argInt, etc.  The
   * most common use of this would be to add the "Usage: blech
   * [options] files" at the top, but it can also be used to make
   * different "sections" of options (give the --help option to GNU
   * tar to see what I mean) or to make a footer at the end of the
   * usage.  Your string can contain newlines, but it shouldn't end in
   * a newline unless you want an extra blank line.  The "indent"
   * value specifies how to line up columns of individual usage
   * messages in the following section.  (Again, refer to the GNU tar
   * usage message to see what I mean.)
   */
  void UsageHeader (apstring text, int indent = 25);

  /**
   * This makes the parsing actually happen.  Returns true on
   * success, or false on failure.
   * \note This method expects the actual program arguments only;
   * it does not skip argv[0].  So if you are passing in the
   * arguments from main(), you'll actually need to pass
   * argc-1, argv+1.
   */
  bool Parse (int argc, const char *const *argv);

  /**
   * This makes the parsing actually happen.  Returns OB_OK on
   * success, or OB_ARGUMENT_PARSING_FAILED on failure.
   * \note This method expects the actual program arguments only;
   * it does not assume the first element is the program name.
   */
  ObRetort Parse (const apstringvec &args);

  /**
   * A convinence method that provides a
   * standard way to exit with error and usage.
   */
  OB_NORETURN void ErrorExit (Str errorMessage);

  /**
   * If Parse() returns false, this method will give you an error
   * message that describes what's wrong.  Note that although it's
   * legal to call Parse() more than once on the same ArgParse object,
   * only the most recent error message is retained.
   */
  apstring ErrorMessage () const;

  /**
   * Returns a usage string made up of the usage messages of the
   * individual arguments, with things from UsageHeader() interspersed
   * at the appropriate places.  The string returned contains embedded
   * newlines, and also a trailing newline.
   */
  apstring UsageMessage () const;

  /**
   * Returns any leftover arguments.  (arguments which didn't start
   * with a dash and didn't get eaten by another option, or any
   * arguments which appeared after "--") Again, this is only valid
   * until the next call to Parse() on this ArgParse object.
   */
  const apstringvec &Leftovers () const;

  /**
   * Determines whether the specified option exists, and returns some
   * information about it, like whether it is an alias.
   */
  ArgInfo Query (apstring name) const;

  /**
   * This is a convenience method which is easier than calling
   * Parse(), at the loss of some flexibility.  Retrieves argv and
   * argc from ob_get_program_arguments(), and automatically removes
   * argv[0] for you.  This assumes that you've called
   * ob_set_program_arguments() previously, either directly or by
   * using the "easy start" constructor.  If Parse() fails,
   * automatically prints the usage message and terminates the
   * program.  Also, automatically checks that the length of
   * Leftovers() is between \a min_non_option_args and \a
   * max_non_option_args, and again terminates the program with a
   * usage message if the condition is not met.  Also automatically
   * handles the help option that was inserted by the "easy start"
   * constructor.  You can specify UNLIMITED for \a
   * max_non_option_args if you want to accept an unlimited number of
   * arguments, and you can let \a max_non_option_args default to 0 if
   * you want it to be the same as \a min_non_option_args.  (i. e. you
   * accept an exact number of arguments, not a range)
   */
  void EasyFinish (int min_non_option_args = 0, int max_non_option_args = 0);

  /**
   * This is the help flag that is implicitly used by the "easy start"
   * constructor and the EasyFinish() method.  You do not need to
   * worry about this if you use both the "easy start" constructor and
   * the EasyFinish() method.  You also don't need to worry about it
   * if you use neither the "easy start" constructor nor the
   * EasyFinish() method.  You need to use it if you want to use the
   * "easy start" contrustor without the EasyFinish() method, or vice
   * versa, since they both assume the help flag is stored here.
   */
  apflag easy_help;

 OB_PRIVATE:
  ArgParseInternalData *d;

  void Bye ();
  OB_NORETURN void Exit (int code);

 private:
  // The default copy constuctor isn't suitable for this class.
  // We could write a copy constructor and assignment operator
  // that do a proper, deep copy it we wanted them, but since there's
  // probably no reason to copy an ArgParse, let's just disallow
  // copying for now.
  ArgParse (const ArgParse &);
  const ArgParse &operator= (const ArgParse &);
  ArgParse (ArgParse &&);
  ArgParse &operator= (ArgParse &&);
};
}
}  // loam, oblong


#endif  // ARG_PARSE_PHOENIX
