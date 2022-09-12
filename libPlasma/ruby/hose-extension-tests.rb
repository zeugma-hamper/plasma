
require_relative './test-helpers.rb'

require 'Pool'
include Plasma


PNAME = formulate_pool_name("ruby_pool_extension_test")
PNAME_A = PNAME + "_a"
PNAME_B = PNAME + "_b"

class TimedGeneratorHose
  attr_accessor :name
  attr_accessor :last_trigger_time
  attr_accessor :delay_time
  attr_accessor :block

  def initialize(name, delay_time, &block)
    @last_trigger_time = Time.now
    @name = name
    @delay_time = delay_time
    @block = block
    @_pool_hose = nil
  end

  def next(timeout=NO_WAIT)
    now = Time.now
    nxt = next_trigger_time
    if (now >= nxt)
      @last_trigger_time = Time.now
      prt = block.call self
      prt.hose = self  if  prt
      return prt
    end
    if ((now + timeout) >= nxt)
      sleep nxt - now
      @last_trigger_time = Time.now
      prt = block.call self
      prt.hose = self  if  prt
      return prt
    end
    if (timeout > NO_WAIT)
      sleep timeout
    end
    return nil
  end

  def next_trigger_time
    @last_trigger_time + delay_time
  end

  def add_trib__dispatch(gang)
    if gang._awaiter.class == TimeCognizantAwaiter
      gang._awaiter.add_hose self
    elsif gang._awaiter.class == GangBasicAwaiter
      gang._awaiter = TimeCognizantAwaiter.new gang
      gang._awaiter.add_hose self
    else
      raise StandardError,
      "TimedGeneratorHose doesn't know how \n" +
        "to frobulate an awaiter of class #{gang._awaiter.class}"
    end
  end

end

class TimeCognizantAwaiter < GangBasicAwaiter
  attr_accessor :time_aware_tributaries
  def initialize(containing_gang)
    @containing_gang_ref = WeakRef.new containing_gang
    @_hose_gang = containing_gang._awaiter._hose_gang
    @time_aware_tributaries = Array.new
  end

  def next(timeout)
    prt = super  NO_WAIT
    return prt  if  prt

    now = Time.now
    nxt_timed_hose, nxt_time = timed_awaiter_next_trigger

    if nxt_time
      if (now < nxt_time)
        prt = super( nxt_time - now )
      else
        prt = nil
      end
      if prt
        return prt
      else
        prt = nxt_timed_hose.next
        # See bug 597 comment 3 for why we need to do this; prt could be nil!
        if prt or timeout == NO_WAIT
          return prt
        elsif timeout == WAIT_FOREVER
          return self.next(WAIT_FOREVER)
        else
          newto = timeout - (Time.now - now)
          if newto > 0
            return self.next(newto)
          else
            return self.next(NO_WAIT)
          end
        end
      end
    else
      return super(timeout)
    end

  end

  def add_hose(hose)
    if hose.respond_to? :next_trigger_time
      time_aware_tributaries.push hose
    else
      super
    end
  end

  def remove_hose(hose)
    if hose.respond_to? :next_trigger_time
      time_aware_tributaries.delete hose
    else
      super
    end
  end

  def timed_awaiter_next_trigger
    nxt = time_aware_tributaries.min do |a,b|
      a.next_trigger_time <=> b.next_trigger_time
    end
    return nxt  ?  [nxt, nxt.next_trigger_time]  :  nil
  end
end


