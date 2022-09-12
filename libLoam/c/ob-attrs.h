
/* (c)  oblong industries */

/**************************************************
 * This file contains macros for gcc attributes,
 * which either improve compiler warnings or code
 * optimization.
 *
 * These macros expand to nothing in non-gcc compilers
 * or in versions of gcc where they are not supported.
 *
 * Since these just affect warnings or optimization,
 * the code will still be perfectly correct if the
 * attributes are not supported.
 *
 * See gcc manual for further documentation:
 * http://gcc.gnu.org/onlinedocs/gcc-4.3.2/gcc/Function-Attributes.html
 *
 * and also Clang manual:
 * http://clang.llvm.org/docs/LanguageExtensions.html#feature_check
 **************************************************/

#ifndef OB_ATTRS_H
#define OB_ATTRS_H

#ifdef __has_attribute
#define OB_HAS_ATTR(expr, gccvers) expr
#else
#define OB_HAS_ATTR(expr, gccvers)                                             \
  defined (__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ >= gccvers)
#endif

/**
 * Suppresses the warning for static functions which
 * are not called.
 */
#if OB_HAS_ATTR(__has_attribute(unused), 300)
#define OB_UNUSED __attribute__ ((unused))
#else
#define OB_UNUSED
#endif

/**
 * Lets the compiler check the usage of user-defined
 * functions that take a printf-style format string.
 * \a string_index and \a first_to_check are argument
 * positions, numbered starting at 1 for most functions,
 * or numbered starting at 2 for non-static C++ methods,
 * because "this" is 1.  See gcc manual for more information.
 */
#if OB_HAS_ATTR(__has_attribute(format), 300)
#define OB_FORMAT(archetype, string_index, first_to_check)                     \
  __attribute__ ((format (archetype, string_index, first_to_check)))
#else
#define OB_FORMAT(archetype, string_index, first_to_check)
#endif

/**
 * Lets the compiler check the usage of functions that take a variable
 * number of arguments, where the last argument is required to be
 * NULL.
 */
#if OB_HAS_ATTR(__has_attribute(sentinel), 400)
#define OB_SENTINEL __attribute__ ((sentinel))
#else
#define OB_SENTINEL
#endif

/**
 * Specifies that every function that this function calls should be
 * inlined into its body.  (This is the opposite of "inline", which
 * means that this function gets inlined into the body of every one of
 * its callers.)
 */
#if OB_HAS_ATTR(__has_attribute(flatten), 402)
#define OB_FLATTEN __attribute__ ((flatten))
#else
#define OB_FLATTEN
#endif

/**
 * Issues a warning if the function's return value is ignored.  (Only
 * enabled for gcc 4.1 and up, because OS X's gcc 4.0.1 seems to
 * erroneously think every result is unused.)
 */
#if OB_HAS_ATTR(__has_attribute(warn_unused_result), 401)
#define OB_WARN_UNUSED_RESULT __attribute__ ((warn_unused_result))
#else
#define OB_WARN_UNUSED_RESULT
#endif

/**
 * Makes a function invisible from outside the shared object where it
 * is defined.  (In other words, use this for non-API functions.)
 */
#if OB_HAS_ATTR(__has_attribute(visibility), 400)
#define OB_HIDDEN __attribute__ ((visibility ("hidden")))
#else
#define OB_HIDDEN
#endif

/**
 * Means that a function has no side effects and returns the same
 * value given the same inputs.  Slightly less restrictive than
 * OB_CONST; see gcc manual for details.
 */
#if OB_HAS_ATTR(__has_attribute(pure), 300)
#define OB_PURE __attribute__ ((__pure__))
#else
#define OB_PURE
#endif

/**
 * Means that a function has no side effects and returns the same
 * value given the same inputs.
 */
// Note: clang chokes on __has_attribute(__const__), so assume we have it there.
#if !defined(_MSC_VER) && OB_HAS_ATTR(1, 300)
#define OB_CONST __attribute__ ((__const__))
#else
#define OB_CONST
#endif

/**
 * This promises that a function will never, ever return under any
 * circumstances.  (Basically, it is useful for error-handling
 * wrappers around exit() or abort().)  It helps the compiler optimize
 * code, and can also help suppress warnings about things that could
 * only happen if an impossible code path was taken.
 */
