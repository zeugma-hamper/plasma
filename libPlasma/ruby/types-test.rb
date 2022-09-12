
require_relative './test-helpers.rb'

require 'Pool'
include Plasma

PNAME = formulate_pool_name("ruby_plasma_unit_tests")

#
# setup slaw types and test constants
#

$test_array_size = 7

$int_sizes = [8, 16, 32, 64]

$int_tags     = $int_sizes.map { |i| ("int" + i.to_s).to_sym }
$unt_tags     = $int_sizes.map { |i| ("unt" + i.to_s).to_sym }

$integer_tags = $int_tags + $unt_tags
$float_tags   = [32, 64].map { |f| ("float" + f.to_s).to_sym }

$basic_numeric_tags = $integer_tags + $float_tags

def vect_tag(tag, n)
  ("v" + n.to_s + tag.to_s).to_sym
end

def array_tag(tag)
  (tag.to_s + "_array").to_sym
end


#
# run a bit of c code to set up a pool with proteins containing all
# the slaw types we support, so we can fetch, deconstruct and test
# against them
#

system "c-test-feeder #{PNAME}"


def check_respond_to(s, meths_map)
  meths_map.each_pair do |meth, bool|
    assert_equal( bool, s.respond_to?(meth), ":#{meth} on #{s.type_tag}" )
    if bool
      if (meth == :each)
        s.method(meth).call {}
      elsif
        (meth == :[]  or  meth == :nth_slaw)
        s.method(meth).call 0
      else
        s.method(meth).call
      end
    else
      begin
        # it would be nice if all these illegal duck-typed methods
        # throw the SlawTypeNoMethodError rather than an
        # ArgumentError, no matter how they are invoked. maybe some
        # more fiddling in Slaw.rb could accomplish that (gracefully,
        # harumph).
        #   p "calling :#{meth} on #{s.type_tag}"
        if (meth == :each)
          s.method(meth).call {}
        elsif
          (meth == :[]  or  meth == :nth_slaw)
          s.method(meth).call 0
        else
          s.method(meth).call
        end
      rescue SlawTypeNoMethodError
        # okay, we expected that
      end
    end
  end
end

#
# "check_ ## type" routines to be called by our test loops.
#

def check_numeric(prot, tag)
  assert_equal( tag, prot.descrips[1].to_sym,
                "tag #{tag} didn't match (protein #{prot.index})" )

  is_integer = (tag.to_s =~ /float/).nil?
  basic_num = is_integer  ?  23  :  23.4

  if is_integer
    assert_equal( basic_num, prot.ingests["slaw"],
                  "value didn't match for #{tag} (protein #{prot.index})" )
  else
    assert_in_delta( basic_num, prot.ingests["slaw"], 0.0001,
                     "value didn't match for #{tag} (protein #{prot.index})" )
  end


  p_slaw = prot.ingests.nth_slaw(0).cdr
  assert_not_nil p_slaw, "couldn't extract slaw ingest (protein #{prot.index})"

  assert_equal( tag, p_slaw.type_tag,
                "type tag mismatch (protein #{prot.index})" )

  newed_slaw = Slaw.new( basic_num, tag.to_sym )
  shortcutted_slaw = eval "#{tag}( basic_num )"

  assert_equal( p_slaw.type_tag, newed_slaw.type_tag,
                "type tag mismatch for newed slaw ('#{tag}')" )

  assert_equal( newed_slaw, p_slaw,
                "newed equality test failed (protein #{prot.index})" )
  assert_equal( shortcutted_slaw, p_slaw,
                "newed equality test failed (protein #{prot.index})" )
  if is_integer
    assert_equal( newed_slaw.emit, basic_num, "emit value test failed" )
  else
    assert_in_delta( newed_slaw.emit, basic_num, 0.0001,
                     "emit value test failed" )
  end

  check_respond_to p_slaw,
  { :each => false, :keys => false, :values => false,
    :car => false, :cdr => false,
    :[] => false, :nth_slaw => false,
    :to_a => false, :to_ary => false, :to_hash => false,
    :to_str => false,
    :to_i => true, :to_int => true, :to_f => true }

end


