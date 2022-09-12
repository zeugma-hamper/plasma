#!/usr/bin/ruby
$VERBOSE = true

require 'getoptlong'

IV = "--internal-valgrind"
$internal_valgrind = nil

GetoptLong.new([IV, GetoptLong::REQUIRED_ARGUMENT]).each do |opt, arg|
  $internal_valgrind = arg if (opt == IV)
end

cmd = "test-prepost-cxx"
cmd = $internal_valgrind + " " + cmd if ($internal_valgrind)

result = `#{cmd}`
if (!($?.exited? && $?.exitstatus == 0))
  STDERR.print cmd
  STDERR.print " exited with code ", $?.exitstatus.to_s if $?.exited?
  STDERR.print " terminated with signal ", $?.termsig.to_s if $?.signaled?
  STDERR.print " (core dumped)" if $?.coredump?
  STDERR.print "\n"
  exit 1
end

expected = <<ADIOS
x = 5
Goodbye!
ADIOS

result = result.gsub(/\r\n/,"\n")
if (result != expected)
  STDERR.print "while running ", cmd, "\n"
  STDERR.print "expected:\n", expected, "but got:\n", result
  exit 2
end