#if OB_HAS_ATTR(__has_attribute(noreturn), 300)
#define OB_NORETURN __attribute__ ((__noreturn__))
#elif defined(_MSC_VER)
#define OB_NORETURN __declspec(noreturn)
#else
#define OB_NORETURN
#endif

/**
 * This will generate a warning when compiling code that calls the
 * function.
 */
#if OB_HAS_ATTR(__has_attribute(deprecated), 404)
#define OB_DEPRECATED __attribute__ ((deprecated))
#define OB_DEPRECATED_MSG(msg) __attribute__ ((deprecated (msg)))
#elif defined(_MSC_VER)
#define OB_DEPRECATED __declspec(deprecated)
#define OB_DEPRECATED_MSG(msg) __declspec(deprecated (msg))
#else
#define OB_DEPRECATED
#define OB_DEPRECATED_MSG(msg)
#endif


/**
 * This will cause an inline function to always be inlined, even when
 * not optimizing.
 */
#if OB_HAS_ATTR(__has_attribute(always_inline), 301)
#define OB_ALWAYS_INLINE __attribute__ ((always_inline))
#else
#define OB_ALWAYS_INLINE
#endif

/**
 * This specifies a "hot" function which is aggressively optimized.
 */
#if OB_HAS_ATTR(__has_attribute(hot), 403)
#define OB_HOT __attribute__ ((hot))
#else
#define OB_HOT
#endif

/**
 * Specifies that the function will not call back to another function
 * in the caller's translation unit.
 */
#if OB_HAS_ATTR(__has_attribute(leaf), 406)
#define OB_LEAF __attribute__ ((leaf))
#else
#define OB_LEAF
#endif

#if OB_HAS_ATTR(__has_attribute(artificial), 403)
#define OB_ARTIFICIAL __attribute__ ((artificial))
#else
#define OB_ARTIFICIAL
#endif

/* clang-format off */

/* Pasting symbols together with ## requires two levels of macro expansion. */
#define OB_CONCAT_IMPL( x, y ) x##y
#define OB_MACRO_CONCAT( x, y ) OB_CONCAT_IMPL( x, y )

/**
 * Arranges for pre_expr to be called before main(), and post_expr to
 * be called after main().  This macro should be used at the top level
 * of a file.  The return value of pre_expr and post_expr, if any, is
 * ignored, so it's okay for them to return void.  If you need only a
 * pre_expr or post_expr, you may set the other one to ob_nop().
 *
 * Just as with C++ global constructors, there's no guarantee what
 * order these will run in between different files, so don't make
 * calls from one of these that assumes another file's initialization
 * has happened.
 *
 * Uses the nonportable __COUNTER__ to allow more than one per file.
 * Useful in unity builds, where all the source is concatenated
 * into a single file.
 */
#if defined (__cplusplus)     /* On Windows, we compile everything as C++ */
#define OB_PRE_POST(pre_expr, post_expr)        \
  namespace {                                   \
  class obclass__                               \
  { public:                                     \
    obclass__ () { pre_expr; }                  \
    ~obclass__ () { post_expr; }                \
  };                                            \
  obclass__ obdummy__;                          \
  }
#elif OB_HAS_ATTR (__has_attribute (constructor), 300)
#define OB_PRE_POST(pre_expr, post_expr) \
  static __attribute__ ((constructor))   \
  void OB_MACRO_CONCAT(obconstructor__,__COUNTER__) (void) \
  { pre_expr;                            \
  }                                      \
  static __attribute__ ((destructor))    \
  void OB_MACRO_CONCAT(obdestructor__,__COUNTER__) (void)  \
  { post_expr;                           \
  }
