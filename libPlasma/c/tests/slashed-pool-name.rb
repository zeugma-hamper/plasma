#!/usr/bin/ruby

$VERBOSE = true

require File.dirname(__FILE__) + '/test-helpers.rb'

ENV['PATH'] = '..' + File::PATH_SEPARATOR + ENV['PATH']

TEST_POOL = ENV["TEST_POOL"]
POOL_TYPE = ENV["POOL_TYPE"]
POOL_SIZE = ENV["POOL_SIZE"]
POOL_TOC_CAPACITY = ENV["POOL_TOC_CAPACITY"]
# TEST_LOG was already set by test-helpers.rb
# TEST_LOG = ENV["TEST_LOG"]
POOLNAME="#{TEST_POOL}dir/a/man/a/plan/a/canal/panama"

if old_tcp_server?
  # Make test system happy by running *something* under valgrind
  obsystem("true")
  exit 0
end

# Create a pool
obsystem("p-create -t #{POOL_TYPE} -s #{POOL_SIZE} -i #{POOL_TOC_CAPACITY} #{POOLNAME}")

# And stick something in it
obsystem("p-deposit -d a_descrip -i a_key:a_value #{POOLNAME}")

# There should be something in it
obbacktick("p-oldest-idx #{POOLNAME}")

# Oh yeah, now clean up...
obsystem("p-stop #{POOLNAME}")
