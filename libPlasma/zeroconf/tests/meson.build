plasma_zeroconf_test_sources = [
  'PoolServerTest.cpp',
  'ZeroconfTest.cpp',
]

plasma_zeroconf_test_incs = include_directories('../../..')

plasma_zeroconf_test_deps = [icu_uc_dep, boost_dep]

foreach p : plasma_zeroconf_test_sources
   base = fs.replace_suffix(p, '')
   exe = executable(
       base, p,
       cpp_args : cpp_flags,
       link_with : [plasma_zeroconf, plasma_cpp, plasma_c, loam_cpp, loam_c],
       dependencies : [plasma_zeroconf_test_deps, gtest_main_dep],
       include_directories : [plasma_zeroconf_test_incs],
   )
   test(base, yotest, args : ['run', 'local', exe], is_parallel : false, suite: 'plasma_zeroconf')
endforeach

