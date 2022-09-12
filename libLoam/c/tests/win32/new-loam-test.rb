#!/usr/bin/env ruby
$VERBOSE = true

# Create a new project file for a loam test (in libLoam/c/tests)

require File.dirname(__FILE__) +
  '/../../../../libPlasma/c/win32/project-helpers.rb'

die_usage if (ARGV.length != 1)

create_project(ARGV[0], File.dirname(__FILE__) + '/test-rand.vcxproj',
               'D67E788A-468A-11DF-9829-0030487F5D3C', 'test-rand',
               'test-rand.c')
