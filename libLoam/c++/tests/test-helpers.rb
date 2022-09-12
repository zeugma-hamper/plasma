# (c) 2010 Oblong Industries

require 'getoptlong'

IV = "--internal-valgrind"
$internal_valgrind = nil

GetoptLong.new([IV, GetoptLong::REQUIRED_ARGUMENT]).each do |opt, arg|
  $internal_valgrind = arg if (opt == IV)
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

def obbacktick(cmd)
  fullcmd = cmd
  fullcmd = $internal_valgrind + " " + cmd if ($internal_valgrind)
  result = `#{fullcmd}`
  # Cygwin ruby running native windows apps doesn't know to strip the CR, so do it here
  result = result.gsub(/\r\n/,"\n")
  if (!($?.exited?))
    STDERR.print fullcmd
    STDERR.print " exited with code ", $?.exitstatus.to_s if $?.exited?
    STDERR.print " terminated with signal ", $?.termsig.to_s if $?.signaled?
    STDERR.print " (core dumped)" if $?.coredump?
    STDERR.print "\n"
    exit 1
  end
  return $?.exitstatus, result
end
