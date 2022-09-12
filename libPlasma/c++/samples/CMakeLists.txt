set (
  samples

  p-eek
)

#-------------- Build locally

foreach (PROJ_NAME ${samples})
  add_executable (${PROJ_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_NAME}/src/${PROJ_NAME}.cpp)
  SET_TARGET_PROPERTIES(${PROJ_NAME} PROPERTIES COMPILE_FLAGS "${Plasma++_CFLAGS}")
  target_link_libraries (${PROJ_NAME} Plasma++ "${Plasma++_LINK_LIBS}")
endforeach()

#-------------- Generate sample directories with obi, then drop in speciaized source

foreach (PROJ_NAME ${samples})
  set(OUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated/${PROJ_NAME})
  ObGenerateProject(${TOP_TEMPLATE} ${CMAKE_CURRENT_SOURCE_DIR}/obi-template ${CMAKE_CURRENT_SOURCE_DIR}/${PROJ_NAME} ${OUT_DIR} ${PROJ_NAME})
endforeach ()

#-------------- Install samples

install (
    FILES Makefile
    DESTINATION "samples/libPlasma++"
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ
)

foreach (sample ${samples})
  install (
    DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/generated/${sample}"
    DESTINATION "samples/libPlasma++"
    DIRECTORY_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_WRITE GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
    PATTERN "*.in" EXCLUDE
    PATTERN ".gitignore" EXCLUDE
    PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ GROUP_WRITE WORLD_READ
  )
endforeach ()
