#.rst:
# Find Oblong's g-speak API
#
# Prefers CMake 3.6.3+, requires CMake 3.5.1+.
#
# User apps should normally load this via
#    find_package(Oblong)
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module defines the following variables:
#
#   ASAN                   - whether to use address sanitizer
#   BUILD_TESTS            - whether to build tests
#   CMAKE_BUILD_TYPE       - Debug, RelWithDebInfo, Release
#   CMAKE_VERBOSE_MAKEFILE - whether build should be verbose
#   COVERAGE_FLAGS         - compiler flags for COVERAGE
#   COVERAGE               - whether to use code coverage
#   G_SPEAK_HOME           - top level directory for g-speak
#   OPTIMIZE_FLAGS         - compiler flags for CMAKE_BUILD_TYPE
#   OB_G_SPEAK_LIB_TYPE    - from USE_STATIC_G_SPEAK; for use as 2nd arg of ObFindLibs when finding g-speak libs
#   SANITIZER_FLAGS        - compiler flags for ASAN/TSAN
#   TSAN                   - whether to use thread sanitizer
#   USE_STATIC_G_SPEAK     - tell ObFindLibs callers (via OB_G_SPEAK_LIB_TYPE) to favor static g-speak libraries
#   YOBUILD                - top level directory for libraries used by g-speak
#   YOVERSION              - major version of YOBUILD
#
# Functions
# ^^^^^^^^^
#
# This module includes Ob*.cmake to define the following functions:
#
#   ObFindLibs()        - see $G_SPEAK_HOME/lib/cmake/ObFindLibs.cmake
#   ObCheckCxxFlags()   - see $G_SPEAK_HOME/lib/cmake/ObCheckCxxFlags.cmake
#
# For apps using the Chromium Embedded Framework to embed a browser,
# ObFindLibs() may also include FindCEFxxx.cmake to define following functions:
#   cef_bless_app()     - sprinkle cef symlinks on app
#   cef_install_blessed_app() - install app with cef symlinks
# See $YOBUILD/cefxxxx/lib/cmake/FindCEFxxxx.cmake
#
# ::
# Copyright (C) 2015-2017 Oblong Industries

# If no install prefix set, default to G_SPEAK_HOME
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
   SET(CMAKE_INSTALL_PREFIX "${G_SPEAK_HOME}" CACHE PATH "Install prefix" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

# Tell cmake about where to find Oblong's libraries (like g-speak and obs)
LIST(INSERT CMAKE_PREFIX_PATH 0 "${G_SPEAK_HOME}")

# The INCLUDE directives below are intended to find foo.cmake in the
# same directory as this file, but alas, cmake doesn't behave like
# the C preprocessor... it only finds them because we omit their
# .cmake suffix, which makes cmake search CMAKE_MODULE_PATH,
# which we set in FindOblong.cmake (or yovo's CMakeLists.txt, FIXME).

# Figure out where g-speak's dependencies live
INCLUDE("ObGetSettings")
#ObGetYobuild(YOBUILD)
#ObGetYoversion(YOVERSION)

# Tell cmake about where to find Oblong's dependencies (like pkg-config)
#LIST(INSERT CMAKE_PREFIX_PATH 0 "${YOBUILD}")

# Get cache values like ASAN, BUILD_TESTS, COVERAGE, etc.
INCLUDE("ObCommonOpts")
ObCommonOpts()

# Kludge: oblong's boost doesn't know where it lives, so turn off autolinking...
ADD_DEFINITIONS("-DBOOST_ALL_NO_LIB")

IF (WIN32)
    # Kludge: on Windows, add g-speak install directories to linker search path, else it doesn't find our dlls
    # FIXME: check these
    SET(CMAKE_EXE_LINKER_FLAGS "/LIBPATH:\"${YOBUILD}/binaries\";\"${G_SPEAK_HOME}/lib/${CMAKE_BUILD_TYPE}\"")
    SET(CMAKE_SHARED_LINKER_FLAGS "/LIBPATH:\"${YOBUILD}/binaries\"")
    # Kludge: on Windows, add yobuild's gstreamer to pkgconfig search path
    # FIXME: build gstreamer normally in yobuild, with DLLs in ${YOBUILD}/bin as ${DEITY} intended
    set(ENV{PKG_CONFIG_PATH} "$ENV{PKG_CONFIG_PATH};${YOBUILD}/gstreamer/1.0/x86_64/lib/pkgconfig")
ENDIF()

INCLUDE("ObCheckCFlags")
INCLUDE("ObCheckCxxFlags")
INCLUDE("ObFindLibs")
INCLUDE("ObGeneratePC")

# If user's asking for verbosity, he probably would like to know these global settings:
IF (CMAKE_VERBOSE_MAKEFILE)
    MESSAGE("ObBoilerplate: G_SPEAK_HOME=${G_SPEAK_HOME}")
    MESSAGE("ObBoilerplate: YOBUILD=${YOBUILD}")
    MESSAGE("ObBoilerplate: ENV{PATH}=$ENV{PATH}")
    MESSAGE("ObBoilerplate: ENV{PKG_CONFIG_PATH}=$ENV{PKG_CONFIG_PATH}")
ENDIF()

# Caller should now call ObFindLibs() and oblong_append_supported_cxx_flags()
