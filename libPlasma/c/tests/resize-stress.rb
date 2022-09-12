#!/usr/bin/ruby

$VERBOSE = true

require File.dirname(__FILE__) + '/test-helpers.rb'

TEST_POOL = ENV["TEST_POOL"]
POOL_TYPE = ENV["POOL_TYPE"]
POOL_SIZE = ENV["POOL_SIZE"]
POOL_TOC_CAPACITY = ENV["POOL_TOC_CAPACITY"]
# TEST_LOG was already set by test-helpers.rb

TEST_POOLS = (1..5).map {|n| TEST_POOL + n.to_s}.join(" ")
TEST_ARGS = "-r -t #{POOL_TYPE} -s #{POOL_SIZE} -i #{POOL_TOC_CAPACITY} -S 1 -j 3 #{TEST_POOLS}"

if old_tcp_server?
  # Make test system happy by running *something* under valgrind
  obsystem("true")
  exit 0
end

# Darn Windows.  And darn Ruby.  Both.  On Windows, Ruby allows
# forward slash in system() as long as there are no shell metacharacters.
# (This is why "../" is okay for the second command below, since it doesn't
# redirect output.)  But if the command contains metacharacters (like to
# redirect output to the test log), then only backslash is allowed.
# Therefore, we have to determine the correct slash (forward for UNIX
# and backward for Windows).  Ruby doesn't even make this particularly
# easy to do, since we have to check two different constants.
# OR ... use PATH.
ENV['PATH'] = '.' + File::PATH_SEPARATOR + '..' + File::PATH_SEPARATOR + ENV['PATH']
obsystem("pingpong_test #{TEST_ARGS} >> #{TEST_LOG}")
obsystem("p-stop -q #{TEST_POOLS}")
