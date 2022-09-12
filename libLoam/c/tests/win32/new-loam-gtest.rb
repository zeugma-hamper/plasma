#!/usr/bin/env ruby
$VERBOSE = true

# Create a new project file for a loam test (in libLoam/c/tests)
# that uses Google Test.

require File.dirname(__FILE__) +
  '/../../../../libPlasma/c/win32/project-helpers.rb'

die_usage if (ARGV.length != 1)

create_project(ARGV[0], File.dirname(__FILE__) + '/TestFile.vcxproj',
               '96BA2408-B0BE-11DF-9829-0030487F5D3C', 'TestFile',
               'TestFile.cpp')
