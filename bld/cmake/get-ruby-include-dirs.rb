# Ruby script to get include directories needed to compile native ruby bindings

keys = [
  'archincludedir',
  'includedir',
  # 'oldincludedir',   # /usr/include... nah, that's covered :-)
  'rubyarchhdrdir',
  'rubyhdrdir',
  'sitearchhdrdir',
  'sitearchincludedir',
  'sitehdrdir',
  'vendorarchhdrdir',
  'vendorhdrdir',
]

goodkeys = keys.select { |k| RbConfig::CONFIG.key?(k) }

dirs = goodkeys.map { |k| RbConfig::CONFIG[k] }

# Bug in initial beta build of mac os 10.14:
# C programs that need to include ruby/config.h fail because
#  ruby -rrbconfig -e "puts RbConfig::CONFIG['rubyarchhdrdir'];"
# outputs
#  /System/Library/Frameworks/Ruby.framework/Versions/2.3/usr/include/ruby-2.3.0/universal-darwin18
# But that directory does not exist, whereas the directory
#  /System/Library/Frameworks/Ruby.framework/Versions/2.3/usr/include/ruby-2.3.0/universal-darwin17
# does exist.
# Workaround:
rubyarchhdrdir = RbConfig::CONFIG['rubyarchhdrdir']
dirs << rubyarchhdrdir.gsub('universal-darwin18', 'universal-darwin17')

# On OSX 10.12, a system or tools update left us with ruby 2.3, but
#   /System/Library/Frameworks/Ruby.framework and
#   /Applications/Xcode8.1.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk/System/Library/Frameworks/Ruby.framework
# do not contain ruby 2.3's ruby.h; that is in
#   /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/Ruby.framework/Versions/2.3/Headers/ruby/ruby.h
# which caused the build failure
#   ../libPlasma/ruby/rubyPlasma.c:15:10: fatal error: 'ruby.h' file not found

dirs << "/Library/Developer/CommandLineTools/SDKs/MacOSX.sdk/System/Library/Frameworks/Ruby.framework/Versions/2.3/Headers"

puts dirs.uniq.select { |d| Dir.exist?(d + '/ruby') }.sort
