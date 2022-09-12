# Check whether samples build
loam_cpp_sample_incs = include_directories('..', '../..', '../../..')
loam_cpp_sample_deps = [icu_uc_dep, icu_i18n_dep, boost_dep]
canary = executable(
  'canary', 'canary/src/canary.cpp',
  cpp_args : cpp_flags,
  link_with: [loam_cpp, loam_c],
  dependencies: loam_cpp_sample_deps,
  include_directories : loam_cpp_sample_incs,
)

# Special case:
# canary is also a runnable test; all it does is verify
# that it can find its shared libraries, and that unicode strings work.
if build_tests
   test('canary', sh, args : ['yotest', 'run', 'local', canary], suite: 'loam++', env : loam_cpp_wrapped_tests_env)
endif

install_data('Makefile', install_dir: 'samples/libLoam++')

# Expand sample templates
foreach sample : ['canary']
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
  install_dir: 'samples/libLoam++',
  strip_directory: true)
