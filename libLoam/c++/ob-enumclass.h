
/* (c)  oblong industries */

#ifndef ENUM_DECLARE_IS_WHY_WE_CANT_HAVE_NICE_THINGS
#define ENUM_DECLARE_IS_WHY_WE_CANT_HAVE_NICE_THINGS

#include <libLoam/c/ob-coretypes.h>


// why this terrible construction? well, g++ on 14.04 (gcc 4.8.4) wants
// the visibility attribute after the enum class name, but when that
// happens it requires a base type also.  clang, on the other hand,
// does this correctly, that is, expects the visibility attribute
// before the enum class name.  With much turdliness, clang also defines
// __GNUC__ even though that's the way everyone will tell you to detect
// gcc/g++.  so, first detect clang, then detect g++, then assume that
// whatever compiler you're working with is implemented correctly in
// this respect. As this macro only ensures that
// the visibility macro is in the place the compiler expects, using this
// macro is not required when the enum class is declared internal to
// another class which is already marked as OB_*_API; in that case just
// use a bare enum class.  See:
// https://gitlab.oblong.com/platform/docs/wikis/how-to-api


/**
 * A macro to declare enum classes with the visibility specifier in the
 * correct place.
 */
#ifdef __clang__
#define OB_ENUMCLASS_DECLARE_BASE(VIS, NAME, TYPE) enum class VIS NAME : TYPE
#elif defined __GNUC__
#define OB_ENUMCLASS_DECLARE_BASE(VIS, NAME, TYPE) enum class NAME : TYPE VIS
#else  //anything else
#define OB_ENUMCLASS_DECLARE_BASE(VIS, NAME, TYPE) enum class VIS NAME : TYPE
#endif

#define OB_ENUMCLASS_DECLARE(VIS, NAME)                                        \
  OB_ENUMCLASS_DECLARE_BASE (VIS, NAME, int)

#endif
