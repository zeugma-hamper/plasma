fs = import('fs')

loam_cpp_test_sources = [
  'ankle-object-interface-specialization-test.cpp',
  'aptest1.cpp',
  'AxisAlignedBoxTests.cpp',
  'BasicStrTest.cpp',
  'Cxx11Test.cpp',
  'EmptyStrTest.cpp',
  'FatherTimeTest.cpp',
  'FrustumTests.cpp',
  'GeomClassesTest.cpp',
  'GeomSlabTest.cpp',
  'IterationStrTest.cpp',
  'MinimalLoamTest.cpp',
  'MoveTest.cpp',
  'NullStrEqTest.cpp',
  'ObAcaciaTest.cpp',
  'ObConsTest.cpp',
  'ObCrawlTest.cpp',
  'ObHormelTest.cpp',
  'ObMapTest.cpp',
  'ObRefTest.cpp',
  'ObRetortTest.cpp',
  'ObTroveTest.cpp',
  'RectangleTests.cpp',
  'RegexpStrTest.cpp',
  'RetortPodTest.cpp',
  'SphereTests.cpp',
  'test_math.cpp',
  'test_rand.cpp',
  'TheForceCanHaveAStrongInfluenceOnTheWeakMinded.cpp',
  'Vect4Tests.cpp',
  'VectTests.cpp',
]

loam_cpp_test_incs = include_directories('..', '../..', '../../..')

loam_cpp_test_deps = [icu_uc_dep, boost_dep]

# Things that run slowly
# FIXME: speed these up
slowtests = [
  'GeomSlabTest.cpp',
  'ObRetortTest.cpp',
  'RegexpStrTest.cpp',
  'test_rand.cpp',
]
fasttimeout = 30
slowtimeout = 200

foreach p : loam_cpp_test_sources
   base = fs.replace_suffix(p, '')
   if p in slowtests
      if skipslowtests == 'yes'
         continue
      endif
      timeout = slowtimeout
      subsuite = '-slow'
   else
      timeout = fasttimeout
      subsuite = ''
   endif
   test(base,
     executable(
       base, p,
       cpp_args : cpp_flags,
       link_with : [loam_c, loam_cpp],
       dependencies : [loam_cpp_test_deps, gtest_main_dep],
       include_directories : [loam_cpp_test_incs],
     ),
     timeout: timeout,
     suite: 'loam++' + subsuite,
   )
endforeach

loam_cpp_wrapped_tests = [
  'aptest3.rb',
  'ObInfoTest.rb',
  'test-prepost-cxx.rb',
]

loam_cpp_wrapped_tests_env = environment()
loam_cpp_wrapped_tests_env.prepend('PATH', ALL_BIN)

foreach p : loam_cpp_wrapped_tests
   base = fs.replace_suffix(p, '')
   executable(
     base, base + '.cpp',
     cpp_args : cpp_flags,
     link_with : [loam_c, loam_cpp],
     dependencies : [loam_cpp_test_deps],
     include_directories : [loam_cpp_test_incs],
   )
   test(base, find_program(p), args : [p], suite: 'loam++', env : loam_cpp_wrapped_tests_env)
endforeach
