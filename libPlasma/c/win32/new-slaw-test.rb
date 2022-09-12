#!/usr/bin/env ruby
$VERBOSE = true

# Create a new project file for a slaw test (in libPlasma/c/t)
# that does not use Google Test.

require File.dirname(__FILE__) + '/project-helpers.rb'

die_usage if (ARGV.length != 1)

create_project(ARGV[0], File.dirname(__FILE__) + '/ins-rep.vcxproj',
               'FFFE1219-E399-1001-AD7F-90F4825DF254', 'insrep',
               'ins-rep.c')
