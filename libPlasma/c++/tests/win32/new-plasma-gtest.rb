#!/usr/bin/env ruby
$VERBOSE = true

# Create a new project file for a loam++ test (in libLoam/c++/tests)
# that uses Google Test.

require File.dirname(__FILE__) +
  '/../../../../libPlasma/c/win32/project-helpers.rb'

die_usage if (ARGV.length != 1)

create_project(ARGV[0], File.dirname(__FILE__) + '/HoseNthTest.vcxproj',
               '7E0DE475-AACE-4E6A-8F6E-87A2ADAC93CC', 'HoseNthTest',
               'HoseNthTest.cpp')
