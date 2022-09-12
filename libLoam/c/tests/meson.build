# c tests

nonparallelizable = [
  'test-userid.sh',
  'LoamGTest.cpp',
]

loam_c_tests = [
  'test-env.c',
  'test-hash.c',
  'test-logging.c',
  'test-paths.c',
  'test-prepost.c',
  'test-rand.c',
  'test-time.c',
  'test-uuid.c',
  'test-vasprintf.c',
  'test-version.c',
]

foreach p : loam_c_tests
   base = fs.replace_suffix(p, '')
   test(base,
    executable(
     base, p, ob_vers_gen_h,
     c_args : c_flags + test_c_flags,
     link_with : [loam_c],
     dependencies : loam_c_deps,
     include_directories : loam_c_incs,
    ),
    suite: 'loam',
   )
endforeach

loam_c_wrapped_tests = [
  'test-endian.sh',
  'test-userid.sh',
]

loam_c_wrapped_tests_env = environment()
loam_c_wrapped_tests_env.prepend('PATH', ALL_BIN)

foreach p : loam_c_wrapped_tests
   base = fs.replace_suffix(p, '')
   executable(
     base, base + '.c',
     c_args : c_flags + test_c_flags,
     link_with : [loam_c],
     dependencies : loam_c_deps,
     include_directories : loam_c_incs,
   )
   f = meson.current_source_dir() + '/' + p
   test(base, find_program(f), args : [f],
     env : loam_c_wrapped_tests_env,
     is_parallel: not (p in nonparallelizable),
     suite: 'loam',
   )
endforeach

# Google Tests
loam_c_gtests = [
  'LoamGTest.cpp',
  'TestAtomic.cpp',
  'TestFile.cpp',
]

foreach p : loam_c_gtests
   base = fs.replace_suffix(p, '')
   test(base,
     executable(
       base, p,
       cpp_args : cpp_flags + test_c_flags,
       link_with : [loam_c],
       dependencies : [gtest_main_dep],
       include_directories : [loam_c_incs],
     ),
     is_parallel: not (p in nonparallelizable),
     suite: 'loam',
   )
endforeach
