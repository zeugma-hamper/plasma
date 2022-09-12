# -*- coding: utf-8 -*-

require_relative './test-helpers.rb'

require 'Pool'
include Plasma

PNAME = formulate_pool_name("ruby_pool_creation_and_deletion_test")
PNAME_A = PNAME + "_a"
PNAME_B = PNAME + "_b"

OPTIONS = { "resizable" => true }

def my_list_pools
  server = nil
  if ENV.has_key?(TEST_POOL)
    if (ENV[TEST_POOL] =~ %r{^([^/]+://[^/]+/)})
      server = $1
    end
  end
  pools = Pool.list_pools(server)
  if server
    return pools.map {|x| server + x}
  else
    return pools
  end
end

class PoolTest < Test::Unit::TestCase

  def setup
    @feeders = Array.new
  end

  def teardown
    @feeders.each {|pid| Process.wait(pid)}
    @feeders = Array.new
  end

  def test_creation_and_deletion
    pname = PNAME

    if my_list_pools.include? pname
      Pool.dispose pname
    end

    count = my_list_pools.size

    Pool.create pname, OPTIONS
    assert( my_list_pools.include?(pname), "apparently didn't create pool" )
    assert_equal( count+1, my_list_pools.size )

    Pool.dispose pname
    assert( !my_list_pools.include?(pname), "apparently didn't delete pool" )
    assert_equal( count, my_list_pools.size )
  end

  def test_multi_pool_await
    # delete pools so we start fresh
    begin
      Pool.dispose PNAME_A
      Pool.dispose PNAME_B
    rescue PoolNotFoundError
      # don't worry about it, you know
    end

    Pool.create PNAME_A, OPTIONS
    Pool.create PNAME_B, OPTIONS

    h = HoseGang.new PNAME_A, PNAME_B

    @feeders = spawn_feeders([[PNAME_A, 0.0], [PNAME_B, 0.25]])

    count = 0
    counts = Hash.new(0)

    while (prot = h.next 3.14159)
      assert_equal( prot.ingests["count"], next_expected(counts, prot.hose),
                    "protein counts out of order" )
      count = count+1
    end

    assert_equal( 12, count, "did not get 12 proteins from our forked feeders" )
    h.withdraw

    Pool.dispose PNAME_A
    Pool.dispose PNAME_B
  end

  def test_fetch
    pname = PNAME

    if my_list_pools.include? pname
      Pool.dispose pname
    end

    count = my_list_pools.size

    Pool.create pname, OPTIONS
    assert( my_list_pools.include?(pname), "apparently didn't create pool" )
    assert_equal( count+1, my_list_pools.size )

    h = Pool.participate pname
    assert( h.is_empty? )
    # we try to read 10 but we should get nothing
    @proteins = h.fetch(10)
    assert_equal( 0, @proteins.length )
    0.upto(4) do |n|
      h.deposit Protein.new(["test_nth"], { "n" => n})
      assert_equal( n, h.newest_index )
    end
    assert( !h.is_empty? )
    # we try to read 10 but only 5 should be returned
    @proteins = h.fetch(10)
    assert_equal( 5, @proteins.length )
    n = 0
    @proteins.each {|p|
      assert_equal( n, p.ingests["n"] )
      assert_equal( n, p.index )
      n = n+1
    }
    # we try to read 10, starting at 2, so we should only read 3
    @proteins = h.fetch(10, 2)
    assert_equal( 3, @proteins.length )
    n = 2
    @proteins.each {|p|
      assert_equal( n, p.ingests["n"] )
      assert_equal( n, p.index )
      n = n+1
    }

    h.withdraw

    Pool.dispose pname
    assert( !my_list_pools.include?(pname), "apparently didn't delete pool" )
    assert_equal( count, my_list_pools.size )
  end

  def test_nth
    pname = PNAME

    if my_list_pools.include? pname
      Pool.dispose pname
    end

    count = my_list_pools.size

    Pool.create pname, OPTIONS
    assert( my_list_pools.include?(pname), "apparently didn't create pool" )
    assert_equal( count+1, my_list_pools.size )

    h = Pool.participate pname
    h.name = "Ramón Estévez"
    assert( h.is_empty? )
    old_size = h.size_used
    # old servers don't report size, so it will be nil
    size_supported = ! old_size.nil?
    if size_supported
      assert_equal( old_size, h.size_used )
    end
    0.upto(5) do |n|
      h.deposit Protein.new(["test_nth"], { "n" => n})
      assert_equal( n, h.newest_index )
      if size_supported
        new_size = h.size_used
        assert(old_size < new_size)
        old_size = new_size
      end
    end
    assert( !h.is_empty? )
    0.upto(5) do |n|
      p = h.nth(n)
      assert_equal(n, p.ingests["n"])
      assert_equal(n, p.index)  # bug 2662
    end

    if size_supported
      assert( h.size > h.size_used )
    end
    assert_equal( "Ramón Estévez", h.name )
    assert_equal( 0, h.oldest_index )
    begin
      h.oldest_index = 2
    rescue PoolUnsupportedError
      # This will happen on older servers.  Allow it.
    else
      assert_equal( 2, h.oldest_index )
      assert_raises(PoolNoProteinError) { h.nth(1) }
    end
    h.withdraw

    Pool.dispose pname
    assert( !my_list_pools.include?(pname), "apparently didn't delete pool" )
    assert_equal( count, my_list_pools.size )
  end

  def test_sync
    pname = PNAME

    if my_list_pools.include? pname
      Pool.dispose pname
    end

    count = my_list_pools.size

    Pool.create pname, OPTIONS
    assert( my_list_pools.include?(pname), "apparently didn't create pool" )
    assert_equal( count+1, my_list_pools.size )

    h = Pool.participate pname
    0.upto(5) do |n|
      h.deposit Protein.new(["test_nth"], { "n" => n})
      assert_equal( n, h.newest_index )
      sy = ((n % 2) == 0)
      begin
        h.sync = sy
      rescue PoolUnsupportedError
        # This will happen on older servers.  Allow it.
      else
        assert_equal( sy, h.sync )
      end
    end

    0.upto(5) do |n|
      p = h.nth(n)
      assert_equal(n, p.ingests["n"])
    end

    h.withdraw

    Pool.dispose pname
    assert( !my_list_pools.include?(pname), "apparently didn't delete pool" )
    assert_equal( count, my_list_pools.size )
  end

  def test_frozen
    pname = PNAME

    if my_list_pools.include? pname
      Pool.dispose pname
    end

    count = my_list_pools.size

    Pool.create pname, OPTIONS
    assert( my_list_pools.include?(pname), "apparently didn't create pool" )
    assert_equal( count+1, my_list_pools.size )

    h = Pool.participate pname
    0.upto(2) do |n|
      h.deposit Protein.new(["test_frozen"], {"n" => n})
    end

    # Because resizable also implies freezable, auto-disposable
    if ! h.resizable?
      h.withdraw
      Pool.dispose pname
      return
    end

    assert( ! h.frozen )
    h.frozen = true
    assert( h.frozen )

    assert_raises(PoolFrozenError) do
      h.deposit Protein.new(["test_frozen"], {"n" => 10})
    end

    h.frozen = false
    assert( ! h.frozen )

    3.upto(5) do |n|
      h.deposit Protein.new(["test_frozen"], {"n" => n})
    end

    0.upto(5) do |n|
      p = h.nth(n)
      assert_equal(n, p.ingests["n"])
    end

    assert( ! h.auto_dispose )
    h.auto_dispose = true
    assert( h.auto_dispose )
    h.withdraw

    assert( !my_list_pools.include?(pname), "apparently didn't delete pool" )
    assert_equal( count, my_list_pools.size )
    assert_raises(PoolNotFoundError) { Pool.dispose pname }
  end

  def test_resize
    pname = PNAME

    if my_list_pools.include? pname
      Pool.dispose pname
    end

    count = my_list_pools.size

    Pool.create pname, OPTIONS
    assert( my_list_pools.include?(pname), "apparently didn't create pool" )
    assert_equal( count+1, my_list_pools.size )

    h = Pool.participate pname

    if ! h.resizable?
      h.withdraw
      Pool.dispose pname
      return
    end

    h.auto_dispose = true

    0.upto(2) do |n|
      h.deposit Protein.new(["test_resize"], {"n" => n})
    end

    desired_size = 40 * 1024
    h.size = desired_size
    assert_equal( desired_size, h.size )

    3.upto(5) do |n|
      h.deposit Protein.new(["test_resize"], {"n" => n})
    end

    0.upto(5) do |n|
      p = h.nth(n)
      assert_equal(n, p.ingests["n"])
    end

    h.withdraw

    assert( !my_list_pools.include?(pname), "apparently didn't delete pool" )
    assert_equal( count, my_list_pools.size )
    assert_raises(PoolNotFoundError) { Pool.dispose pname }
  end

  def test_withdraw
    begin
      Pool.dispose PNAME
    rescue PoolNotFoundError
      # don't worry about it, you know
    end

    Pool.create PNAME, OPTIONS
    h = Pool.participate PNAME
    h.deposit Protein.new(["foo"], { "bar" => "baz"})
    h.withdraw
    assert_raises(PoolWithdrawnError) { h.deposit Protein.new(["foo"], { "bar" => "baz"}) }

    Pool.dispose PNAME
  end

  def test_bad_withdraw
    # delete pools so we start fresh
    begin
      Pool.dispose PNAME_A
      Pool.dispose PNAME_B
    rescue PoolNotFoundError
      # don't worry about it, you know
    end

    Pool.create PNAME_A, OPTIONS
    Pool.create PNAME_B, OPTIONS

    p1 = Pool.participate PNAME_A
    p2 = Pool.participate PNAME_B

    h = HoseGang.new p1, p2

    assert_raises(PoolOperationError) { p1.withdraw }

    h.withdraw
    Pool.dispose PNAME_A
    Pool.dispose PNAME_B
  end

  def test_rename
    # delete pools so we start fresh
    begin
      Pool.dispose PNAME_A
      Pool.dispose PNAME_B
    rescue PoolNotFoundError
      # don't worry about it, you know
    end

    count = my_list_pools.size

    Pool.create PNAME_A, OPTIONS
    assert( my_list_pools.include?(PNAME_A), "apparently didn't create pool" )
    assert_equal( count+1, my_list_pools.size )

    begin
      Pool.rename(PNAME_A, PNAME_B)
    rescue PoolUnsupportedError
      assert( my_list_pools.include?(PNAME_A), "apparently didn't not rename pool" )
      assert_equal( count+1, my_list_pools.size )
      Pool.dispose PNAME_A
    else
      assert( my_list_pools.include?(PNAME_B), "apparently didn't rename pool" )
      assert_equal( count+1, my_list_pools.size )
      Pool.dispose PNAME_B
    end

    assert( !my_list_pools.include?(PNAME_A), "apparently didn't delete pool" )
    assert( !my_list_pools.include?(PNAME_B), "apparently didn't delete pool" )
    assert_equal( count, my_list_pools.size )
  end

  def test_rename_exists
    # delete pools so we start fresh
    begin
      Pool.dispose PNAME_A
      Pool.dispose PNAME_B
    rescue PoolNotFoundError
      # don't worry about it, you know
    end

    count = my_list_pools.size

    Pool.create PNAME_A, OPTIONS
    assert( my_list_pools.include?(PNAME_A), "apparently didn't create pool" )
    assert_equal( count+1, my_list_pools.size )

    Pool.create PNAME_B, OPTIONS
    assert( my_list_pools.include?(PNAME_B), "apparently didn't create pool" )
    assert_equal( count+2, my_list_pools.size )

    assert_raises(PoolExistsError, PoolUnsupportedError) { Pool.rename(PNAME_A, PNAME_B) }

    Pool.dispose PNAME_B
    assert( !my_list_pools.include?(PNAME_B), "apparently didn't delete pool" )
    assert_equal( count+1, my_list_pools.size )

    Pool.dispose PNAME_A
    assert( !my_list_pools.include?(PNAME_A), "apparently didn't delete pool" )
    assert_equal( count, my_list_pools.size )
  end

  def test_rename_not_found
    # delete pools so we start fresh
    begin
      Pool.dispose PNAME_A
      Pool.dispose PNAME_B
    rescue PoolNotFoundError
      # don't worry about it, you know
    end

    assert_raises(PoolNotFoundError, PoolUnsupportedError) { Pool.rename(PNAME_A, PNAME_B) }
  end

  def test_in_use
    begin
      Pool.dispose PNAME
    rescue PoolNotFoundError
      # don't worry about it, you know
    end

    assert_raises(PoolNotFoundError, PoolUnsupportedError) { Pool.is_in_use?(PNAME) }
    Pool.create PNAME, OPTIONS

    begin
      assert( !Pool.is_in_use?(PNAME) )
    rescue PoolUnsupportedError
      # nothing
    else
      h = Pool.participate PNAME
      assert( Pool.is_in_use?(PNAME) )
      h.withdraw
      assert( !Pool.is_in_use?(PNAME) )
    end

    Pool.dispose PNAME
    assert_raises(PoolNotFoundError, PoolUnsupportedError) { Pool.is_in_use?(PNAME) }
  end

end
