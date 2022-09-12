#!/usr/bin/ruby

$VERBOSE = true

require File.dirname(__FILE__) + '/../tests/test-helpers.rb'

P_CREATE = "../p-create"
P_STOP = "../p-stop"
TEST_POOL = ENV["TEST_POOL"]
OB_POOLS_DIR = ENV["OB_POOLS_DIR"]
TEST_POOL_DIR = OB_POOLS_DIR + "/" + TEST_POOL

# shuffle is only in 1.9
# sort_by {rand} is okay, since the random numbers are generated once
# sort {rand} would, however, be the infamous "browser ballot bug"
#
# And the reason for filtering out groups that start with com.apple:
# Well, I'm not really sure, but there's a group on hyacinth called
# com.apple.sharepoint.group.7, and whenever we randomly select it,
# the test fails.  That's probably what I deserve for picking groups
# at random.  But to avoid this problem (without really understanding
# the cause), we only choose groups that don't start with com.apple.
GROUPS = `groups`.split.reject {|x| x =~ /^com\.apple/}.sort_by {rand}

# Just do a couple randomly-chosen groups; that should be enough
GROUPS[0..1].each do |group|
  obsystem(P_CREATE + " -z -m 770 -g " + group + " " + TEST_POOL)
  line = `ls -ld #{TEST_POOL_DIR}`
  fields = line.split
  expected = "drwxrwx---"
  # Only examine first ten characters, because Fedora gives us "drwxrwx---."
  actual = fields[0][0..9]
  if actual != expected
    STDERR.print "expected #{expected} but got #{actual}\n"
    exit 1
  end
  if fields[3] != group
    STDERR.print "expected #{group} but got #{fields[3]}\n"
    exit 1
  end
end

obsystem(P_STOP + " " + TEST_POOL)
