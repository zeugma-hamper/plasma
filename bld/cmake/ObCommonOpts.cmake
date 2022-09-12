#.rst:
# This file provides the function:
#  ObCommonOpts     - define standard options like CMAKE_BUILD_TYPE, ASAN, etc.
#
# Copyright (C) 2015-2017 Oblong Industries

# ObCommonOpts
# ------------
#
# Boilerplate cmake options for projects using g-speak
#
# Defines cmake commandline options settable with -Dfoo=bar:
#   ASAN                   - whether to use address sanitizer
#   BUILD_TESTS            - whether to build tests
#   CMAKE_BUILD_TYPE       - Debug, RelWithDebInfo, Release
#   CMAKE_VERBOSE_MAKEFILE - whether build should be verbose
#   COVERAGE               - whether to use code coverage
#   TSAN                   - whether to use thread sanitizer
#   USE_STATIC_G_SPEAK     - tell ObFindLibs to favor static g-speak libraries
# and sets the additional variables
#   OPTIMIZE_FLAGS      - compiler flags for CMAKE_BUILD_TYPE
#   COVERAGE_FLAGS      - compiler flags for COVERAGE
#   SANITIZER_FLAGS     - compiler flags for ASAN/TSAN
#   OB_G_SPEAK_LIB_TYPE - STATIC if USE_STATIC_G_SPEAK, else SHARED
#                        use when calling ObFindLibs to find g-speak.
#                        (Used to be named OB_PREFER_LIB_TYPE.)

FUNCTION(ObCommonOpts)
    OPTION(ASAN            "Enable Address Sanitizer"                  OFF)
    OPTION(BUILD_TESTS     "Build tests, too"                          ON)
    OPTION(CMAKE_VERBOSE_MAKEFILE "Verbose output from make"           OFF)
    OPTION(COVERAGE        "Generate coverage report after test run"   OFF)
    OPTION(TSAN            "Enable Thread Sanitizer (experimental)"    OFF)
    OPTION(USE_STATIC_G_SPEAK "tell ObFindLibs to favor static g-speak libraries" OFF)

    IF(DEFINED CMAKE_BUILD_TYPE AND (NOT ${CMAKE_BUILD_TYPE} STREQUAL "None"))
        # Somebody set it already, don't force it, just add the type.
        SET(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo")
    ELSE()
        # Nobody set it yet, give it a reasonable default.
        SET(CMAKE_BUILD_TYPE "RelWithDebInfo" CACHE STRING "Choose the type of build, options are: Debug Release RelWithDebInfo" FORCE)
    ENDIF()
    # Set the possible values of build type for cmake-gui
    SET_PROPERTY(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo")

    SET(OPTIMIZE_FLAGS )
    IF (NOT WIN32)
        IF (${CMAKE_BUILD_TYPE} STREQUAL "Release")
            SET(OPTIMIZE_FLAGS "-g0 -O3 -DNDEBUG")
        ELSEIF (${CMAKE_BUILD_TYPE} STREQUAL "Debug")
            IF (TSAN OR ASAN)
                # Sanitizers have overhead, compensate with a little optimization
                SET(OPTIMIZE_FLAGS "-g -O1 -UNDEBUG")
            ELSE()
                SET(OPTIMIZE_FLAGS "-g -O0 -UNDEBUG")
            ENDIF()
        ELSEIF (${CMAKE_BUILD_TYPE} STREQUAL "RelWithDebInfo")
            SET(OPTIMIZE_FLAGS "-g -O3 -UNDEBUG")
        ELSE()
            MESSAGE(FATAL_ERROR "ObCommonOpts: Unknown build type ${CMAKE_BUILD_TYPE}")
        ENDIF()
    ENDIF()

    SET(SANITIZER_FLAGS "")
    IF (ASAN AND TSAN)
        MESSAGE(FATAL "ObCommonOpts: ASAN and TSAN are mutually exclusive.")
    ENDIF()
    IF (ASAN)
        SET(SANITIZER_FLAGS "-fsanitize=address -fno-omit-frame-pointer")
    ELSEIF (TSAN)
        SET(SANITIZER_FLAGS "-fsanitize=thread -fno-omit-frame-pointer")
    ENDIF()

    SET(COVERAGE_FLAGS "")
    if (COVERAGE)
        FIND_PROGRAM( GCOV gcov )
        IF (NOT GCOV)
            MESSAGE(FATAL_ERROR "ObCommonOpts: COVERAGE selected, but gcov not found (part of compiler")
        ENDIF()
        FIND_PROGRAM( LCOV lcov )
        IF (NOT LCOV)
            MESSAGE(FATAL_ERROR "ObCommonOpts: COVERAGE selected, but lcov not found (package lcov)")
        ENDIF()
        SET(COVERAGE_FLAGS "--coverage")
    ENDIF ()

    # Export promised variables to parent scope
    # (Should we instead make them global, e.g. internal cache variables?)
    SET(COVERAGE_FLAGS  ${COVERAGE_FLAGS} PARENT_SCOPE)
    SET(SANITIZER_FLAGS ${SANITIZER_FLAGS} PARENT_SCOPE)
    SET(OPTIMIZE_FLAGS  ${OPTIMIZE_FLAGS} PARENT_SCOPE)
    IF(USE_STATIC_G_SPEAK)
        SET(OB_G_SPEAK_LIB_TYPE "STATIC" PARENT_SCOPE)
        SET(OB_PREFER_LIB_TYPE "STATIC" PARENT_SCOPE)   # remove once apps switch to OB_G_SPEAK_LIB_TYPE
    ELSE()
        SET(OB_G_SPEAK_LIB_TYPE "SHARED" PARENT_SCOPE)
        SET(OB_PREFER_LIB_TYPE "SHARED" PARENT_SCOPE)   # remove once apps switch to OB_G_SPEAK_LIB_TYPE
    ENDIF()

ENDFUNCTION()
