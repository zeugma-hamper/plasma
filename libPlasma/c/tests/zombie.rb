#!/usr/bin/ruby
require 'yaml'

$VERBOSE = true

require File.dirname(__FILE__) + '/test-helpers.rb'

ENV['PATH'] = '.' + File::PATH_SEPARATOR + '..' + File::PATH_SEPARATOR + ENV['PATH']

TEST_POOL = ENV["TEST_POOL"]
POOL_TYPE = ENV["POOL_TYPE"]
POOL_SIZE = ENV["POOL_SIZE"]
POOL_TOC_CAPACITY = ENV["POOL_TOC_CAPACITY"]
# TEST_LOG was already set by test-helpers.rb

if old_tcp_server?
  # Make test system happy by running *something* under valgrind
  obsystem("true")
  exit 0
end

obsystem("zombie #{TEST_POOL}")
obsystem("p-create -z -s #{POOL_SIZE} #{TEST_POOL}")
begin
  info = obbacktick("p-info #{TEST_POOL}")
ensure
  obsystem("p-stop #{TEST_POOL}")
end

# extract last yaml document from info
bar = info.split(/\n/)
baz = Array.new

loop do
  s = bar.pop
  baz.unshift(s)
  break if s =~ /^---/
end

INFO = YAML.load(baz.join("\n") + "\n")
ACTUAL_SIZE = INFO["size"]
if ACTUAL_SIZE != (((POOL_SIZE.to_i + 4095) >> 12) << 12)
  STDERR.print "Expected '#{POOL_SIZE}' but got '#{ACTUAL_SIZE}'\n"
  exit 1
end
