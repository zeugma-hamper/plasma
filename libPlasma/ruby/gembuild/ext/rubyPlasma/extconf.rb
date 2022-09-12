require 'mkmf'

class String
  def explode
    # http://stackoverflow.com/questions/4078906/is-there-a-natural-sort-by-method-for-ruby
    scan(/[^\d]+|[\d]+/).collect { |w| w.match(/\d+/) ? w.to_i : w }
  end
end

def cmp_version(a, b)
  a.explode() <=> b.explode()
end

def find_pkg_config
  puts "extconf: Looking for pkg-config in $YOBUILD/bin (for debugging)"
  filename = "#{ENV['YOBUILD']}/bin/pkg-config"
  if File.file?(filename)
    return filename
  end

  puts "extconf: Looking for pkg-config in /opt/oblong/deps-*/bin"
  filename = Dir.glob('/opt/oblong/deps-*/bin/pkg-config').sort { |a,b| cmp_version(a,b) }.last
  if filename && File.file?(filename)
    return filename
  end

  puts "extconf: Looking for pkg-config in /opt/oblong/deps/bin (for yobuild8)"
  filename = '/opt/oblong/deps/bin/pkg-config'
  if File.file?(filename)
    return filename
  end

  puts "extconf: Defaulting to just using pkg-config from PATH"
  return 'pkg-config'
end

def find_pc_dir
  puts "extconf: Looking for libPlasma.pc in $G_SPEAK_HOME/lib/pkgconfig (for debugging)"
  dir = "#{ENV['G_SPEAK_HOME']}/lib/pkgconfig"
  if dir && (File.file?(dir + "/libPlasma.pc"))
    return dir, "libPlasma"
  end

  puts "extconf: Looking for libPlasma.pc in $G_SPEAK_HOME/lib/*/pkgconfig (for debugging)"
  dir = "#{ENV['G_SPEAK_HOME']}/lib/*/pkgconfig"
  if dir && (File.file?(dir + "/libPlasma.pc"))
    return dir, "libPlasma"
  end

  puts "extconf: Looking for libPlasma.pc in /opt/oblong/g-speak*/lib/pkgconfig"
  dir = Dir.glob('/opt/oblong/g-speak*/lib/pkgconfig').sort { |a,b| cmp_version(a,b) }.last
  if dir && (File.file?(dir + "/libPlasma.pc"))
    return dir, "libPlasma"
  end

  puts "extconf: Looking for libPlasma.pc in /opt/oblong/g-speak*/lib/*/pkgconfig"
  dir = Dir.glob('/opt/oblong/g-speak*/lib/*/pkgconfig').sort { |a,b| cmp_version(a,b) }.last
  if dir && (File.file?(dir + "/libPlasma.pc"))
    return dir, "libPlasma"
  end

  abort "extconf: fail: Could not find libPlasma.pc or libGreenhouse.pc"
end

pkg_config          = find_pkg_config
pkg_config_dir, pkg = find_pc_dir

puts "extconf: using pkg_config #{pkg_config}, pkg_config_dir #{pkg_config_dir}, pkg #{pkg}"

cflags  = `env PKG_CONFIG_PATH=#{pkg_config_dir} #{pkg_config} --cflags #{pkg}`
if cflags.empty?
  abort "Could not get cflags for #{pkg} from pkgconfig"
end
ldflags = `env PKG_CONFIG_PATH=#{pkg_config_dir} #{pkg_config} --libs   #{pkg}`
if ldflags.empty?
  abort "Could not get ldflags for #{pkg} from pkgconfig"
end

$CFLAGS  << ' ' + cflags
$LDFLAGS << ' ' + ldflags

extension_name = 'rubyPlasma'
dir_config(extension_name)
create_makefile(extension_name)
