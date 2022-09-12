# (c) 2010, 2015 Oblong Industries

# test/unit went away in Ruby 2.0, but we still need to build on ruby 1.x
begin
  require 'minitest/autorun'
  Test = MiniTest
  module MiniTest
    # Allow assert_not_nil, even though it's obsolete
    module Assertions
      alias assert_not_nil refute_nil
    end
  end
rescue LoadError
  require 'test/unit'
  MiniTest = Test
  module Test
    Assertions = Unit::Assertions
    # Allow refute_nil, even though it's from the future
    module Assertions
      alias refute_nil assert_not_nil
    end
  end
end

def is_mac?
  RbConfig::CONFIG['host_os'] =~ /^darwin/i
end

# Check for Mac OS X 10.5 (where "gem" command is broken)
def is_105?
  RbConfig::CONFIG['host_os'] =~ /^darwin9/i
end

# Check for Ruby 1.8.5 or earlier (where json gem's C extension fails to build)
def is_185?
  RbConfig::CONFIG['MAJOR'] == "1" and
    RbConfig::CONFIG['MINOR'] == "8" and
    RbConfig::CONFIG['TEENY'].to_i <= 5
end
