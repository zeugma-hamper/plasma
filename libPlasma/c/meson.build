plasma_c_sources = [
  'eintr-helper.c',
  'plasma-util.c',
  'pool.c',
  'pool_cmd.c',
  'pool-context.c',
  'pool-fifo.c',
  'pool-log.c',
  'pool_mmap.c',
  'pool-mmap-header.c',
  'pool_multi.c',
  'pool_net.c',
  'pool_options.c',
  'pool-portable.c',
  'pool_tcp.c',
  'pool-toc.c',
  'protein.c',
  'slaw.c',
  'slaw-coerce.c',
  'slaw-concat.c',
  'slaw-interop.c',
  'slaw-io.c',
  'slaw-io-convenience.c',
  'slaw-io-file.c',
  'slaw-numerics.c',
  'slaw-ordering.c',
  'slaw-path.c',
  'slaw-string.c',
  'slaw-walk.c',
  'slaw-yaml.c',
  'ossl/ossl-client.c',
  'ossl/ossl-common.c',
  'ossl/ossl-eintr-helper.c',
  'ossl/ossl-io.c',
  'ossl/ossl-locking.c',
  'ossl/ossl-server.c',
  'ossl/ossl-validation.c',
]

if not meson.is_unity()
  plasma_c_sources += [
    'slaw-v1.c',
  ]
endif
if cc.get_id() == 'msvc'
  plasma_c_sources += [
    'win32/fifo_ops_win32.c',
    'win32/sem_ops_win32.c',
  ]
else
  plasma_c_sources += [
    'fifo_ops.c',
    'pool-flock-ops.c',
    'pool-lock-ops.c',
    'sem_ops.c',
  ]
endif

plasma_c_headers = [
  'c-plasma.h',
  'plasma-retorts.h',
  'plasma-types.h',
  'pool_cmd.h',
  'pool.h',
  'pool-log.h',
  'pool_options.h',
  'pool-time.h',
  'protein.h',
  'slaw-coerce.h',
  'slaw.h',
  'slaw-interop.h',
  'slaw-io.h',
  'slaw-numeric-ilk-rumpus.h',
  'slaw-ordering.h',
  'slaw-path.h',
  'slaw-string.h',
  'slaw-walk.h',
]

plasma_c_incs = include_directories(
  '../../libLoam/c',   # for ob-vers-gen.h
  '../..',
)

plasma_c_deps = [thread_dep, m_dep, openssl_dep, yaml_dep]

plasma_c_link_args = []
if build_machine.system() == 'windows'
  plasma_c_link_args += [ 'ws2_32.lib' ]
endif

plasma_c = library(
   'Plasma',
   plasma_c_sources,
   c_args : [c_flags, '-DHAVE_OPENSSL', '-DOB_PLASMA_EXPORTS'],
   dependencies : plasma_c_deps,
   link_with : [loam_c],
   link_args : plasma_c_link_args,
   include_directories : plasma_c_incs,
   install: true,
   install_rpath: ob_rpath,
   version: '2',
)

install_headers(plasma_c_headers, subdir: 'libPlasma/c')

pkg.generate(
  plasma_c,
  requires: loam_c,
  filebase: 'libPlasma',
)

#------------- executables -----------

plasma_programs = [
  'bin2yaml',
  'p-await',
  'p-create',
  'p-deposit',
  'p-info',
  'p-list',
  'p-newest-idx',
  'p-nth',
  'p-oldest-idx',
  'p-rename',
  'p-resize',
  'p-sleep',
  'p-stop',
  'pool_tcp_server',
  'yaml2bin',
]
plasma_executables = []

foreach p : plasma_programs
   plasma_executables += executable(
     p, p + '.c',
     c_args : c_flags,
     link_with : [plasma_c, loam_c],
     link_args : plasma_c_link_args,
     include_directories : plasma_c_incs,
     install: true,
     install_rpath: ob_rpath,
   )
endforeach

# matrix_test is called by scripts in both the "tests" and "perf" directories,
# so keep it here in the parent directory.  Similarly, await_test is used
# from both the "tests" and "tests-mmap-only" directories, and ob-lsfd is
# used from both "tests" and "t".  fill_pool is only used by the
# scripts in the "perf" directory, but since currently that directory has
# no Makefile and no executables (just scripts), I'll keep fill_pool here
# for now.  The other two executables here (multi_test and simple_read)
# are not installed and are not used by any tests, so their usefulness is
# questionable... they are probably best regarded as "sample programs".

plasma_noinst_programs = [
  'await_test',
  'fill_pool',
  'matrix_test',
  'multi_test',
]
if cc.get_id() == 'msvc'
  plasma_noinst_programs += [
    'fire-and-forget',  # needed by yotest
  ]
else
  plasma_noinst_programs += [
    'ob-lsfd',
    'simple_read',
  ]
endif

foreach p : plasma_noinst_programs
   executable(
     p, p + '.c',
     c_args : c_flags,
     dependencies : plasma_c_deps,
     link_with : [plasma_c, loam_c],
     include_directories : plasma_c_incs,
   )
endforeach

install_data (
  'ob-plasma-cert.sh',
  install_dir: 'bin',
)

if build_tests
  subdir('tests')
  subdir('t')
endif