def check_vect(prot, tag, num_components)
  assert_equal( tag, prot.descrips[1].to_sym,
                "tag #{tag} didn't match (protein #{prot.index})" )

  is_integer = (tag.to_s =~ /float/).nil?
  basic_num = is_integer  ?  23  :  23.4

  (0..num_components-1).each do |n|
    if is_integer
      assert_equal( basic_num+n, prot.ingests["slaw"][n],
                "value didn't match for #{tag}/#{n} (protein #{prot.index})" )
    else
      assert_in_delta( basic_num+n, prot.ingests["slaw"][n], 0.0001,
                "value didn't match for #{tag}/#{n} (protein #{prot.index})" )
    end
  end

  p_slaw = prot.ingests.nth_slaw(0).cdr
  assert_not_nil p_slaw, "couldn't extract slaw ingest (protein #{prot.index})"

  assert_equal( tag, p_slaw.type_tag,
                "type tag mismatch (protein #{prot.index})" )

  vect = (0..num_components-1).map { |n| basic_num + n }

  newed_slaw = Slaw.new( vect, tag.to_sym )
  shortcutted_slaw = eval "#{tag}( *vect )"

  assert_equal( p_slaw.type_tag, newed_slaw.type_tag,
                "type tag mismatch for newed slaw ('#{tag}')" )

  assert_equal( newed_slaw, p_slaw,
                "newed equality test failed (protein #{prot.index})" )
  assert_equal( shortcutted_slaw, p_slaw,
                "newed equality test failed (protein #{prot.index})" )
  assert_equal( newed_slaw.emit, prot.ingests["slaw"],
                "slaw 'value' test failed")

  check_respond_to p_slaw,
  { :each => true, :keys => false, :values => false,
    :car => false, :cdr => false,
    :[] => true, :nth_slaw => false,
    :to_a => true, :to_ary => false, :to_hash => false,
    :to_str => false,
    :to_i => false, :to_int => false, :to_f => false }

end


def check_array(prot, tag)
  assert_equal( tag, prot.descrips[1].to_sym,
                "tag #{tag} didn't match (protein #{prot.index})" )

  is_integer = (tag.to_s =~ /float/).nil?
  basic_num = is_integer  ?  23  :  23.4

  (0..$test_array_size-1).each do |n|
    if is_integer
      assert_equal( basic_num+n, prot.ingests["slaw"][n],
                  "value didn't match for #{tag}/#{n} (protein #{prot.index})" )
    else
      assert_in_delta( basic_num+n, prot.ingests["slaw"][n], 0.0001,
                  "value didn't match for #{tag}/#{n} (protein #{prot.index})" )
    end
  end

  p_slaw = prot.ingests.nth_slaw(0).cdr
  assert_not_nil p_slaw, "couldn't extract slaw ingest (protein #{prot.index})"

  assert_equal( tag, p_slaw.type_tag,
                "type tag mismatch (protein #{prot.index})" )

  arr = (0..$test_array_size-1).map { |n| basic_num + n }

  newed_slaw = Slaw.new( arr, tag.to_sym )
  shortcutted_slaw = eval "#{tag}( arr )"

  assert_equal( p_slaw.type_tag, newed_slaw.type_tag,
                "type tag mismatch for newed slaw ('#{tag}')" )

  assert_equal( newed_slaw, p_slaw,
                "newed equality test failed (protein #{prot.index})" )
  assert_equal( shortcutted_slaw, p_slaw,
                "newed equality test failed (protein #{prot.index})" )
  assert_equal( newed_slaw.emit, prot.ingests["slaw"],
                "slaw 'value' test failed")

  check_respond_to p_slaw,
  { :each => true, :keys => false, :values => false,
    :car => false, :cdr => false,
    :[] => true, :nth_slaw => false,
    :to_a => true, :to_ary => true, :to_hash => false,
    :to_str => false,
    :to_i => false, :to_int => false, :to_f => false }
end


def check_vect_array(prot, tag, num_components)
  assert_equal( array_tag(tag), prot.descrips[1].to_sym,
                "tag #{tag} didn't match (protein #{prot.index})" )

  is_integer = (tag.to_s =~ /float/).nil?
  basic_num = is_integer  ?  23  :  23.4

  n = 0
  (0..$test_array_size-1).each do |an|
    (0..num_components-1).each do |vn|
      if is_integer
        assert_equal( basic_num+n, prot.ingests["slaw"][an][vn],
          "value didn't match for #{tag}/#{an}/#{vn} (protein #{prot.index})" )
      else
        assert_in_delta( basic_num+n, prot.ingests["slaw"][an][vn], 0.0001,
          "value didn't match for #{tag}/#{an}/#{vn} (protein #{prot.index})" )
      end
      n = n + 1
    end
  end

  p_slaw = prot.ingests.nth_slaw(0).cdr
  assert_equal( array_tag(tag), p_slaw.type_tag,
                "type tag mismatch (protein #{prot.index})" )

  vects_arr = Array.new
  n = 0
  (0..$test_array_size-1).each do |an|
    this_vect = Array.new
    (0..num_components-1).each do |vn|
      this_vect.push basic_num+n
      n = n + 1
    end
    vects_arr.push this_vect
  end

  newed_slaw = Slaw.new( vects_arr, array_tag(tag).to_sym )
  # shortcutted_slaw = eval "#{array_tag(tag)}( vects_arr )"

  assert_equal( newed_slaw, p_slaw,
                "newed equality test failed (protein #{prot.index})" )

  check_respond_to p_slaw,
  { :each => true, :keys => false, :values => false,
    :car => false, :cdr => false,
    :[] => true, :nth_slaw => false,
    :to_a => true, :to_ary => true, :to_hash => false,
    :to_str => false,
    :to_i => false, :to_int => false, :to_f => false }
