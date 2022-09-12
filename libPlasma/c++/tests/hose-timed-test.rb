#!/usr/bin/ruby
$VERBOSE = true

# tell it where to find tcp server
ENV['PATH'] = '../../c' + File::PATH_SEPARATOR + ENV['PATH']

require File.dirname(__FILE__) + '/../../c/tests/test-helpers.rb'

OLD = old_tcp_server?
if OLD
  # Make test system happy by running *something* under valgrind
  obsystem("true")
else
  obsystem "./HoseTimedTest"
end
