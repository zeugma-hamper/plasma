loam_cpp_sources = [
  'AnkleObject.cpp',
  'ArgParse.cpp',
  'AxisAlignedBox.cpp',
  'FatherTime.cpp',
  'Frustum.cpp',
  'GeomSlab.cpp',
  'GeomTransform.cpp',
  'Line.cpp',
  'LoamxxRetorts.cpp',
  'Matrix44.cpp',
  'ObColor.cpp',
  'ObHormel.cpp',
  'ObInfo.cpp',
  'ObMap.cpp',
  'ObPseudopod.cpp',
  'ObResolvedPath.cpp',
  'ObRetort.cpp',
  'ObRetortPod.cpp',
  'ObTrove.cpp',
  'ObUniqueTrove.cpp',
  'Plane.cpp',
  'Quat.cpp',
  'Rectangle.cpp',
  'Sphere.cpp',
  'Str.cpp',
  'StrIterator.cpp',
  'StrMatchData.cpp',
  'StrPath.cpp',
  'Vect.cpp',
  'Vect4.cpp',
  'ob-coretypes-hash.cpp',
  'ob-math-utils.cpp',
]

if cc.get_id() == 'msvc'
  loam_cpp_sources += [
    'win32/Mutex_win32.cpp',
  ]
else
  loam_cpp_sources += [
    'Mutex.cpp',
  ]
endif

loam_cpp_headers = [
  'AnkleObject.h',
  'ankle-object-interface-specialization.h',
  'ArgParse.h',
  'AxisAlignedBox.h',
  'CrawlIterator.h',
  'ErrnoSaver.h',
  'FatherTime.h',
  'Frustum.h',
  'GeomSlab.h',
  'GeomTransform.h',
  'Line.h',
  'LoamForward.h',
  'LoamStreams.h',
  'LoamxxRetorts.h',
  'Matrix44.h',
  'Mutex.h',
  'ObAcacia.h',
  'ObColor.h',
  'ObCons.h',
  'ob-coretypes-hash.h',
  'ObCrawl.h',
  'ob-enumclass.h',
  'ObHasher.h',
  'ObHormel.h',
  'ObInfo.h',
  'ObMap.h',
  'ob-math-utils.h',
  'ObPseudopod.h',
  'ObRef.h',
  'ObResolvedPath.h',
  'ObRetort.h',
  'ObRetortPod.h',
  'ObTrove.h',
  'ob-typeof.h',
  'ObUniqueTrove.h',
  'ObWeakRef.h',
  'OC_Functionary.h',
  'OC_Helpy.h',
  'OC_Undeffy.h',
  'OH_Functionary.h',
  'OM_Functionary.h',
  'OM_Helpy.h',
  'OT_Functionary.h',
  'OT_Helpy.h',
  'patella-macros.h',
  'Plane.h',
  'Preterite.h',
  'Quat.h',
  'Rectangle.h',
  'Sphere.h',
  'Str.h',
  'StrIterator.h',
  'StrPath.h',
  'Vect4.h',
  'Vect.h',
]

loam_cpp_incs = include_directories(
  '../..',
)

loam_cpp_deps = [icu_uc_dep, icu_i18n_dep, boost_dep, thread_dep, m_dep]

loam_cpp = library(
   'Loam++',
   loam_cpp_sources,
   cpp_args : cpp_flags + ['-DOB_LOAMXX_EXPORTS'],
   dependencies : loam_cpp_deps,
   link_with : loam_c,
   include_directories : [loam_cpp_incs, loam_c_incs],
   install: true,
   install_rpath: ob_rpath,
   version: '2',
)

pkg.generate(
  loam_cpp,
  filebase: 'libLoam++',
  # FIXME: figure out how to refer to boost_dep more cleanly?
  libraries: [
    boost_dep_0,
    ob_rpath_flags,
  ],
  requires: [loam_c, icu_i18n_dep],
)

install_headers(loam_cpp_headers, subdir: 'libLoam/c++')

if build_tests
  subdir('tests')
endif
subdir('samples')
