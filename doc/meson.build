# FIXME: this just does the equivalent of 'make' in the doc directory.
# Much more is possible (see old Makefile.am).  The external script
# https://gitlab.oblong.com/platform/platform_site_deploy/blob/master/build-doxy.sh
# is currently the main customer.

doxygen = find_program('doxygen', required: false)
if doxygen.found()
  dcdata = configuration_data()
  dcdata.set('top_srcdir', meson.source_root())
  dcdata.set('srcdir', meson.current_source_dir())

  docbuilder = find_program('./docbuilder.py')

  gentargets = []
  foreach f : ['apps', 'common', 'internal', 'latex', 'platform']
    genfile = configure_file(
      input:  'Doxyfile.'+f+'.in',
      output: 'Doxyfile.'+f+'.gen',
      configuration: dcdata,
    )
    gentargets += custom_target(
      f + 'doc',
      command: [docbuilder, meson.current_build_dir(), 'Doxyfile.'+f+'.gen'],
      input: genfile,
      output: f,
    )
  endforeach

  alias_target('doc', gentargets)
else
  message('Doxygen not found.  To build doc, install doxygen, graphviz, and imagemagick.')
endif
