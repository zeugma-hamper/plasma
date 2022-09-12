configure_file(
  input: 'ob-vers-gen.h.in',
  output: 'ob-vers-gen.h',
  configuration: cdata)

ob_vers_gen_h = meson.current_build_dir() / 'ob-vers-gen.h'

loam_c_flags = c_flags + ['-DQUOTED_PREFIX="' + get_option('prefix') + '"', ]

loam_c_sources = [
  'ob-atomic.c',
  'ob-dirs.c',
  'ob-file.c',
  'ob-log.c',
  'ob-math.c',
  'ob-rand.c',
  'ob-retorts.c',
  'ob-string.c',
  'ob-thread.c',
  'ob-time.c',
  'ob-truly-rand.c',
  'ob-util.c',
  'ob-vers.c',
  'datadir.c',
  'dSFMT.c',
  'prefix.c',
]

if cc.get_id() == 'msvc'
  loam_c_sources += [
    'win32/ob-sys-win32.c',
    'win32/ob-sys-win32-cpp.cpp',
    'ob-hash.c',
    'ob-hash-city.c',
  ]
elif target_machine.cpu_family() == 'x86_64'
  loam_c_sources += [
    'ob-hash-64.S',
  ]
elif target_machine.cpu_family() == 'x86'
  loam_c_sources += [
    'ob-hash-32.S',
  ]
else
  loam_c_sources += [
    'ob-hash.c',
    'ob-hash-city.c',
  ]
endif

loam_c_headers = [
  'ob-api.h',
  'ob-atomic.h',
  'ob-attrs.h',
  'ob-coretypes.h',
  'ob-dirs.h',
  'ob-endian.h',
  'ob-file.h',
  'ob-geom.h',
  'ob-hash.h',
  'ob-log.h',
  'ob-math.h',
  'ob-mvtypes.h',
  'ob-pthread.h',
  'ob-rand.h',
  'ob-retorts.h',
  'ob-string.h',
  'ob-sys.h',
  'ob-thread.h',
  'ob-time.h',
  'ob-types.h',
  'ob-util.h',
  ob_vers_gen_h,
  'ob-vers.h',
]

loam_c_incs = include_directories(
  '.',     # so tests/test-version.c can find ob-vers-gen.h
  '../..',
  )

loam_c_deps = [thread_dep, m_dep]

loam_c_link_args = []
if build_machine.system() == 'windows'
  loam_c_link_args += [ 'ws2_32.lib' ]
endif

loam_c = library(
   'Loam',
   loam_c_sources,
   c_args : loam_c_flags + ['-DOB_LOAM_EXPORTS'],
   link_args : loam_c_link_args,
   cpp_args : loam_c_flags + ['-DOB_LOAM_EXPORTS'],
   dependencies : loam_c_deps,
   include_directories : loam_c_incs,
   install: true,
   install_rpath: ob_rpath,
   version: '2',
)

pkg.generate(
  loam_c,
  filebase: 'libLoam',
  libraries: [ob_rpath_flags, thread_dep],

  # We shouldn't ever have to specify -I$YOBUILD/include; that
  # should always come organically from a .pc file in $YOBUILD.
  # But without this kludge, rtsp-viddle-server fails to find our glib.h.
  # The proper fix is probably
  # a) RTSPVideoConfig.h should include glib.h rather than glib-2.0/glib.h
  # b) yobuild's gstreamer-rtsp-server-1.0.pc should Require glib-2.0
  extra_cflags: '-I' + YOBUILD + '/include',
)

install_headers(loam_c_headers, subdir: 'libLoam/c')

if build_machine.system() == 'windows'
  install_headers(
    ['win32/ob-pthread-win32.h', 'win32/ob-sys-win32.h'],
    subdir: 'libLoam/c/win32'
  )
endif

#------------- executables -----------

ob_version = executable(
   'ob-version', 'ob-version.c', loam_c_headers,
   c_args : c_flags,
   link_with : [loam_c],
   include_directories : loam_c_incs,
   install: true,
   install_rpath: ob_rpath,
)

ob_which = executable(
   'ob-which', 'ob-which.c', loam_c_headers,
   c_args : c_flags,
   link_with : [loam_c],
   include_directories : loam_c_incs,
   install: true,
   install_rpath: ob_rpath,
)
if build_tests
  subdir('tests')
endif
