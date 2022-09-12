#!/usr/bin/ruby

# There is a large amount of redundancy between this test, and
# test-version.pl.  The point of this test is to test the ob-version
# executable and its yaml/json output, while test-version.pl tests the
# API using its own custom-built test program.  But someday we might
# eliminate test-version.pl in favor of this test, since that one doesn't
# add a lot of value over this one.

require File.dirname(__FILE__) + '/test-helpers.rb'
require 'getoptlong'
require 'rbconfig'
require 'tempfile'
require 'yaml'
begin
  require 'rubygems'
  require 'json'
rescue LoadError
  # Sadly, this is a pretty large chunk of code copied from
  # libPlasma/c/json-tests.wrap.rb and libPlasma/c/test-helpers.rb
  # Obviously it would be nice to refactor someday to avoid the duplication.
  GET_OUT_OF_JAIL_FREE = "OBLONG_MACHINE_SETUP_MISSING_PACKAGES"

  if ENV.has_key?(GET_OUT_OF_JAIL_FREE) or is_105?
    # If someone explicitly chose not to install all the packages,
    # then we'll give them a break about not having json.
    HAVE_JSON = false
  else
    jsongem = is_185? ? "json_pure" : "json"
    msg = ["The json rubygem is required for this test.",
           "You can install it with 'sudo gem install #{jsongem}'",
           "Or you can force this test to pass without actually",
           "running by setting the environment variable:",
           GET_OUT_OF_JAIL_FREE + "=ignore"]
    len = msg.map {|x| x.length}.max
    edge = "+-" + ("-" * len) + "-+\n"
    STDERR.print edge
    msg.each {|x| STDERR.print "| ", x.ljust(len), " |\n"}
    STDERR.print edge
    exit 1
  end
else
  HAVE_JSON = true
end

# json and tmpdir cause warnings on Windows Ruby, so wait until after
# they're loaded to enable warnings.
$VERBOSE = true

IV = "--internal-valgrind"
$internal_valgrind = nil

GetoptLong.new([IV, GetoptLong::REQUIRED_ARGUMENT]).each do |opt, arg|
  $internal_valgrind = arg if (opt == IV)
end

def is_windows?
  RbConfig::CONFIG['host_os'] == "mingw32" || RbConfig::CONFIG['host_os'] == "cygwin"
end

if is_windows?
  ENV['PATH'] = Dir.getwd.sub(%r{tests/win32}, "win32") + ";" + ENV['PATH']
  IS_WINDOWS = true
else
  ENV['PATH'] = "..:" + ENV['PATH']
  IS_WINDOWS = false
end

cmd = "ob-version -y"

cmd = $internal_valgrind + " " + cmd if ($internal_valgrind)

$vers_data = `#{cmd}`
if (!($?.exited? && $?.exitstatus == 0))
  STDERR.print cmd
  STDERR.print " exited with code ", $?.exitstatus.to_s if $?.exited?
  STDERR.print " terminated with signal ", $?.termsig.to_s if $?.signaled?
  STDERR.print " (core dumped)" if $?.coredump?
  STDERR.print "\n"
  exit 1
end

$unyaml = YAML.load($vers_data)

LSBRP = '/usr/bin/lsb_release' # for Linux
SV = '/usr/bin/sw_vers' # for Mac OS X
CPUINFO = '/proc/cpuinfo' # for Linux

class ObVersionTest < Test::Unit::TestCase
  if HAVE_JSON
    def test_yaml_json_equal
      unjson = JSON.load($vers_data)
      assert_equal($unyaml, unjson)
    end
  end

  if !IS_WINDOWS
    def test_kernel_version
      kernel = `uname -r`.chomp
      assert_equal(kernel, $unyaml['kernel-version'])
    end

    def DISABLED_test_os
      if File.exists?(SV)
        os = `#{SV} -productVersion`
      elsif File.exists?(LSBRP)
        os = `#{LSBRP} --description --short`.gsub(/\"/, "")
      else
        raise "could not find #{LSBRP}; maybe you need to yum install redhat-lsb"
      end
      os.chomp!
      assert_equal(os, $unyaml['os-version'])
    end
  end

  if (RbConfig::CONFIG['target_os'] == "linux")
    def test_libc
      libc = `/usr/bin/getconf GNU_LIBC_VERSION`.chomp
      assert_equal(libc, $unyaml['libc-version'])
    end
  end

  def test_cores
    cores = $unyaml['number-of-cores']
    # This is just a sanity check.  Must have at least one core,
    # and let's pick an upper bound that's unlikely to be reached soon.
    assert(cores >= 1  &&  cores <= 1024)
  end

  def test_bit_width
    bits = $unyaml['bit-width']
    # Don't know which, but it has to be one of these.
    assert(bits == 32  ||  bits == 64)
  end

  def test_tmp
    dir = $unyaml['ob-tmp-dir'][0]
    Tempfile.open("tstobv", dir) do |t|
      t.print "Hello, World!\n"
    end
  end
end
