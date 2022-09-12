#!/usr/bin/env ruby
$VERBOSE = true

# Create a new project file for a pool test (in libPlasma/c/tests)
# that uses Google Test.

require File.dirname(__FILE__) + '/project-helpers.rb'

die_usage if (ARGV.length != 1)

create_project(ARGV[0], File.dirname(__FILE__) + '/IndexedPoolTest.vcxproj',
               '4406ECBE-D7BB-9BD2-93EC-08A5EFC9F058', 'IndexedPoolTest',
               'IndexedPoolTest.cpp')
