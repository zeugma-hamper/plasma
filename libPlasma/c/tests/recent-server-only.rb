#!/usr/bin/ruby
$VERBOSE = true
require File.dirname(__FILE__) + '/test-helpers.rb'
if old_tcp_server?
  # Make test system happy by running *something* under valgrind
  obsystem("true")
else
  obsystem "./RecentServerOnly"
end