end


#
# test loops for numeric types
#

class PoolTypesTest < Test::Unit::TestCase
  def test_all_basic_types
    h = Pool.participate( PNAME );
    h.rewind

    assert_equal(0, h.oldest_index, "oldest index")
    assert_equal(82, h.newest_index, "newest index")

    # how about we start with a bignum ?
    s = int64(8371855301)
    assert_equal( 8371855301, s.emit)

    # basics
    #
    $basic_numeric_tags.each do |tag|
      prot = h.next
      assert_not_nil prot
      check_numeric prot, tag
    end

    # vects
    #
    (2..4).each do |n|
      $basic_numeric_tags.each do |tag|
        prot = h.next
        assert_not_nil prot
        check_vect prot, vect_tag(tag,n), n
      end
    end

    # numeric basic arrays
    #
    $basic_numeric_tags.each do |tag|
      prot = h.next
      assert_not_nil prot
      check_array prot, array_tag( tag )
    end

    # vect arrays
    #
    (2..4).each do |n|
      $basic_numeric_tags.each do |tag|
        prot = h.next
        assert_not_nil prot
        check_vect_array prot, vect_tag(tag,n), n
      end
    end

    # nil
    #
    prot = h.next
    assert_not_nil prot
    p_slaw = prot.ingests.nth_slaw(0).cdr
    assert_nil p_slaw.emit
    assert_equal( nil, p_slaw.type_tag,
                  "type tag mismatch for nil slaw (#{prot.index})" );
    newed_slaw = Slaw.new(nil)
    assert_equal( newed_slaw, p_slaw,
                  "newed equality test failed for slaw-nil")
    assert_equal( newed_slaw.emit, p_slaw.emit, "slaw 'value' test failed")
    check_respond_to p_slaw,
    { :each => false, :keys => false, :values => false,
      :car => false, :cdr => false,
      :[] => false, :nth_slaw => false,
      :to_a => false, :to_ary => false, :to_hash => false,
      :to_str => false,
      :to_i => false, :to_int => false, :to_f => false }


    # true slaw
    prot = h.next
    assert_not_nil prot
    p_slaw = prot.ingests.nth_slaw(0).cdr
    assert p_slaw.emit
    assert_equal( prot.descrips[1].to_sym, p_slaw.type_tag,
                  "type tag mismatch for true slaw" );
    newed_slaw = Slaw.new(true, :boolean)
    assert_equal( newed_slaw, p_slaw,
                  "newed equality test failed for slaw-true")
    assert_equal( newed_slaw.emit, p_slaw.emit, "slaw 'value' test failed")
    check_respond_to p_slaw,
    { :each => false, :keys => false, :values => false,
      :car => false, :cdr => false,
      :[] => false, :nth_slaw => false,
      :to_a => false, :to_ary => false, :to_hash => false,
      :to_str => false,
      :to_i => false, :to_int => false, :to_f => false }

    # false slaw
    prot = h.next
    assert_not_nil prot
    p_slaw = prot.ingests.nth_slaw(0).cdr
    assert !p_slaw.emit
    assert_equal( prot.descrips[1].to_sym, p_slaw.type_tag,
                  "type tag mismatch for false slaw" );
    newed_slaw = Slaw.new(false, :boolean)
    assert_equal( newed_slaw, p_slaw,
                  "newed equality test failed for slaw-false")
    assert_equal( newed_slaw.emit, p_slaw.emit, "slaw 'value' test failed")
    check_respond_to p_slaw,
    { :each => false, :keys => false, :values => false,
      :car => false, :cdr => false,
      :[] => false, :nth_slaw => false,
      :to_a => false, :to_ary => false, :to_hash => false,
      :to_str => false,
      :to_i => false, :to_int => false, :to_f => false }

  end
end

