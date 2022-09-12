#!/usr/bin/env ruby
$VERBOSE = true

# Create a new project file for a slaw test (in libPlasma/c/t)
# that uses Google Test.

require File.dirname(__FILE__) + '/project-helpers.rb'

die_usage if (ARGV.length != 1)

create_project(ARGV[0], File.dirname(__FILE__) + '/SlawDuplicatesTest.vcxproj',
               'FF39547C-3E4B-46E2-AE3D-CD17FC3987C3', 'SlawDuplicatesTest',
               'SlawDuplicatesTest.cpp')
