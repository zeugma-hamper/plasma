#!/usr/bin/ruby
$VERBOSE = true
require_relative './test-helpers.rb'
# Suppress this logging message, printed by test_illegal_yaml:
# error: <2000500c> (1/2): Scanner error: while scanning a quoted scalar at line 1, column 1
# error: <2000500c> (2/2): found unexpected end of stream at line 1, column 17
modify_logging_configuration("ERRU 2000500c=off")
obwrap("slaw-tests.rb")
