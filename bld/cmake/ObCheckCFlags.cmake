#.rst:
# This file provides the function:
#  ObCheckCFlags - filter unsupported compiler flags
#
# Copyright (C) 2015-2018 Oblong Industries

# ObCheckCFlags
# ---------------
#
# Syntax:
# ObCheckCFlags(<OUTVAR> flag...)
# Output variables:
#   OUTVAR - the subset of the given flags the C compiler supports
#
# More convenient than CHECK_C_COMPILER_FLAG.

INCLUDE(CheckCCompilerFlag)

FUNCTION(ObCheckCFlags RESULTVAR)
    FOREACH(flag ${ARGN})
       STRING(REGEX REPLACE "[^a-zA-Z0-9_]" _ SANITIZED_FLAG ${flag})
       SET(FLAGFLAG "HAS_CFLAG${SANITIZED_FLAG}")
       CHECK_C_COMPILER_FLAG(${flag} ${FLAGFLAG})
       IF (${${FLAGFLAG}})
          SET(${RESULTVAR} "${${RESULTVAR}} ${flag}" PARENT_SCOPE)
          SET(${RESULTVAR} "${${RESULTVAR}} ${flag}")
       ENDIF()
    ENDFOREACH()
ENDFUNCTION()
