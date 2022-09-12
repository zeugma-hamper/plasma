#!/usr/bin/env ruby
$VERBOSE = true

# Create a new project file for a loam++ test (in libLoam/c++/tests)
# that uses Google Test.

require File.dirname(__FILE__) +
  '/../../../../libPlasma/c/win32/project-helpers.rb'

die_usage if (ARGV.length != 1)

create_project(ARGV[0], File.dirname(__FILE__) + '/BasicStrTest.vcxproj',
               '816DF7A7-F2DF-4B36-AF6D-E7CA307A1A40', 'BasicStrTest',
               'BasicStrTest.C')
