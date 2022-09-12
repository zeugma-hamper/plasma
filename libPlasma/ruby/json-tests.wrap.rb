#!/usr/bin/ruby
$VERBOSE = true
require_relative './test-helpers.rb'
SCRIPT = "json-tests.rb"
GET_OUT_OF_JAIL_FREE = "OBLONG_MACHINE_SETUP_MISSING_PACKAGES"
begin
  require 'rubygems'
  require 'json'
rescue LoadError
  if ENV.has_key?(GET_OUT_OF_JAIL_FREE) or is_105?
    # If someone explicitly chose not to install all the packages,
    # then we'll give them a break about not having json.
    parse_iv
    obsystem("echo skipping #{SCRIPT} because no json gem")
  else
    jsongem = is_185? ? "json_pure" : "json"
    msg = ["The json rubygem is required for this test.",
           "You can install it with 'sudo gem install #{jsongem}'",
           "Or you can force this test to pass without actually",
           "running by setting the environment variable:",
           GET_OUT_OF_JAIL_FREE + "=ignore"]
    len = msg.map {|x| x.length}.max
    edge = "+-" + ("-" * len) + "-+\n"
    STDERR.print edge
    msg.each {|x| STDERR.print "| ", x.ljust(len), " |\n"}
    STDERR.print edge
    exit 1
  end
else
  obwrap(SCRIPT)
end
