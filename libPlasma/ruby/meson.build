rubyplasma_sources = [
  'rubyPlasma.c',
]

ruby_dep = dependency('ruby')
rubyplasma_deps = [ruby_dep]

get_ruby = run_command('../../bld/cmake/get-ruby-include-dirs.sh', check: true)
ruby_incs = get_ruby.stdout().strip().split('\n')

if build_machine.system() == 'darwin'
  name_suffix = 'bundle'
elif build_machine.system() == 'linux'
  name_suffix = 'so'
elif build_machine.system() == 'windows'
  name_suffix = 'dll'
endif

rubyplasma = shared_library(
   'rubyPlasma',
   rubyplasma_sources,
   name_prefix: '',
   c_args : c_flags,
   dependencies : rubyplasma_deps,
   link_with : [plasma_cpp, plasma_c, loam_cpp, loam_c],
   include_directories : [loam_c_incs, ruby_incs],
   # Note: in past, we installed this to lib/ruby.
   # There appears to be no way to tell meson to do that.
   install: true,
   name_suffix: name_suffix,
   # Note: all our other libraries have a hardcoded soversion of 2, but not this one.
   #version: '2',
)

rubyplasma_headers = [
  'Pool.rb',
  'Protein.rb',
  'SlawInputFile.rb',
  'slaw-not-tests.rb',
  'SlawOutputFile.rb',
  'Slaw.rb',
]

install_data(
  rubyplasma_headers,
  install_dir: get_option('libdir') / 'ruby',
)

subdir('gembuild')

if build_tests
  # Don't run delete-pools.wrap.rb, as it's too order-dependent, and fails if run first :-(

  configure_file(
    input: 'rubyPlasmaWrapper.meson.in',
    output: 'rubyPlasmaWrapper',
    configuration: cdata)

  executable(
    'c-test-feeder', 'c-test-feeder.c',
    include_directories: '../..',
    link_with: [plasma_c, loam_c],
  )

  rubyplasma_tests = [
    'bug3787.wrap.rb',
    'file-io-test.wrap.rb',
    'hose-extension-tests.wrap.rb',
    'json-tests.wrap.rb',
    'pool-tests.wrap.rb',
    'slaw-tests.wrap.rb',
    'types-test.wrap.rb',
  ]
  rubyplasma_tests_env = environment()
  rubyplasma_tests_env.prepend('PATH', meson.current_build_dir())
  foreach test : rubyplasma_tests
    test(test, yotest, args: ['run', 'local', meson.current_source_dir() / test], suite: 'rubyplasma', is_parallel: false, env: rubyplasma_tests_env)
  endforeach
endif
