project('{{project_name}}', 'cpp',
  version : '1.0.0',
  license : 'proprietary',
  default_options : [
    'werror=true',
    'warning_level=2',
    'cpp_std=c++11',
    'buildtype=debug',
    'b_ndebug=if-release',
  ]
)

cpp_compiler = meson.get_compiler('cpp')
if cpp_compiler.get_id() == 'msvc'
  maybe_cpp_flags = [
    # Suppress some warnings that are too annoying:
    '/wd4244', # conversion from 'double' to 'float32', possible loss of data
    '/wd4305', #  'argument': truncation from 'double' to 'float32'
    '/wd4251', # FIXME: needs to have dll-interface to be used by clients of class 'oblong::basement::UrDrome'
  ]
else
  maybe_cpp_flags = [
    # Let's be a little stricter than meson's warning_level=2
    # (which is -Wall -Winvalid-pch -Wnon-virtual-dtor -Wextra):
    '-Wempty-body',
    '-Wpointer-arith',
    '-Wshadow',
    '-Wsuggest-override',
    '-Wvla',
    '-Wwrite-strings',
    # Suppress some warnings that are too annoying:
    '-Wno-error=sign-compare',
    '-Wno-error=suggest-override',  # e.g. boost/smart_ptr/detail/local_counted_base.hpp's local_cb_destroy
  ]
endif
cpp_flags = cpp_compiler.get_supported_arguments(maybe_cpp_flags)

# Find system libraries this project depends on.
libs = [
   'icu-i18n',
   'icu-uc',
   'libLoam++',
]
project_deps = []
foreach lib : libs
   project_deps += dependency(lib, required : true)
endforeach

# Build the project, and install it if asked (via 'ninja install').
executable(
   '{{project_name}}',
   'src/{{project_name}}.cpp',
   cpp_args : cpp_flags,
   dependencies : project_deps,
   install : true)