class TimePseudoHoseTest < Test::Unit::TestCase

  def setup
    @feeders = Array.new
  end

  def teardown
    @feeders.each {|pid| Process.wait(pid)}
    @feeders = Array.new
    # make sure all hoses withdraw, since they only withdraw when collected
    GC.start
  end

  def test_interleaved_pseudo_and_real_hoses
    pname_a = PNAME_A
    pname_b = PNAME_B

    begin
      Pool.dispose pname_a
      Pool.dispose pname_b
    rescue PoolNotFoundError
      # don't worry about it, you know
    end

    Pool.create pname_a
    Pool.create pname_b

    h = HoseGang.new pname_a, pname_b

    g_count = 0
    g = TimedGeneratorHose.new( "gen-test", 0.49 ) do
      if g_count < 6
        prot = Protein.new( ["g-test-feeder"],
                            { "count" => g_count } )
        g_count = g_count + 1
        prot
      else
        nil
      end
    end
    h.add_tributary g

    @feeders = spawn_feeders([[pname_a, 0.10], [pname_b, 0.25]])
    count = 0
    counts = Hash.new(0)

    while (prot = h.next 3.14159)
      assert_equal( prot.ingests["count"], next_expected(counts, prot.hose),
                    "protein counts out of order" )
      count = count+1
    end

    assert_equal( 6, g_count )
    assert_equal( 18, count, "did not get 18 proteins from our forked feeders" )
    h.withdraw
  end


  def test_only_real_hoses_with_pseudos_awaiter
    pname_a = PNAME_A
    pname_b = PNAME_B

    begin
      Pool.dispose pname_a
      Pool.dispose pname_b
    rescue PoolNotFoundError
      # don't worry about it, you know
    end

    Pool.create pname_a
    Pool.create pname_b

    h = HoseGang.new pname_a, pname_b

    g_count = 0
    g = TimedGeneratorHose.new( "gen-test", 0.49 ) do
      if g_count < 6
        prot = Protein.new( ["g-test-feeder"],
                            { "count" => g_count } )
        g_count = g_count + 1
        prot
      else
        nil
      end
    end
    h.add_tributary g
    assert_equal(true, h.remove_tributary(g))

    @feeders = spawn_feeders([[pname_a, 0.10], [pname_b, 0.25]])
    count = 0
    counts = Hash.new(0)

    while (prot = h.next 3.14159)
      assert_equal( prot.ingests["count"], next_expected(counts, prot.hose),
                    "protein counts out of order" )
      count = count+1
    end

    assert_equal( 0, g_count )
    assert_equal( 12, count, "did not get 12 proteins from our forked feeders" )
    h.withdraw
  end

  def test_multiple_psuedo_hoses
    pname_a = PNAME_A
    pname_b = PNAME_B

    begin
      Pool.dispose pname_a
      Pool.dispose pname_b
    rescue PoolNotFoundError
      # don't worry about it, you know
    end

    Pool.create pname_a
    Pool.create pname_b

    h = HoseGang.new pname_a, pname_b

    e_count = 0
    e = TimedGeneratorHose.new( "een-test", 0.11 ) do
      if e_count < 6
        prot = Protein.new( ["e-test-feeder"],
                            { "count" => e_count } )
        e_count = e_count + 1
        prot
      else
        nil
      end
    end
    h.add_tributary e

    f_count = 0
    f = TimedGeneratorHose.new( "fen-test", 0.53 ) do
      if f_count < 6
        prot = Protein.new( ["f-test-feeder"],
                            { "count" => f_count } )
        f_count = f_count + 1
        prot
      else
        nil
      end
    end
    h.add_tributary f

    g_count = 0
    g = TimedGeneratorHose.new( "gen-test", 0.49 ) do
      if g_count < 6
        prot = Protein.new( ["g-test-feeder"],
                            { "count" => g_count } )
        g_count = g_count + 1
        prot
      else
        nil
      end
    end
    h.add_tributary g

    @feeders = spawn_feeders([[pname_a, 0.10], [pname_b, 0.25]])
    count = 0
    counts = Hash.new(0)

    while (prot = h.next 3.14159)
      assert_equal( prot.ingests["count"], next_expected(counts, prot.hose),
                    "protein counts out of order" )
      count = count+1
    end

    assert_equal( 30, count, "did not get 30 proteins from our forked feeders" )
    h.withdraw
  end
end
