configure_file(
    input: 'build.sh.in',
    output: 'plasmagembuild.sh',
    configuration: cdata)

configure_file(
    input: 'rubyPlasma.gemspec.in',
    output: 'rubyPlasma.gemspec',
    configuration: cdata)

plasmagembuild = find_program(meson.current_build_dir() / 'plasmagembuild.sh')

custom_target('plasmagem',
   output: 'rubyPlasma-' + version + '.gem',
   command: [plasmagembuild],
   install: true,
   install_dir: get_option('libdir') / 'ruby',
)
