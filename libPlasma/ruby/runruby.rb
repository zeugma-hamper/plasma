#!/usr/bin/ruby
$VERBOSE = true
require 'rbconfig'

MYDIR = File.dirname(__FILE__)
ARCH = "/usr/bin/arch"

def is_mac?
  RbConfig::CONFIG['host_os'] =~ /^darwin/i
end

# This does the following things
# - always use /usr/bin/ruby (custom rubys are an endless source of problems)
# - On Mac, force 32-bit
# - set up paths using rubyPlasmaWrapper script
def runruby(argv)
  args = Array.new
  args.push(MYDIR + "/rubyPlasmaWrapper")
  if (is_mac? and File.executable?(ARCH))
    args.push(ARCH)
    args.push("-i386")
  end
  args.push("/usr/bin/ruby")
  args.push("-w")
  args.push(*argv)
  exec(*args)
  exit 1 # exec failed
end

if (ARGV.length == 0)
  STDERR.print "Usage: runruby.rb <script> [args ...]\n"
  exit 1
else
  runruby(ARGV)
end
