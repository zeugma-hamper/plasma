# -*- coding: utf-8 -*-

require_relative './test-helpers.rb'

require 'Pool'
include Plasma

PNAME = formulate_pool_name("unit-test-pool-")

def with_hose name
  Pool.create name
  hose = Hose.new name
  yield hose
  hose.withdraw
  Pool.dispose name
end

def after_depositing
  with_hose PNAME do | hose |
    2048.times do |counter|
      hose.deposit Protein.new([counter], {})
    end
    yield hose
  end
end

class PoolTest < Test::Unit::TestCase
  def test_3787
    after_depositing do | hose |
      2048.times do
        hose.next
      end
    end
  end
end
