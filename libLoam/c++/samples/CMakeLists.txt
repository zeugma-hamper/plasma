set (
  samples

  canary
)

#-------------- Build locally

foreach (PROJ_NAME ${samples})
  add_executable (${PROJ_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_NAME}/src/${PROJ_NAME}.cpp)
  SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES COMPILE_FLAGS "${Loam++_CFLAGS}")
  target_link_libraries (${PROJ_NAME} Loam++ "${Loam++_LINK_LIBS}")
endforeach()

# Special case:
# canary is also a runnable test; all it does is verify
# that it can find its shared libraries, and that unicode strings work.
if (BUILD_TESTS)
  # Have to wrap the test so it can find its libraries when running on windows
  add_wrapped_test(canary canary "local")
endif()

#-------------- Generate sample directories with obi, then drop in speciaized source

foreach (PROJ_NAME ${samples})

  set(OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/${PROJ_NAME})
  ObGenerateProject(${TOP_TEMPLATE} ${CMAKE_CURRENT_SOURCE_DIR}/obi-template ${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_NAME} ${OUT_DIR} ${PROJ_NAME})
endforeach ()

#-------------- Install samples

install (
    FILES Makefile
    DESTINATION "samples/libLoam++"
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ
)

foreach (sample ${samples})
  install (
    DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated/${sample}"
    DESTINATION "samples/libLoam++"
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    PATTERN "*.in" EXCLUDE
    PATTERN ".gitignore" EXCLUDE
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ
  )
endforeach ()
