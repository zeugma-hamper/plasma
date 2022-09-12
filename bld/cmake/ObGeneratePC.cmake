#.rst:
# This file provides the function:
#  ObGeneratePC     - Generate pkgconfig (.pc) files for use with ObFindLibs
#
# Copyright (C) 2018 Oblong Industries

# ObGeneratePC
# ------------
#
# Options:
# NAME:             name of the library and its .pc file (without suffix)
# VERSION:          version number of the library
# REQUIRES:         list of pkgconfig modules this depends on
# LIBS:             list of linker flags for this library
# CFLAGS:           space separated list of compiler options
# EXTRA:            unformatted text, e.g. variable definitions, to add to the pc file
# REQUIRES.PRIVATE: list of additional pkgconfig modules for --static
# LIBS.PRIVATE:     list of additional linker flags for --static
# CFLAGS.PRIVATE:   list of additional compiler options for --static
# e.g.
#   ObGeneratePC(NAME libLoam LIBS "$(ICU_LIBS}" CFLAGS "$(ICU_CPPFLAGS} $(BOOST_CPPFLAGS}" EXTRA "cefbranch=${CEF_BRANCH}")
# See https://people.freedesktop.org/~dbn/pkg-config-guide.html

function(ObGeneratePC)
  set(options )
  set(oneValueArgs NAME DESCRIPTION VERSION EXTRA)
  set(multiValueArgs REQUIRES LIBS CFLAGS REQUIRES.PRIVATE LIBS.PRIVATE CFLAGS.PRIVATE)
  cmake_parse_arguments(PC "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  # _libs is in cmake format (semicolon-separated, lacking -l's),
  # but we need gcc commandline format (space separated, with -l's).
  # So convert _libs into new string _libs_space_sep.
  #
  # Replace spaces with semicolons so we can iterate over it
  STRING(REPLACE " " ";" _libs_list "${PC_LIBS}" )
  # But -framework takes an arg
  STRING(REPLACE "-framework;" "-framework " _libs_list2 "${_libs_list}" )
  # If a library doesn't start with / or -, throw a hail mary and prefix it with -l
  # *deep breath*
  # On Mac, libtool will ignore /absolute/path/to/libfoo.dylib sometimes,
  # so translate those to -L/absolute/path/to -lfoo
  # and hope.
  set(_libs_space_sep "")
  foreach(lib ${_libs_list2})
    if (lib MATCHES "^/.*lib[^/]*dylib")
      GET_FILENAME_COMPONENT(libpath ${lib} DIRECTORY)
      GET_FILENAME_COMPONENT(libname ${lib} NAME)
      STRING(REGEX REPLACE "^lib" "" libshortname ${libname})
      STRING(REGEX REPLACE ".dylib$" "" libshortername ${libshortname})
      message("Achtung!  To work around libtool problem on mac, using -L${libpath} -l${libshortername} instead of ${lib} in ${PC_NAME}.pc")
      set(_libs_space_sep "${_libs_space_sep} -L${libpath} -l${libshortername}")
    elseif (lib MATCHES "^/usr/lib/x86_64-linux-gnu/libdl.so")
      message("Achtung!  To work around cmake problem, using -l form of libdl in ${PC_NAME}.pc")
      set(_libs_space_sep "${_libs_space_sep} -ldl")
    elseif (lib MATCHES "^/usr/lib/x86_64-linux-gnu/librt.so")
      message("Achtung!  To work around cmake problem, using -l form of librt in ${PC_NAME}.pc")
      set(_libs_space_sep "${_libs_space_sep} -lrt")
    elseif (lib MATCHES "^/usr/lib/x86_64-linux-gnu/libm.so")
      message("Achtung!  To work around cmake problem, using -l form of libm in ${PC_NAME}.pc")
      set(_libs_space_sep "${_libs_space_sep} -lm")
    elseif (lib MATCHES "^[/-]" OR lib MATCHES "^[Cc]:/")
      # Normal case: absolute paths and options work, so allow them.
      set(_libs_space_sep "${_libs_space_sep} ${lib}")
    else()
      set(_libs_space_sep "${_libs_space_sep} -l${lib}")
    endif()
  endforeach()
  SET(PC_LIBS "${_libs_space_sep}")

  # On Ubuntu 20.04, one sees
  #  -- Found Boost: /usr/lib/x86_64-linux-gnu/cmake/Boost-1.71.0/BoostConfig.cmake (found version "1.71.0") found components: filesystem regex system
  #  Boost_LIBRARIES is Boost::filesystem;Boost::regex;Boost::system
  # instead of the expected
  #  -- Boost version: 1.65.1
  #  -- Found the following Boost libraries:
  #  --   filesystem
  #  --   regex
  #  --   system
  #  Boost_LIBRARIES is -lboost_filesystem;-lboost_regex;-lboost_system
  # and the former is simply not appropriate for use in .pc files.
  # Translate back to sanity.
  # FIXME: do this right?
  STRING(REGEX REPLACE "Boost::filesystem" "boost_filesystem" PC_LIBS "${PC_LIBS}")
  STRING(REGEX REPLACE "Boost::regex" "boost_regex" PC_LIBS "${PC_LIBS}")
  STRING(REGEX REPLACE "Boost::system" "boost_system" PC_LIBS "${PC_LIBS}")

  IF (WIN32)
    # Postprocess options to be more Visual C++ compatible
    # Replace -isystem with -I
    STRING(REGEX REPLACE "-isystem *" "-I" PC_CFLAGS "${PC_CFLAGS}")
    # Remove rpath options
    STRING(REGEX REPLACE "-Wl,-rpath,[^ ]*" "" PC_LIBS "${PC_LIBS}")
    # Insulate -libpath from the -l transformation
    STRING(REPLACE "-libpath:" "-L" PC_LIBS "${PC_LIBS}")
    # Visual C++ uses foo.lib rather than -lfoo
    STRING(REGEX REPLACE "-l([a-zA-Z0-9_+-.]+)" "\\1.lib" PC_LIBS "${PC_LIBS}")
    # Somehow this happened:
    # Libs:  Loam++.lib -libpath:C://opt/oblong/deps-64-12/lib icuin.lib.lib icuuc.lib.lib
    STRING(REPLACE ".lib.lib" ".lib" PC_LIBS "${PC_LIBS}")
    # Visual C++ uses /libpath:foo rather than -Lfoo, but it'll accept -libpath instead of /libpath, thank goodness
    STRING(REPLACE "-L" "-libpath:" PC_LIBS "${PC_LIBS}")
  ENDIF()

  # Be kind to pkgconfig until all our users have one that handles -isystem
  STRING(REGEX REPLACE "-isystem *" "-I" PC_CFLAGS "${PC_CFLAGS}")

  # Replace semicolons (cmake's list separator) with spaces in multivalued options
  STRING(REPLACE ";" " " PC_CFLAGS "${PC_CFLAGS}" )
  STRING(REPLACE ";" " " PC_CFLAGS.PRIVATE "${PC_CFLAGS.PRIVATE}" )
  # FIXME: process LIBS.PRIVATE like we do LIBS
  STRING(REPLACE ";" " " PC_LIBS.PRIVATE "${PC_LIBS.PRIVATE}" )
  STRING(REPLACE ";" " " PC_REQUIRES "${PC_REQUIRES}" )
  STRING(REPLACE ";" " " PC_REQUIRES.PRIVATE "${PC_REQUIRES.PRIVATE}" )

  FILE(WRITE "${CMAKE_BINARY_DIR}/lib/pkgconfig/${PC_NAME}.pc"
"prefix=${CMAKE_INSTALL_PREFIX}
exec_prefix=\${prefix}
libdir=\${prefix}/lib
includedir=\${prefix}/include
${PC_EXTRA}

Name: ${PC_NAME}
Description: ${PC_DESCRIPTION}
Version: ${PC_VERSION}
Requires: ${PC_REQUIRES}
Libs: ${PC_LIBS}
Cflags: ${PC_CFLAGS}
Requires.private: ${PC_REQUIRES.PRIVATE}
Libs.private: ${PC_LIBS.PRIVATE}
Cflags.private: ${PC_CFLAGS.PRIVATE}
")

endfunction()
