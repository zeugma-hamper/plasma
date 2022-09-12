# (c) 2010 Oblong Industries

require 'getoptlong'
require 'rbconfig'

# test/unit went away in Ruby 2.0, but we still need to build on ruby 1.x
begin
  require 'minitest/autorun'
  Test = MiniTest
  module MiniTest
    # Allow assert_not_nil, even though it's obsolete
    module Assertions
      alias assert_not_nil refute_nil
    end
  end
rescue LoadError
  require 'test/unit'
  MiniTest = Test
  module Test
    Assertions = Unit::Assertions
    # Allow refute_nil, even though it's from the future
    module Assertions
      alias refute_nil assert_not_nil
    end
  end
end

TEST_POOL = "TEST_POOL"
OB_LOG = "OB_LOG"
SO_FILE = ".libs/rubyPlasma.so"
BUNDLE_FILE = ".libs/rubyPlasma.bundle"
ARCH = "/usr/bin/arch"
IV = "--internal-valgrind"
$internal_valgrind = nil
SRCDIR = __dir__
YOVO_BUILD_TYPE=ENV["OBLONG_YOVO_BUILD_TYPE"]

def is_mac?
  RbConfig::CONFIG['host_os'] =~ /^darwin/i
end

# Check for Mac OS X 10.5 (where "gem" command is broken)
def is_105?
  RbConfig::CONFIG['host_os'] =~ /^darwin9/i
end

# Check for Ruby 1.8.5 or earlier (where json gem's C extension fails to build)
def is_185?
  RbConfig::CONFIG['MAJOR'] == "1" and
    RbConfig::CONFIG['MINOR'] == "8" and
    RbConfig::CONFIG['TEENY'].to_i <= 5
end

def obsystem(cmd)
  fullcmd = cmd
  fullcmd = $internal_valgrind + " " + cmd if ($internal_valgrind)
  success = system(fullcmd)
  if (!success)
    STDERR.print fullcmd
    STDERR.print " exited with code ", $?.exitstatus.to_s if $?.exited?
    STDERR.print " terminated with signal ", $?.termsig.to_s if $?.signaled?
    STDERR.print " (core dumped)" if $?.coredump?
    STDERR.print "\n"
    exit 1
  end
end

def runruby(script, *extras)
  args = Array.new
  if (File.executable?("./rubyPlasmaWrapper"))
    args.push("./rubyPlasmaWrapper")   # automake case
  else
    args.push("rubyPlasmaWrapper")
  end
  if (is_mac?  and  /i386/.match(`file #{BUNDLE_FILE}`))
    args.push(ARCH)
    args.push("-i386")
  end
  args.push("/usr/bin/ruby")
  args.push("-w")
  args.push("#{SRCDIR}/#{script}")
  args.push(*extras)
  exec(*args)
  exit 1 # exec failed
end

def parse_iv
  GetoptLong.new([IV, GetoptLong::REQUIRED_ARGUMENT]).each do |opt, arg|
    $internal_valgrind = arg if (opt == IV)
  end
end

def obwrap(script)
  parse_iv

  # yotest sets ASAN_PRELOAD if yovo was built with address sanitizer
  if ENV.has_key?("ASAN_PRELOAD")
   # FIXME: Avoid dreaded error
   # ERROR: AddressSanitizer failed to deallocate 0x4000 (16384) bytes at address 0x62900000a200
   # AddressSanitizer CHECK failed: ../../../../src/libsanitizer/sanitizer_common/sanitizer_posix.cc:133 "(("unable to unmap" && 0)) != (0)" (0x0, 0x0)
   # which happens no matter what I do with LD_PRELOAD or ASAN_OPTIONS on ubuntu.
   obsystem("echo 'skipping #{script} because ASAN_PRELOAD is set, and Address Sanitizer crashes rubyPlasma at the moment'")
   return
  end

  # skip if we're valgrinding, or if .so doesn't exist (e. g. static build)
  if (YOVO_BUILD_TYPE != "shared" and
      ($internal_valgrind or not(File.exists?(SO_FILE) or
                                File.exists?(BUNDLE_FILE))))
    # Basically skip valgrind, by running a harmless program
    why = $internal_valgrind ? "valgrind" : "static build"
    obsystem("echo skipping #{script} because #{why}")
  else
    runruby(script)
  end
end

def spawn_feeders(x)
  x.map do |pool, arg|
    fork { runruby("r-test-feeder.rb", pool, "6", arg.to_s, "0.5") }
  end
end

def next_expected(counts, trib)
  expected = counts[trib]
  counts[trib] = expected + 1
  return expected
end

def formulate_pool_name(pname)
  if ENV.has_key?(TEST_POOL)
    return ENV[TEST_POOL] + "+" + pname
  else
    return pname
  end
end

def modify_logging_configuration(cfg)
  if ENV.has_key?(OB_LOG)
    ENV[OB_LOG] += " #{cfg}"
  else
    ENV[OB_LOG] = cfg
  end
end
