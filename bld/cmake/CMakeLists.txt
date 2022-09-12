# This is the last subdirectory included by the
# top level CMakeLists.txt, so the tests it
# adds come last.

# FindGSpeak.cmake is a legacy file; new apps should use samples/FindOblong.cmake
configure_file(FindGSpeak.cmake.in "${g-speak_BINARY_DIR}/FindGSpeak.cmake" @ONLY)

install(
  FILES ${g-speak_BINARY_DIR}/FindGSpeak.cmake
        ObCheckCFlags.cmake
        ObCheckCxxFlags.cmake
        ObBoilerplate.cmake
        ObCommonOpts.cmake
        ObFindLibs.cmake
        ObGeneratePC.cmake
        ObGenerateProject.cmake
        ObGetSettings.cmake
  DESTINATION lib/cmake
)

#---- begin debian alternatives magic for cmake ----
# When cmake sees 'find_package(Oblong)', it evaluates
# ../lib/cmake/Oblong/OblongConfig.cmake 
# from each directory in PATH.  
# In Ubuntu, we register symlinks in /usr/bin for peek, poke, etc., and
# a symlink in /usr/lib/cmake for OblongConfig.cmake.
# See ../../debian/oblong-loam4.3.postinst
# OblongConfig.cmake just includes $G_SPEAK_HOME/src/FindOblong.cmake.
configure_file(Oblong/OblongConfig.cmake.in "${g-speak_BINARY_DIR}/OblongConfig.cmake" @ONLY)
install(
  FILES ${g-speak_BINARY_DIR}/OblongConfig.cmake
  DESTINATION lib/cmake/Oblong
)
#---- end debian alternatives magic for cmake ----

IF(EXISTS "${g-speak_SOURCE_DIR}/.git/config")
  # This test should be the last one run, so it can check that configure,
  # build, and test did not leave files behind that are not ignored by
  # .gitignore.
  add_test(
    NAME diffGitStatusTest
    COMMAND "${SHELL}" "${g-speak_SOURCE_DIR}/bld/cmake/diff-git-status-test.sh" "test" "${g-speak_BINARY_DIR}"
    WORKING_DIRECTORY "${g-speak_SOURCE_DIR}"
  )
ENDIF()

