project(rubygem)

set(VERSION_XYZ "${MAJOR_VERSION}.${MINOR_VERSION}.${MICRO_VERSION}")

#------------- library -----------

add_custom_command(
   OUTPUT rubyPlasma-${VERSION_XYZ}.gem
   COMMAND sh ${rubygem_BINARY_DIR}/build.sh
)
add_custom_target(rubyPlasmaGem ALL DEPENDS rubyPlasma-${VERSION_XYZ}.gem)

#------------- install -----------

install (
  FILES ${rubygem_BINARY_DIR}/rubyPlasma-${VERSION_XYZ}.gem
  DESTINATION "${CMAKE_INSTALL_LIBDIR}/ruby"
)
