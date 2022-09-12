# (c) 2010 Oblong Industries

require_relative './test-helpers.rb'

require 'Pool'
include Plasma

Pool.dispose(formulate_pool_name("ruby_plasma_unit_tests"))
Pool.dispose(formulate_pool_name("ruby_pool_extension_test_a"))
Pool.dispose(formulate_pool_name("ruby_pool_extension_test_b"))
