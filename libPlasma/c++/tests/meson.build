plasma_cpp_test_incs = include_directories('..')

# Note: can't make shared libraries with gtest, they crash.
plasma_cpp_test_helper_lib = static_library(
   'plasma_cpp_test_helper_lib',
   'SlawTypesTest.cpp', 'Callbacks.cpp',
   cpp_args : cpp_flags,
   dependencies : [plasma_cpp_deps, gtest_main_dep],
   link_with : [plasma_cpp, plasma_c, loam_c, loam_cpp],
   include_directories : [plasma_cpp_incs, plasma_c_incs],
)

plasma_cpp_tests = [
  'ArraySlawTest.cpp',
  'HoseNthTest.cpp',
  'HoseTest.cpp',
  'HoseTimedTest.cpp',
  'PoolTest.cpp',
  'ProteinTest.cpp',
  'RefCountedTest.cpp',
  'SlawConsTest.cpp',
  'SlawIteratorTest.cpp',
  'SlawListTest.cpp',
  'SlawMapDuplicateTest.cpp',
  'SlawMapTest.cpp',
  'SlawRefTest.cpp',
  'SlawScopeTest.cpp',
  'SlawSerializationTest.cpp',
  'SlawTest.cpp',
  'slaw-traits-test.cpp',
  'SlawVectorArrayTest.cpp',
]

fs = import('fs')

rigs = ['local', 'tcp', 'tcpo', 'tcps']

foreach p : plasma_cpp_tests
   base = fs.replace_suffix(p, '')
   executable(
     base, p,
     cpp_args : c_flags + test_c_flags,
     link_with : [plasma_cpp_test_helper_lib, plasma_cpp, plasma_c, loam_cpp, loam_c],
     dependencies : [plasma_cpp_deps, gtest_main_dep],
     include_directories : [plasma_c_incs, plasma_cpp_test_incs],
   )
   f = meson.current_build_dir() + '/' + base
   foreach rig : rigs
     if (rig == 'tcpo' or rig == 'tcps')
        if skipslowtests == 'yes'
           continue
        endif
        timeout = 60
        suitesuffix = '-slow'
     else
        timeout = 15
        suitesuffix = ''
     endif
     test(base + '-' + rig, yotest, args : ['run', rig, f], is_parallel : false, timeout: timeout, suite: 'plasma++' + suitesuffix)
   endforeach
endforeach

# Mixing c++11 and objc++ simply doesn't work in meson?!
# FIXME
if build_machine.system() == 'darwin'
   foreach p : [] # 'ObjcPlusPlus.cpp'
      base = fs.replace_suffix(p, '')
      executable(
        base, p,
        # Work around https://github.com/mesonbuild/meson/issues/5495
        cpp_args : c_flags + test_c_flags + ['-std=c++11', '-xobjective-c++'],
        link_with : [plasma_cpp, plasma_c, loam_cpp, loam_c],
        dependencies : [plasma_cpp_deps, gtest_main_dep, cocoa_dep, core_dep],
        include_directories : [plasma_c_incs, plasma_cpp_test_incs],
      )
      f = meson.current_build_dir() + '/' + base
      test(base, yotest, args : ['run', 'local', f], is_parallel : false, suite: 'plasma++')
   endforeach
endif
