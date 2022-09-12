#.rst:
# This file provides the function:
#  ObGenerateProject - Generate a source project directory from a pair of template directories
#
# Copyright (C) 2018 Oblong Industries

# ObGenerateProject
# -----------------
#
# Usage: ObGenerateProject(TEMPLATE_DIR1 TEMPLATE_DIR2 TEMPLATE_DIR3 OUT_DIR PROJ_NAME)
#
# Like "obi new", but at cmake time, and handles two levels of template.
# Creates output directory ${OUT_DIR}/${PROJ_NAME}
# TEMPLATE_DIR1 is a very generic template (say, with truly common boilerplate)
# TEMPLATE_DIR2 specializes it somewhat (say, for a library)
# TEMPLATE_DIR3 is a specialized subtemplate (e.g. for the samples for that library)

function(ObGenerateProject TEMPLATE_DIR1 TEMPLATE_DIR2 TEMPLATE_DIR3 OUT_DIR PROJ_NAME)
  file(GLOB_RECURSE files RELATIVE ${TEMPLATE_DIR1} ${TEMPLATE_DIR1}/*)
  foreach(file ${files})
    if (EXISTS ${TEMPLATE_DIR2}/${file} OR EXISTS ${TEMPLATE_DIR3}/${file})
        continue()
    endif()
    ObGenerateProjectFile(${TEMPLATE_DIR1}/${file} ${OUT_DIR}/${file})
  endforeach()

  file(GLOB_RECURSE files RELATIVE ${TEMPLATE_DIR2} ${TEMPLATE_DIR2}/*)
  foreach(file ${files})
    if (EXISTS ${TEMPLATE_DIR3}/${file})
        continue()
    endif()
    ObGenerateProjectFile(${TEMPLATE_DIR2}/${file} ${OUT_DIR}/${file})
  endforeach()

  file(GLOB_RECURSE files RELATIVE ${TEMPLATE_DIR3} ${TEMPLATE_DIR3}/*)
  foreach(file ${files})
    ObGenerateProjectFile(${TEMPLATE_DIR3}/${file} ${OUT_DIR}/${file})
  endforeach()

  # The above isn't quite enough in the --greenhouse, --asan, or --tsan
  # cases, so use ob-set-defaults to get it right.
  set(opts "")
  if (ASAN)
    LIST(APPEND opts "--asan")
  endif()
  if (USE_STATIC_G_SPEAK)
    LIST(APPEND opts "--greenhouse")
  endif()
  if (TSAN)
    LIST(APPEND opts "--tsan")
  endif()
  if (NOT "${opts}" STREQUAL "")
    MESSAGE("ObGenerateProject: running ob-set-defaults ${opts} in ${OUT_DIR}")
    execute_process(
      COMMAND ob-set-defaults ${opts}
      WORKING_DIRECTORY "${OUT_DIR}"
    )
  endif()

endfunction()

# Create the directory to hold ${file}, if needed
function(ObEnsureDirectory file)
  get_filename_component(dir ${file} DIRECTORY)
  file(MAKE_DIRECTORY ${dir})
endfunction()

# Given an input file path and and output file path,
# read input file, adjust output path and contents, and create output file.
function(ObGenerateProjectFile IN_FILE OUT_FILE)
  # Adjust output path, make sure parent directory exists
  string(REPLACE "@PROJECT@" "${PROJ_NAME}" OUT_FILE "${OUT_FILE}")
  string(REPLACE "@G_SPEAK_XY@" "${G_SPEAK_XY}" OUT_FILE "${OUT_FILE}")
  ObEnsureDirectory(${OUT_FILE})

  # FIXME: use @var@ instead of {{var}} so we can use configure_file,
  # because then we can drop the hardcoding of variables, and
  # ninja will rerun cmake and regenerate the file if the template is edited.
  # e.g.
  # configure_file(${IN_FILE} ${OUT_FILE} @ONLY)
  # Alas, one of our goals is to use the same template files as obi, so that's hard.
  file(READ ${IN_FILE} CONTENTS)
  string(REPLACE "{{project_name}}" "${PROJ_NAME}" CONTENTS "${CONTENTS}")
  string(REPLACE "{{g_speak_version}}" "${G_SPEAK_XY}" CONTENTS "${CONTENTS}")
  string(REPLACE "{{cef_branch}}" "cef${CEF_BRANCH}" CONTENTS "${CONTENTS}")
  string(REPLACE "{{yobuild_major}}" "${YOVERSION}" CONTENTS "${CONTENTS}")
  string(REPLACE "{{yobuild}}" "${YOBUILD}" CONTENTS "${CONTENTS}")
  string(REPLACE "{{G_SPEAK_HOME}}" "${G_SPEAK_HOME}" CONTENTS "${CONTENTS}")
  # Curse you, debian/install and snapcraft.yaml/fileset!
  # See https://forum.snapcraft.io/t/fileset-syntax-preserves-an-ancient-papercut/6930
  STRING(REGEX REPLACE "^/" "" NOSLASH_YOBUILD "${YOBUILD}")
  STRING(REGEX REPLACE "^/" "" NOSLASH_G_SPEAK_HOME "${G_SPEAK_HOME}")
  STRING(REPLACE "{{NOSLASH_YOBUILD}}" "${NOSLASH_YOBUILD}" CONTENTS "${CONTENTS}")
  STRING(REPLACE "{{NOSLASH_G_SPEAK_HOME}}" "${NOSLASH_G_SPEAK_HOME}" CONTENTS "${CONTENTS}")
  if (WIN32)
    # CMake's FILE WRITE uses DOS line ending on DOS, which breaks
    # ob-set-defaults.conf, and there's no option to disable it...
    # (well, we could convert to hex, that would do it, but that sounds hard)
    # so we have to postprocess the output file.
    # We could use sed if we had it on windows, but we don't.
    # So abuse configure_file.  Let's hope that is gentle.
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/ObGenerateProjectFile.tmp" "${CONTENTS}")
    configure_file("${CMAKE_CURRENT_BINARY_DIR}/ObGenerateProjectFile.tmp" "${OUT_FILE}" @ONLY NEWLINE_STYLE UNIX)
    file(REMOVE "${CMAKE_CURRENT_BINARY_DIR}/ObGenerateProjectFile.tmp")
  else()
    file(WRITE ${OUT_FILE} "${CONTENTS}")
  endif()
endfunction()

