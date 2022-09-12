# Check whether samples build
plasma_cpp_sample_incs = include_directories('..', '../..', '../../..')
plasma_cpp_sample_deps = [icu_uc_dep, boost_dep]
executable(
  'p-eek', 'p-eek/src/p-eek.cpp',
  cpp_args : cpp_flags,
  link_with: [plasma_cpp, plasma_c, loam_cpp, loam_c],
  dependencies: plasma_cpp_sample_deps,
  include_directories : plasma_cpp_sample_incs,
)

install_data('Makefile', install_dir: 'samples/libPlasma++')

# Expand sample templates
foreach sample : ['p-eek']
  run_command(
    expand_sample,
    meson.current_build_dir() / 'generated' / sample,
    outer_template,
    meson.current_source_dir() / 'obi-template',
    meson.current_source_dir() / sample,
    check: true)
endforeach
install_subdir(
  meson.current_build_dir() / 'generated',
  install_dir: 'samples/libPlasma++',
  strip_directory: true)
