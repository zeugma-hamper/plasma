#!/usr/bin/ruby

# Test, using p-info, that we're actually using the slaw and protocol
# versions that we think we are.

$VERBOSE = true

require_relative 'test-helpers.rb'

ENV['PATH'] = '.' + File::PATH_SEPARATOR + '..' + File::PATH_SEPARATOR + ENV['PATH']

TEST_POOL = ENV["TEST_POOL"]
P_INFO = "p-info"
SRC_TOP = __dir__ + "/../../.."
TMP_FILE = "scratch/avoid-fallback.tmp"

def die(msg)
  STDERR.print msg
  exit 1
end

def extract_constant(file, macro)
  result = nil
  rex = Regexp.new("^\\\#define\\s+#{macro}\\s+(.+)")
  IO.foreach(SRC_TOP + "/" + file) do |line|
    if line =~ rex
      result = $1
    end
  end
  if result
    result.sub!(/OB_CONST_U64 *\(/, "")
    result.sub!(/\)/, "")
  else
    die "Couldn't find #{macro} in #{file}\n"
  end
  # to_i sucks because it just returns 0 if you give it something it can't parse
  if result !~ /^\d+$/
    die "for '#{macro}', found '#{result}'\n"
  end
  result.to_i
end

if old_tcp_server?
  # Make test system happy by running *something* under valgrind
  obsystem("true")
  exit 0
end

SLAW_VERSION = extract_constant("libPlasma/c/slaw-interop.h",
                                "SLAW_VERSION_CURRENT")
MMAP_VERSION = extract_constant("libPlasma/c/pool_mmap.c",
                                "POOL_MMAP_CURRENT_VERSION")
TCP_VERSION = extract_constant("libPlasma/c/private/pool_tcp.h",
                               "POOL_TCP_VERSION_CURRENT")

STDOUT.reopen(TMP_FILE, "w")
obsystem(P_INFO + " " + TEST_POOL)
STDOUT.reopen(STDERR)

info = Hash.new

IO.foreach(TMP_FILE) do |line|
  break if line =~ /^\.\.\./
  info[$1] = $2 if line =~ /^(\S+)\:\s+(\S+)/
end

if using_tcp?
  expected = "tcp"
  actual = info['type']
  if (expected != actual)
    die "expected type #{expected} but got #{actual}\n"
  end
  expected = TCP_VERSION
  actual = info['net-pool-version'].to_i
  if (expected != actual)
    die "expected net-pool-version #{expected} but got #{actual}\n"
  end
else
  expected = "mmap"
  actual = info['type']
  if (expected != actual)
    die "expected type #{expected} but got #{actual}\n"
  end
  expected = MMAP_VERSION
  actual = info['mmap-pool-version'].to_i
  if (expected != actual)
    die "expected mmap-pool-version #{expected} but got #{actual}\n"
  end
end

expected = SLAW_VERSION
actual = info['slaw-version'].to_i
if (expected != actual)
  die "expected slaw-version #{expected} but got #{actual}\n"
end
