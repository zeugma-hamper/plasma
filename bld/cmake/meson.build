configure_file(
  input: 'FindGSpeak.cmake.in',
  output: 'FindGSpeak.cmake',
  configuration: cdata,
  install: true,
  install_dir : 'lib/cmake'
)

#---- begin cmake magic ----
# When cmake sees 'find_package(Oblong)', it evaluates
# ../lib/cmake/Oblong/OblongConfig.cmake
# from each directory in PATH.
# In Ubuntu, we register symlinks in /usr/bin for peek, poke, etc., and
# a symlink in /usr/lib/cmake for OblongConfig.cmake.
# See ../../debian/oblong-loam4.3.postinst
# OblongConfig.cmake just includes $G_SPEAK_HOME/src/FindOblong.cmake.
configure_file(
  input: 'Oblong/OblongConfig.cmake.in',
  output: 'OblongConfig.cmake',
  configuration: cdata,
  install: true,
  install_dir : 'lib/cmake/Oblong'
)
#---- end cmake magic ----

cmake_sources = [
  'ObCheckCFlags.cmake',
  'ObCheckCxxFlags.cmake',
  'ObBoilerplate.cmake',
  'ObCommonOpts.cmake',
  'ObFindLibs.cmake',
  'ObGeneratePC.cmake',
  'ObGenerateProject.cmake',
  'ObGetSettings.cmake',
]

install_data(
  cmake_sources,
  install_dir : 'lib/cmake'
)

if get_option('b_sanitize') == 'address' or build_machine.system() == 'linux'
  # Needed for running e.g. ruby or python that loads a native g-speak plugin
  # Install unconditionally on Linux because conditional debian packaging is... hard.
  install_data(
    'asanpreload',
    install_dir: 'bin',
    install_mode: 'r-xr-xr-x',
  )
endif
