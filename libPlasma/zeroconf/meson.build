plasma_zeroconf_sources = [
  'zeroconf-utils.c',
  'PoolServer.cpp',
  'Zeroconf.cpp',
]

if build_machine.system() == 'darwin'
  plasma_zeroconf_sources += [
    'zeroconf-client-bonjour.c',
    'zeroconf-server-bonjour.c',
  ]
elif build_machine.system() == 'linux'
  plasma_zeroconf_sources += [
    'zeroconf-client-avahi.c',
    'zeroconf-server-avahi.c',
  ]
endif

plasma_zeroconf_headers = [
  'PoolServer.h',
  'Zeroconf.h',
]

plasma_zeroconf_incs = include_directories('../..')

plasma_zeroconf_deps = [icu_uc_dep, thread_dep, boost_dep]
if build_machine.system() == 'linux'
  avahi_client_dep = dependency('avahi-client')
  plasma_zeroconf_deps += [
    avahi_client_dep,
  ]
endif

plasma_zeroconf = library(
   'PlasmaZeroconf',
   plasma_zeroconf_sources,
   c_args : [c_flags],
   dependencies : plasma_zeroconf_deps,
   link_with : [plasma_cpp, plasma_c, loam_cpp, loam_c],
   include_directories : plasma_zeroconf_incs,
   install: true,
   install_rpath: ob_rpath,
   version: '2',
)

install_headers(plasma_zeroconf_headers, subdir: 'libPlasma/zeroconf')

pkg.generate(
  plasma_zeroconf,
  requires: [plasma_cpp, plasma_c, loam_cpp, loam_c],
  filebase: 'libPlasmaZeroconf',
)

#------------- executables -----------

plasma_zeroconf_programs = [
  'pool-server-zeroconf-adapter',
]
plasma_zeroconf_executables = []

foreach p : plasma_zeroconf_programs
   plasma_executables += executable(
     p, p + '.cpp',
     cpp_args : cpp_flags,
     link_with : [plasma_zeroconf, plasma_cpp, plasma_c, loam_cpp, loam_c],
     dependencies : plasma_zeroconf_deps,
     include_directories : plasma_zeroconf_incs,
     install: true,
     install_rpath: ob_rpath,
   )
endforeach

if build_tests
  subdir('tests')
endif
