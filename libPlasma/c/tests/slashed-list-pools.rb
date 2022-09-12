#!/usr/bin/ruby

$VERBOSE = true

require File.dirname(__FILE__) + '/test-helpers.rb'

ENV['PATH'] = '.' + File::PATH_SEPARATOR + '..' + File::PATH_SEPARATOR + ENV['PATH']

P_CREATE = "p-create"
P_STOP = "p-stop"
P_LIST = "p-list"
TEST_POOL = ENV["TEST_POOL"]
POOL_SERVER = TEST_POOL.sub(%r([^/]+$), "")

POOLS_TEXT = <<THATS_ALL_FOLKS
a/man/a/plan/9/from/outer/space
a/man/a/plan/a/canal+/company
a/man/a/plan/a/canal/canada
a/man/a/plan/a/canal/panama
a/man/page
a/panama/canal
a/panama/hat
a/serious/man
a/sirius/xm/man
THATS_ALL_FOLKS

# shuffle is only in 1.9
# sort_by {rand} is okay, since the random numbers are generated once
# sort {rand} would, however, be the infamous "browser ballot bug"
# http://www.robweir.com/blog/2010/02/microsoft-random-browser-ballot.html
POOLS = POOLS_TEXT.split.sort_by {rand}.collect {|x| POOL_SERVER + x}

if old_tcp_server?
  # Make test system happy by running *something* under valgrind
  obsystem("true")
  exit 0
end

# This also tests creating multiple pools in one command (bug 693)
obsystem(P_CREATE + " " + POOLS.join(" "))

exitcode = 0
listing = obbacktick(P_LIST + " " + POOL_SERVER)
if (listing != POOLS_TEXT)
  STDERR.print "expected:\n", POOLS_TEXT, "but got:\n", listing
  exitcode = 1
end

# Now try just listing a subdirectory
SUBDIR_TEXT = <<THATS_ALL_FOLKS
9/from/outer/space
a/canal+/company
a/canal/canada
a/canal/panama
THATS_ALL_FOLKS
listing = obbacktick(P_LIST + " " + POOL_SERVER + "a/man/a/plan/")
if (listing != SUBDIR_TEXT)
  STDERR.print "expected:\n", SUBDIR_TEXT, "but got:\n", listing
  exitcode = 1
end

obsystem(P_STOP + " " + POOLS.join(" "))
exit exitcode
