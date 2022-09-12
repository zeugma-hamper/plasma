plasma_cpp_sources = [
  'AtomicComposite.cpp',
  'CompositeSlaw.cpp',
  'Hose.cpp',
  'HoseGang.cpp',
  'Pool.cpp',
  'Protein.cpp',
  'ProteinComposite.cpp',
  'Slaw.cpp',
  'SlawCons.cpp',
  'SlawIterator.cpp',
  'SlawList.cpp',
  'SlawMap.cpp',
  'SlawRef.cpp',
]

staging_plasma_cpp_sources = [
  '../../libStaging/libPlasma/c++/PlasmaHelpers.cpp',
  '../../libStaging/libPlasma/c++/Slawzall.cpp',
]


plasma_cpp_headers = [
  'CompositeSlaw.h',
  'HoseGang.h',
  'Hose.h',
  'PlasmaForward.h',
  'Plasma.h',
  'PlasmaStreams.h',
  'PlasmaxxRetorts.h',
  'Pool.h',
  'Protein.h',
  'RefCounted.h',
  'Slaw.h',
  'SlawIterator.h',
  'SlawRef.h',
  'slaw-traits.h',
]

staging_plasma_cpp_headers = [
  '../../libStaging/libPlasma/c++/PlasmaHelpers.h',
  '../../libStaging/libPlasma/c++/Slawzall.h',
  '../../libStaging/libPlasma/c++/std-slaw-traits.h',
]

plasma_cpp_incs = include_directories(
  '../..',
)

plasma_cpp_deps = [icu_i18n_dep, boost_dep, thread_dep, m_dep]

plasma_cpp = library(
   'Plasma++',
   plasma_cpp_sources + staging_plasma_cpp_sources,
   cpp_args : cpp_flags + ['-DOB_PLASMAXX_EXPORTS'],
   dependencies : plasma_cpp_deps,
   link_with : [plasma_c, loam_c, loam_cpp],
   include_directories : [plasma_cpp_incs, plasma_c_incs],
   install: true,
   install_rpath: ob_rpath,
   version: '2',
)

install_headers(plasma_cpp_headers, subdir: 'libPlasma/c++')
install_headers(staging_plasma_cpp_headers, subdir: 'libStaging/libPlasma/c++')

pkg.generate(
  plasma_cpp,
  filebase: 'libPlasma++',
  requires: [plasma_c, loam_cpp, loam_c],
)

subdir('samples')
if build_tests
  subdir('tests')
endif