#elif defined(_MSC_VER)
  /* Visual C supports static constructors in C++, but not in C,
   * so we have to add entries to the C runtime's initializer table.
   *
   * The following code fakes unique function names by appending line numbers.
   * Thus if two files call OB_PRE_POST from same line number, you'll get a
   * linker error.  If that happens, just move one of the OB_PRE_POST
   * calls up or down a line.
   *
   * https://msdn.microsoft.com/en-us/library/bb918180.aspx
   * "CRT Initialization" documents how to add pointers to the
   * global constructor table.
   * i.e. open the table with __pragma(section(".CRT$XCU",read))
   * and then append to it by defining an extern function pointer variable
   * with attribute __declspec(allocate(".CRT$XCU")).
   * This is an assembly language technique leaking through to C.
   * Examples: https://bugzilla.gnome.org/show_bug.cgi?id=752837#c51
   * https://opensource.apple.com/source/OpenSSL098/OpenSSL098-35/src/fips/fips_premain.c
   *
   * Use pragmas to avoid the constructors being optimized
   * away on VS2015 if WholeProgramOptimization is enabled.
   *
   * Annoyingly, the symbols need to be extern (but not dllexport),
   * even though they are not really used from another object file.
   * This means they have to have unique names.
   * We fake that with __LINE__.
   */

  /* Keep a function from being optimized out by the linker. */
  #ifdef _WIN64
  #define OB_MSVC_LINKER_PRAGMA_KEEP2(_func) \
          __pragma(comment(linker,"/include:" #_func))
  #else
  #define OB_MSVC_LINKER_PRAGMA_KEEP2(_func)\
           __pragma(comment(linker,"/include:_" #_func))
  #endif
  #define OB_MSVC_LINKER_PRAGMA_KEEP(_func) OB_MSVC_LINKER_PRAGMA_KEEP2(_func)

  /* OBUNIQ(x) uses preprocessor tricks to keep OB_PRE_POST calls from clashing.
   * e.g. if on line 75 of your file, you have
   *    int OBUNIQ(sayhi)(void) { printf("hi\n"); }
   * that defines the function
   *    int sayhi75(void) { printf("hi\n"); }
   * Only used by OB_PRE_POST.
   */
  #define OB_TOKENPASTE2(x, y) x ## y
  #define OB_TOKENPASTE(x, y) OB_TOKENPASTE2(x, y)
  #define OBUNIQ(x) OB_TOKENPASTE(x, __LINE__)

  /* The C runtime's global initializer table is a big array of
   * pointers to functions that take void and return int.
   * Declare a type for that to make the following macro clearer.
   */
  typedef int (*ob_crt_initializer_t)(void);

  #define OB_PRE_POST(pre_expr, post_expr)                                    \
   /* Discourage compiler from optimizing anything away. */                   \
   __pragma(optimize("", off))                                                \
   /* Open MSVC C Runtime's initializer table section for later use. */       \
   __pragma(section(".CRT$XCU",read))                                         \
                                                                              \
   /* Wrap pre_expr to make a MSVC C runtime initter that returns success. */ \
   int OBUNIQ(ob_pre_fn_) (void) { pre_expr; return 0; }                      \
   /* Keep linker from optimizing ob_pre_fn_ away. */                         \
   OB_MSVC_LINKER_PRAGMA_KEEP(OBUNIQ(ob_pre_fn_))                             \
   /* Append &ob_pre_fn_ to C runtime's global initializer table. */          \
   __declspec(allocate(".CRT$XCU"))                                           \
   ob_crt_initializer_t OBUNIQ(ob_pre_fn_ptr_) = OBUNIQ(ob_pre_fn_);          \
                                                                              \
   /* Handle post_expr similarly, but queue call until shutdown. */           \
   void OBUNIQ(ob_post_fn_) (void) { post_expr; }                             \
   int OBUNIQ(ob_post_q_) (void) { atexit(OBUNIQ(ob_post_fn_)); return 0; }   \
   OB_MSVC_LINKER_PRAGMA_KEEP(OBUNIQ(ob_post_q_))                             \
   __declspec(allocate(".CRT$XCU"))                                           \
   ob_crt_initializer_t OBUNIQ(ob_post_q_ptr_) = OBUNIQ(ob_post_q_);          \
                                                                              \
   /* Restore original compiler optimization settings */                      \
   __pragma(optimize("", on))

#else
#define OB_PRE_POST(pre_expr, post_expr) \
  ob_pre_post_not_supported_see_ob_attrs_h_and_add_your_compiler
#endif

/* clang-format on */

/**
 * This isn't strictly speaking an attribute, but:
 * Specifies that #pragma message is supported.
 */
#if defined(__GNUC__) && (__GNUC__ * 100 + __GNUC_MINOR__ >= 404)
#define OB_HAS_PRAGMA_MESSAGE 1
#else
#undef OB_HAS_PRAGMA_MESSAGE
#endif

#endif /* OB_ATTRS_H */
