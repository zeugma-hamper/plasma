#.rst:
# This file provides the function:
#  ObFindLibs       - convenient access to system and Oblong libraries
#
# Prefers CMake 3.6.3+, requires CMake 3.5.1+.
#
# Copyright (C) 2015-2017 Oblong Industries

# ObFindLibs
# ----------
#
# ObFindLibs is like PKG_CHECK_MODULES, but has better error handling,
# as well as special handling for static libraries and for CEF.
# It aborts on error.
# Before calling, add G_SPEAK_HOME and YOBUILD to CMAKE_PREFIX_PATH.
#
# Syntax:
# ObFindLibs(<PREFIX> [STATIC|SHARED] <MODULE> [<MODULE>]*)
#
# Input parameters:
#   <PREFIX>      - prefix for output variables
#   STATIC        - if present, work properly with static libraries
#   SHARED        - default
#   <MODULE>...   - libraries to find (e.g. libLoam, libWebThing, gstreamer-net-1.0, etc)
#
# Output variables:
#   <PREFIX>_LDFLAGS: value to pass to TARGET_LINK_LIBRARIES to use given libs
#   <PREFIX>_CFLAGS: value to pass to ADD_COMPILE_OPTIONS to use given libs
#   <PREFIX>_INCLUDE_DIRS - not needed if you obey <PREFIX>_CFLAGS, but just in case
#
# Special case:
#   If module libcefNNNN is specified, or if module libWebThing is specified,
#   the appropriate FindCEF<NNNN>.cmake is included, providing functions
#   cef_bless_app() et al for use when linking apps that use cef or webthing.
#
# pkg-config minitutorial:
# Most system libraries' -dev packages come with a pkg-config (.pc) file
# so they can be found using pkg-config. Oblong packages do, too.
# For instance,
#   sudo apt install libxrandr-dev oblong-loam4.2
# installs
#   /usr/lib/x86_64-linux-gnu/pkgconfig/xrandr.pc
#   /opt/oblong/g-speak4.2/lib/pkgconfig/libLoam.pc
# To get a list of all available pkg-config packages, do e.g.
#   pkg-config --list-all
# after setting PKG_CONFIG_PATH properly, or do the quick-n-dirty
#   ls {/usr/lib,/usr/lib/*,/opt/oblong/*/lib}/pkgconfig
# See also:
# - https://www.freedesktop.org/wiki/Software/pkg-config/
# - http://pkgconf.org/

FIND_PACKAGE(PkgConfig REQUIRED)

