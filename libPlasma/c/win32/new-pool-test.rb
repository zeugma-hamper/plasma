#!/usr/bin/env ruby
$VERBOSE = true

# Create a new project file for a pool test (in libPlasma/c/tests)
# that does not use Google Test.

require File.dirname(__FILE__) + '/project-helpers.rb'

die_usage if (ARGV.length != 1)

create_project(ARGV[0], File.dirname(__FILE__) + '/rewind_test.vcxproj',
               '4406ECBE-A001-4516-93EC-08A5EFC9F058', 'rewindtest',
               'rewind_test.c')
