#.rst:
# This file provides the functions:
#  ObGetCefBranch     - retrieve CEF branch used by WebThing
#  ObGetGspeakVersion - Get X.Y version of the g-speak current project is to be built against
#  ObGetVersionGit    - version number of current project from git describe
#  ObGetYobuild       - retrieve path to Oblong dependencies directory
#  ObGetYoversion     - get major version of yobuild
#  ObRunObs           - run obs with given arguments and save output
#
# Copyright (C) 2015-2018 Oblong Industries

FIND_PACKAGE(PkgConfig REQUIRED)

# ObRunObs
# --------------
#
# Syntax:
# ObRunObs(<OUTVAR> CMD ...)
# Output variables:
#   OUTVAR - set to output of obs running given command

FUNCTION(ObRunObs OUTVAR)
    IF(WIN32)
        # Have to look on path since it might have been installed into $G_SPEAK_HOME/bin
        FIND_PROGRAM(OBS obs HINTS "C:/cygwin64/bin" "C:/cygwin/bin")
        EXECUTE_PROCESS(
            COMMAND C:/cygwin64/bin/sh.exe "${OBS}" ${ARGN}
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            RESULT_VARIABLE STATUS
            OUTPUT_VARIABLE ${OUTVAR}
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        IF (NOT "${STATUS}" EQUAL "0")
            MESSAGE(FATAL_ERROR "ObRunObs: command 'C:/cygwin64/bin/sh.exe /bin/obs ${ARGN}' failed, STATUS is ${STATUS}, output is ${OUTVAR}")
        ENDIF()
    ELSE()
        EXECUTE_PROCESS(
            COMMAND obs ${ARGN}
            WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
            RESULT_VARIABLE STATUS
            OUTPUT_VARIABLE ${OUTVAR}
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        IF (NOT "${STATUS}" EQUAL "0")
            MESSAGE(FATAL_ERROR "ObRunObs: command 'obs ${ARGN}' failed, STATUS is ${STATUS}, output is ${OUTVAR}")
        ENDIF()
    ENDIF()
    SET(${OUTVAR} ${${OUTVAR}} PARENT_SCOPE)
ENDFUNCTION()

# ObGetCefBranch
# --------------
#
# Syntax:
# ObGetCefBranch(<OUTVAR>)
# Output variables:
#   OUTVAR - set to e.g. 3202 if current project depends on cef 3202

FUNCTION(ObGetCefBranch OUTVAR)
    # Just ask obs (which just looks in source tree).
    # Pass it G_SPEAK_HOME so it can fall back to the default cef for
    # that g-speak if it feels like it, as get-yobuild-home does.
    ObRunObs(${OUTVAR} get-cef-version "${G_SPEAK_HOME}")

    # Strip leading 'cef' (a kludge that made sense a long time ago).
    STRING(REGEX REPLACE "^cef" "" ${OUTVAR} ${${OUTVAR}})

    SET(${OUTVAR} ${${OUTVAR}} PARENT_SCOPE)
ENDFUNCTION()

# ObGetGspeakVersion
# --------------
#
# Syntax:
# ObGetGspeakVersion(<OUTVAR>)
# Output variables:
#   OUTVAR - Get X.Y version of the g-speak current project is to be built against
# Before calling, add G_SPEAK_HOME to CMAKE_PREFIX_PATH.

FUNCTION(ObGetGspeakVersion OUTVAR)
    ObRunObs(${OUTVAR} get-gspeak-version "${G_SPEAK_HOME}")
    SET(${OUTVAR} ${${OUTVAR}} PARENT_SCOPE)
ENDFUNCTION()

# ObGetYobuild
# --------------
#
# Syntax:
# ObGetYobuild(<OUTVAR>)
# Output variables:
#   OUTVAR - prefix of Oblong's dependencies directory
# Before calling, add G_SPEAK_HOME to CMAKE_PREFIX_PATH.

FUNCTION(ObGetYobuild OUTVAR)
    # Just ask obs (which looks in source tree and asks ob-version,
    # and falls back to default for given g-speak if that fails)
    ObRunObs(${OUTVAR} get-yobuild-home "${G_SPEAK_HOME}")
    IF (WIN32)
      IF (${OUTVAR} MATCHES "^/cygdrive")
        MESSAGE("ObGetYobuild: converting /cygdrive/* to *:")
        # FIXME
        STRING(REGEX REPLACE "/cygdrive/(.)/*" "\\1:/" ${OUTVAR} "${${OUTVAR}}")
      ELSEIF (NOT ${OUTVAR} MATCHES "^(.:|\\\\)")
        MESSAGE("ObGetYobuild: adding missing drive letter")
        # On Windows, must contain a drive letter, else yovo tests fail
        # because putting /opt/blah in PATH doesn't work properly.
        # FIXME
        SET(${OUTVAR} "C:/${${OUTVAR}}")
      ENDIF()
    ENDIF()
    SET(${OUTVAR} ${${OUTVAR}} PARENT_SCOPE)
ENDFUNCTION()

# ObGetYoversion
# --------------
#
# Syntax:
# ObGetYoversion(<OUTVAR>)
# Output variables:
#   OUTVAR - major version number of yobuild (usualy last number in ObGetYobuild output)
# Before calling, add G_SPEAK_HOME to CMAKE_PREFIX_PATH.

FUNCTION(ObGetYoversion OUTVAR)
    # Just ask obs (which looks in source tree and asks ob-version,
    # and falls back to default for given g-speak if that fails)
    ObRunObs(${OUTVAR} get-yoversion "${G_SPEAK_HOME}")
    SET(${OUTVAR} ${${OUTVAR}} PARENT_SCOPE)
ENDFUNCTION()

# ObGetVersionGit
# --------------
#
# Syntax:
# ObGetVersionGit(<OUTVAR>)
# Output variables:
#   OUTVAR - version number of current project from git describe

FUNCTION(ObGetVersionGit OUTVAR)
    ObRunObs(${OUTVAR} get-version-git)
    SET(${OUTVAR} ${${OUTVAR}} PARENT_SCOPE)
ENDFUNCTION()
