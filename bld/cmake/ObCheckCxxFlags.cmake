#.rst:
# This file provides the function:
#  ObCheckCxxFlags - filter unsupported compiler flags
#
# Copyright (C) 2015-2017 Oblong Industries

# ObCheckCxxFlags
# ---------------
#
# Syntax:
# ObCheckCxxFlags(<OUTVAR> flag...)
# Output variables:
#   OUTVAR - the subset of the given flags the C++ compiler supports
#
# More convenient than CHECK_CXX_COMPILER_FLAG.

INCLUDE(CheckCXXCompilerFlag)

FUNCTION(ObCheckCxxFlags RESULTVAR)
    FOREACH(flag ${ARGN})
       STRING(REGEX REPLACE "[^a-zA-Z0-9_]" _ SANITIZED_FLAG ${flag})
       SET(FLAGFLAG "HAS_CXXFLAG${SANITIZED_FLAG}")
       CHECK_CXX_COMPILER_FLAG(${flag} ${FLAGFLAG})
       IF (${${FLAGFLAG}})
          SET(${RESULTVAR} "${${RESULTVAR}} ${flag}" PARENT_SCOPE)
          SET(${RESULTVAR} "${${RESULTVAR}} ${flag}")
       ENDIF()
    ENDFOREACH()
ENDFUNCTION()
