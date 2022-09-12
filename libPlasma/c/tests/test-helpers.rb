# (c)  Oblong Industries

require 'getoptlong'
require 'rbconfig'

ENV['PATH'] = '.' + File::PATH_SEPARATOR + '..' + File::PATH_SEPARATOR + ENV['PATH']

def is_windows?
  RbConfig::CONFIG['host_os'] == "mingw32"
end

def which(cmd)
  exts = ENV['PATHEXT'] ? ENV['PATHEXT'].split(';') : ['']
  ENV['PATH'].split(File::PATH_SEPARATOR).each do |path|
    exts.each { |ext|
      exe = File.join(path, "#{cmd}#{ext}")
      return exe if File.executable?(exe) && !File.directory?(exe)
    }
  end
  return nil
end

TCP_ENV = "OBLONG_TEST_USE_POOL_TCP_SERVER"
TCP_SERVER = which("pool_tcp_server")

if ENV.has_key?("TEST_LOG")
  TEST_LOG = ENV["TEST_LOG"]
else
  TEST_LOG = "/dev/fd/2"
end

# Returns true if we are using tcp pools, not local pools.
def using_tcp?
  ENV.has_key?(TCP_ENV)
end

# Returns true if we are using tcp pools in compatibility testing;
# i. e. the server may not support all the latest features.
def old_tcp_server?
  if using_tcp?
    if is_windows?
      tcp_server = ENV[TCP_ENV]
    else
      tcp_server = ENV[TCP_ENV].split(/:/)[0]
    end
    File.open(TEST_LOG, "a") {|x| x.print "old_tcp_server: TCP_SERVER ", TCP_SERVER, " tcp_server ", tcp_server, "\n"}
    stat1 = File.stat(TCP_SERVER)
    stat2 = File.stat(tcp_server)
    if (stat1.dev != stat2.dev || stat1.ino != stat2.ino)
      # We are running with a tcp server other than the one we were built with.
      # Therefore, it's not guaranteed that the version will be the latest one.
      File.open(TEST_LOG, "a") {|x| x.print "old_tcp_server: returning true\n"}
      return true
    end
  end
  File.open(TEST_LOG, "a") {|x| x.print "old_tcp_server: returning false\n"}
  return false
end

IV = "--internal-valgrind"
$internal_valgrind = nil

GetoptLong.new([IV, GetoptLong::REQUIRED_ARGUMENT]).each do |opt, arg|
  $internal_valgrind = arg if (opt == IV)
end

def freak_out(cmd, status)
  roof = String.new
  roof << cmd
  roof << " exited with code #{status.exitstatus}" if status.exited?
  roof << " terminated with signal #{status.termsig}" if status.signaled?
  roof << " (core dumped)" if status.coredump?
  raise roof
end

def obsystem(cmd)
  File.open(TEST_LOG, "a") {|x| x.print "obsystem: ", cmd, "\n"}
  fullcmd = cmd
  fullcmd = $internal_valgrind + " " + cmd if ($internal_valgrind)
  success = system(fullcmd)
  if (!success)
    freak_out(fullcmd, $?)
  end
end

def obbacktick(cmd)
  File.open(TEST_LOG, "a") {|x| x.print "obbacktick: ", cmd, "\n"}
  fullcmd = cmd
  fullcmd = $internal_valgrind + " " + cmd if ($internal_valgrind)
  result = `#{fullcmd}`
  # Cygwin ruby running native windows apps doesn't know to strip the CR, so do it here
  result = result.gsub(/\r\n/,"\n")
  if (!($?.exited? && $?.exitstatus == 0))
    freak_out(fullcmd, $?)
  end
  return result
end