FUNCTION(ObFindLibs PREFIX)
    IF(${CMAKE_VERSION} VERSION_LESS "3.6.2")
        MESSAGE(WARNING "ObFindLibs: when building libraries, consider CMake 3.6.3+ (or Oblong's 3.6.2), see https://gitlab.kitware.com/cmake/cmake/issues/16293")
    ENDIF()
    IF(${CMAKE_VERSION} VERSION_LESS "3.5.1")
        # Ubuntu 16.04 ships with cmake-3.5.1
        # We no longer test with lower versions
        MESSAGE(FATAL_ERROR "ObFindLibs: CMake 3.5.1 or later required")
    ENDIF()

    # Invalidate cache if CMAKE_PREFIX_PATH has changed
    IF(NOT(DEFINED(${PREFIX}_CMAKE_PREFIX_PATH_LAST)))
         SET(${PREFIX}_CMAKE_PREFIX_PATH_LAST "NotAnOption" CACHE STRING "CMAKE_CMAKE_PREFIX_PATH last time we searched for ${PREFIX} libraries")
         MARK_AS_ADVANCED (FORCE ${PREFIX}_CMAKE_PREFIX_PATH_LAST)
    ENDIF()
    IF(NOT (CMAKE_PREFIX_PATH STREQUAL "${${PREFIX}_CMAKE_PREFIX_PATH_LAST}"))
         UNSET(__pkg_config_checked_${PREFIX} CACHE)
         SET(${PREFIX}_CMAKE_PREFIX_PATH_LAST "${CMAKE_PREFIX_PATH}" CACHE STRING "Update ${PREFIX}'s cmake-prefix-path change detector" FORCE)
    ENDIF()

    set(options STATIC SHARED)
    set(OneValueArgs )
    set(multiValueArgs )
    cmake_parse_arguments(OFP "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    # Walk through them one by one to get (slightly) better errors
    FOREACH(lib ${OFP_UNPARSED_ARGUMENTS})
        PKG_CHECK_MODULES(${lib} ${lib})
        IF (NOT "${${lib}_FOUND}" STREQUAL "1")
            MESSAGE("ObFindLibs: CMAKE_PREFIX_PATH=${CMAKE_PREFIX_PATH}")
            MESSAGE("ObFindLibs: ENV{PKG_CONFIG_PATH}=$ENV{PKG_CONFIG_PATH}")
            MESSAGE(FATAL_ERROR "ObFindLibs: Could not find pkg-config module ${lib} or one of its dependencies")
        ENDIF()
        IF ("${${lib}_LIBRARIES}${${lib}_CFLAGS}" STREQUAL "")
            MESSAGE(FATAL_ERROR "ObFindLibs: Found pkg-config module ${lib}, but it was empty, perhaps a dependency is missing.")
        ENDIF()

        # Freaky bit #1 to support CEF:
        # detect implicit or explicit references to CEF, remember which branch it needs
        IF (lib MATCHES "^CEF([0-9]+)")
            set(CEF_BRANCH ${CMAKE_MATCH_1})
        ELSEIF ((lib STREQUAL "libWebThing") OR (lib STREQUAL "libWebThing2"))
            # This calls obs get-cef-version, which just looks in debian/control.
            # (ObGetCefBranch used to bypass obs and use pkg-config to get the variable from libWebThing.pc,
            # but that ran into https://gitlab.kitware.com/cmake/cmake/issues/15805 )
            ObGetCefBranch(CEF_BRANCH)
        ENDIF()
    ENDFOREACH()

    # Then let pkg-config deduplicate options
    # (required, otherwise we get corrupt .dylibs on mac, see bug 15121)
    PKG_CHECK_MODULES(${PREFIX} REQUIRED ${OFP_UNPARSED_ARGUMENTS})

    IF (OFP_STATIC)
        # Fun fact: static libraries have to come after their references, else symbols won't be found.
        # Easiest workaround is to list all libraries twice.
        SET(${PREFIX}_LDFLAGS " ${${PREFIX}_STATIC_LDFLAGS} ${${PREFIX}_STATIC_LDFLAGS}")
    ENDIF()

    if (APPLE)
        # Somehow any -rpath from pkgconfig is dangerous; cmake generates its own clashing ones.  So filter 'em out.
        # (required, otherwise we get corrupt .dylibs on mac, see bug 15121)
        STRING(REGEX REPLACE "-Wl,-rpath,[^; ]*" "" ${PREFIX}_LDFLAGS "${${PREFIX}_LDFLAGS}")

        # FIXME: insulate ourselves from packages that use -pthread on osx; see bug 18328
        if ("${${PREFIX}_CFLAGS}" MATCHES "-pthread")
            # Alas, CMAKE_CXX_FLAGS is passed to linker when linking shared libraries
            # See also discussion at http://savannah.gnu.org/patch/?8186
            MESSAGE(STATUS "ObFindLibs: Replacing -pthread in ${PREFIX}_CFLAGS with -D_REENTRANT to avoid clang error when linking shared libraries")
            if (${PREFIX}_CFLAGS MATCHES "-D_REENTRANT")
                STRING(REPLACE "-pthread" "" ${PREFIX}_CFLAGS "${${PREFIX}_CFLAGS}")
            else()
                STRING(REPLACE "-pthread" "-D_REENTRANT" ${PREFIX}_CFLAGS "${${PREFIX}_CFLAGS}")
            endif()
        endif()
    endif()

    # gnutls.pc uses -R instead of -rpath on all platforms, as it's under the
    # mistaken impression that you'll be linking using 'libtool --mode=link gcc'.
    # cf. https://gitlab.com/gnutls/gnutls/issues/49
    # So filter those out, too, and hope nobody actually puts a .la in a .pc file,
    # as Gnome seems to do.
    STRING(REGEX REPLACE "[; ]-R[^; ]*" "" ${PREFIX}_LDFLAGS "${${PREFIX}_LDFLAGS}")

    if (WIN32)
        # Borrowed from ObGeneratePC.
        # Postprocess options to be more Visual C++ compatible.
        # Replace -isystem with -I
        STRING(REGEX REPLACE "-isystem *" "-I" ${PREFIX}_CFLAGS "${${PREFIX}_CFLAGS}")
        # Insulate -libpath from the -l transformation
        STRING(REPLACE "-libpath:" "-L" ${PREFIX}_LDFLAGS "${${PREFIX}_LDFLAGS}")
        # Visual C++ uses foo.lib rather than -lfoo
        STRING(REGEX REPLACE "-l([a-zA-Z0-9_+-.]+)" "\\1.lib" ${PREFIX}_LDFLAGS "${${PREFIX}_LDFLAGS}")
        # Visual C++ uses /libpath:foo rather than -Lfoo, but it'll accept -libpath instead of /libpath, thank goodness
        STRING(REPLACE "-L" "-libpath:" ${PREFIX}_LDFLAGS "${${PREFIX}_LDFLAGS}")
    else()
        # Insulate user from .pc files which use -I instead of -isystem,
        # and have warnings with recent compilers.
        STRING(REPLACE "-I" "-isystem " ${PREFIX}_CFLAGS "${${PREFIX}_CFLAGS}")
    endif()

    # Kludge: remove mysterious literal -L$libdir :-(
    # (Seen on osx1015)
    STRING(REPLACE "-L$libdir" "" ${PREFIX}_LDFLAGS "${${PREFIX}_LDFLAGS}")

    # pkgconfig and cmake aren't *quite* on speaking terms; translate a bit for them.
    STRING(REPLACE "-framework;Chromium;Embedded;Framework" "-framework 'Chromium Embedded Framework'" ${PREFIX}_LDFLAGS "${${PREFIX}_LDFLAGS}")
    IF(NOT WIN32)
        # with visual studio, translating semicolons to spaces in LDFLAGS causes link failure, see bug 15772
        STRING(REPLACE ";" " " ${PREFIX}_LDFLAGS "${${PREFIX}_LDFLAGS}")
    ENDIF()
    STRING(REPLACE ";" " " ${PREFIX}_CFLAGS "${${PREFIX}_CFLAGS}")

    # Remove leading and trailing whitespace, or cmake might think the whole thing is a library name, or abort.
    STRING(STRIP "${${PREFIX}_CFLAGS}" "${PREFIX}_CFLAGS")
    STRING(STRIP "${${PREFIX}_LDFLAGS}" "${PREFIX}_LDFLAGS")

    # Export output variable to parent scope
    SET(${PREFIX}_LDFLAGS "${${PREFIX}_LDFLAGS}" PARENT_SCOPE)
    SET(${PREFIX}_CFLAGS "${${PREFIX}_CFLAGS}" PARENT_SCOPE)
    SET(${PREFIX}_INCLUDE_DIRS "${${PREFIX}_INCLUDE_DIRS}" PARENT_SCOPE)

    # Freaky bit #2 to support CEF:
    # grab cef helper functions it needs
    if (NOT "${CEF_BRANCH}" STREQUAL "")
        # Assumes that ObBoilerplate.cmake, or somebody else, has set YOBUILD.
        IF (YOBUILD STREQUAL "")
            # FIXME: get yobuild from prefix variable in libcef${CEF_BRANCH}.pc instead?
            MESSAGE(FATAL_ERROR "ObFindLibs: bug: cef magic: please set YOBUILD and/or report this to community.oblong.com")
        ENDIF()
        INCLUDE("${YOBUILD}/lib/cmake/FindCEF${CEF_BRANCH}.cmake")
    ENDIF()

ENDFUNCTION()
