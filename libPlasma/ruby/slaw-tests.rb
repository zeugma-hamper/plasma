# -*- coding: utf-8 -*-

require_relative './test-helpers.rb'
require 'base64'

require 'Pool'
include Plasma

MATTIE_EXAMPLE = <<'END'
%YAML 1.1
%TAG ! tag:oblong.com,2009:slaw/
--- !protein
descrips:
- layout-manager
- change
- tapir
- 0b6fb6ef-5bb9-445e-8dfb-d7bf9b46e2f1
ingests: !!omap
- anatomy: !!omap
  - am-grabbed: osx-mouse
  - protagonist: !!omap []
END

PATRICK_EXAMPLE = <<'END'
%YAML 1.1
%TAG ! tag:oblong.com,2009:slaw/
--- !protein
descrips:
- layout-manager
- change
- tapir
- 0b6fb6ef-5bb9-445e-8dfb-d7bf9b46e2f1
ingests: !!omap
- anatomy: !!omap
  - am-grabbed: osx-mouse
  - protagonist: []
END

PATRICK_INDIGESTION_EXAMPLE = <<'END'
%YAML 1.1
%TAG ! tag:oblong.com,2009:slaw/
--- !protein
descrips: !protein
  descrips:
  - !protein
    descrips:
    - I ♥ toxic waste
    ingests: !!omap
    - large alarmed secret: oblolate the syntax information
  ingests: !!omap
  - ? !protein
      descrips:
      - I ♥ toxic waste
      ingests: !!omap
      - large alarmed secret: oblolate the syntax information
    : !protein
      descrips:
      - I ♥ toxic waste
      ingests: !!omap
      - large alarmed secret: oblolate the syntax information
ingests: !protein
  descrips:
  - !cons
    ? !protein
      descrips:
      - I ♥ toxic waste
      ingests: !!omap
      - large alarmed secret: oblolate the syntax information
    : !protein
      descrips:
      - I ♥ toxic waste
      ingests: !!omap
      - large alarmed secret: oblolate the syntax information
...
END

MATTIE_INDIGESTION_EXAMPLE = <<'END'
%YAML 1.1
%TAG ! tag:oblong.com,2009:slaw/
--- !protein

descrips:
- bgt-config

ingests:
  global-configurations:
    sml-font-size: 10.0
    med-font-size: 15.0
    lrg-font-size: 20.0
    rel-starting-loc: !vector [0.5, -0.5]
    filter-icon-path: /kipple/global_tech/check_mark.png
    dormant-adj-color: !vector [0.5, 0.5, 0.5, 0.2]
  data-sources:
  - !protein
    descrips:
    - data-source
    - file
    ingests:
      group-name: "countries-red"
      file-path: "/kipple/global_tech/data/live_data/Country-Nodes-Rev02-RED.upipe"
      delim: "\\|"
      escape-char: "##"
      color: !vector [ 0.9, 0.2, 0.1, 0.75 ]
      icon:  "/kipple/circle.tiff"
      scale: 3.0
      display-cols: [ 1, 0, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17 ]
      special-cols:
      - !cons
        4:
          send-key-sequence:
            app-name: ppt
            sequence: DYNAMIC_VALUE\n
      - !cons
        15:
          foreground-window:
            file-name: DNA.pdf
      - !cons
        16:
          open-path:
            path: DYNAMIC_VALUE
      - !cons
        17:
          open-path:
            path: DYNAMIC_VALUE
      derived-filters:
        Country Code: [ suppliers, research-institutes ]
  - !protein
    descrips:
    - data-source
    - file
    ingests:
      group-name: "countries-yellow"
      file-path: "/kipple/global_tech/data/live_data/Country-Nodes-Rev02-YELLOW.upipe"
      delim: "\\|"
      escape-char: "##"
      color: !vector [ 0.8, 0.7, 0.0, 0.75 ]
      icon:  "/kipple/circle.tiff"
      scale: 3.0
      display-cols: [ 1, 0, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ]
      special-cols:
      - !cons
        4:
          send-key-sequence:
            app-name: ppt
            sequence: DYNAMIC_VALUE\n
      - !cons
        15:
          foreground-window:
            file-name: DNA.pdf
      derived-filters:
        Country Code: [ suppliers, research-institutes ]
  - !protein
    descrips:
    - data-source
    - file
    ingests:
      group-name: "countries-green"
      file-path: "/kipple/global_tech/data/live_data/Country-Nodes-Rev02-GREEN.upipe"
      delim: "\\|"
      escape-char: "##"
      color: !vector [ 0.1, 0.9, 0.3, 0.75 ]
      icon:  "/kipple/circle.tiff"
      scale: 3.0
      display-cols: [ 1, 0, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 ]
      special-cols:
      - !cons
        4:
          send-key-sequence:
            app-name: ppt
            sequence: DYNAMIC_VALUE\n
      - !cons
        15:
          foreground-window:
            file-name: DNA.pdf
      derived-filters:
        Country Code: [ suppliers, research-institutes ]
  - !protein
    descrips:
    - data-source
    - file
    ingests:
      group-name: "suppliers"
      file-path: "/kipple/global_tech/data/live_data/Supplier-Nodes-Rev01.upipe"
      delim: "\\|"
      escape-char: "##"
      color: !vector [ 0.2, 0.7, 0.9, 0.75 ]
      scale: 1.0
      display-cols: [ 0, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 ]
      special-cols:
      - !cons
        12:
          open-path:
            path: DYNAMIC_VALUE
  - !protein
    descrips:
    - data-source
    - file
    ingests:
      group-name: "research-institutes"
      file-path: "/kipple/global_tech/data/live_data/Research_Entities01.upipe"
      delim: "\\|"
      escape-char: "##"
      color: !vector [ 0.7, 0.2, 0.9, 0.75 ]
      scale: 1.0
      display-cols: [ 0, 1, 2, 3, 4, 5 ]
  filters:
  - description: Environment
    key: domain_ENV
    val: '^[1-9]+[0-9]*$'
  - description: Manufacturing
    key: domain_MFG
    val: '^[1-9]+[0-9]*$'
  - description: Networked Systems
    key: domain_NET
    val: '^[1-9]+[0-9]*$'
  - description: Platform Performance
    key: domain_PRF
    val: '^[1-9]+[0-9]*$'
  - description: Platform Systems & Subsystems
    key: domain_PSS
    val: '^[1-9]+[0-9]*$'
  - description: Structures
    key: domain_STR
    val: '^[1-9]+[0-9]*$'
  - description: Support & Services
    key: domain_SUP
    val: '^[1-9]+[0-9]*$'
  - description: Systems Engineering & Analysis
    key: domain_SEA
    val: '^[1-9]+[0-9]*$'
END

RUDE_EXAMPLE = <<'END'
%YAML 1.1
%TAG ! tag:oblong.com,2009:slaw/
--- !protein
rude_data: !!binary wAHQDQ==
END

class SlawTest < Test::Unit::TestCase

  def test_nested_slaw_in_list
    s = Slaw.new( [1,
                   float64(2),
                   3,
                   int8(4)] )
    assert_equal( 4, s.count )
    assert_equal( [1,2,3,4], s.emit )
  end

  def test_nested_map_in_list
    s = Slaw.new( [1, {"two"=>float64(2.0), "two-point-five"=>2.5}, 3] )
    assert_equal( 3, s.count )
    assert_equal( 1, s[0] )
    assert_equal( {"two"=>2.0, "two-point-five"=>2.5}, s[1] )
    assert_equal( 3, s[2] )
    assert_equal( [1, {"two"=>2.0, "two-point-five"=>2.5}, 3], s.emit )
  end

  def test_further_nesting
    s = Slaw.new( [1, [2, 3], {4=>5}, {6=>{7=>[8,9]}}] )
    assert_equal( 4, s.count )
    assert_equal( 1, s[0] )
    assert_equal( [2,3], s[1] )
    assert_equal( {4=>5}, s[2] )
    assert_equal( {6=>{7=>[8,9]}}, s[3] )
    assert_equal( [8,9], s[3][6][7] )
    assert_equal( [1, [2, 3], {4=>5}, {6=>{7=>[8,9]}}], s.emit )
  end

  def test_vect
    s = v3float64( 1.0, 2.0, 3.0 )
    assert_equal( 1.0, s[0] )
    assert_equal( 2.0, s[1] )
    assert_equal( 3.0, s[2] )
  end

  def test_list_concat
    s1 = Slaw.new( [1, float64(2)] )
    s2 = Slaw.new( [3, int8(4), "5"] )
    s = s1 + s2
    assert_equal( 5, s.count )
    assert_equal( [1,2,3,4,"5"], s.emit )
    q = Slaw.new( [1,
                   float64(2),
                   3,
                   int8(4),
                   "5"] )
    assert_equal( s, q )
  end

  def test_list_concat2
    s1 = Slaw.new( [1, float64(2)] )
    s2 = Slaw.new( [3, int8(4), "5"] )
    s = Slaw.concatenate_lists(s1, s2)
    assert_equal( 5, s.count )
    assert_equal( [1,2,3,4,"5"], s.emit )
    q = Slaw.new( [1,
                   float64(2),
                   3,
                   int8(4),
                   "5"] )
    assert_equal( s, q )
  end

  def test_list_concat3
    s1 = Slaw.new( [1, 2] )
    s2 = Slaw.new( [3] )
    s3 = Slaw.new( [] )
    s4 = Slaw.new( [4, 5] )
    s = Slaw.concatenate_lists(s1, s2, s3, s4)
    assert_equal( 5, s.count )
    assert_equal( [1,2,3,4,5], s.emit )
  end

  def test_ordered_map
    s = Slaw.new( ["one"   , 1,
                   "two"   , 2,
                   "three" , 3,
                   "four"  , 4], :map )
    assert_equal( :map, s.type_tag )
    assert_equal( 1, s["one"] )
    assert_equal( 2, s["two"] )
    assert_equal( 3, s["three"] )
    assert_equal( 4, s["four"] )
    s0 = s.nth_slaw(0)
    assert_equal( :cons, s0.type_tag )
    assert_equal( "one", s0.car.emit )
    assert_equal( 1, s0.cdr.emit )
    s1 = s.nth_slaw(1)
    assert_equal( :cons, s1.type_tag )
    assert_equal( "two", s1.car.emit )
    assert_equal( 2, s1.cdr.emit )
    s3 = s.nth_slaw(-1)
    assert_equal( :cons, s3.type_tag )
    assert_equal( "four", s3.car.emit )
    assert_equal( 4, s3.cdr.emit )

    t = Slaw.new(s) # test again but values pulled only from cslaw
    assert_equal( 1, t["one"] )
    assert_equal( 2, t["two"] )
    assert_equal( 3, t["three"] )
    assert_equal( 4, t["four"] )
    s0 = t.nth_slaw(0)
    assert_equal( :cons, s0.type_tag )
    assert_equal( "one", s0.car.emit )
    assert_equal( 1, s0.cdr.emit )
    s1 = t.nth_slaw(1)
    assert_equal( :cons, s1.type_tag )
    assert_equal( "two", s1.car.emit )
    assert_equal( 2, s1.cdr.emit )
    s3 = t.nth_slaw(-1)
    assert_equal( :cons, s3.type_tag )
    assert_equal( "four", s3.car.emit )
    assert_equal( 4, s3.cdr.emit )
  end

  def test_nested_protein
    s = Slaw.new( [1,
                   Protein.new(["first", "protein"], {"ingest"=>23.0}),
                   Protein.new(["second", "protein"], ["ingest",24.0])
                  ])
    assert_equal( :list, s.type_tag )
    assert_equal( 1, s[0] )
    assert_equal( :protein, s[1].type_tag )
    assert_equal( :protein, s[2].type_tag )
    assert_equal( ["first", "protein"], s[1].descrips.emit )
    assert_equal( ["second", "protein"], s[2].descrips.emit )
    assert_equal( "protein", s[1].descrips.nth_slaw(1).emit )
    assert_equal( 23.0, s[1].ingests["ingest"] )
    assert_equal( 24.0, s[2].ingests["ingest"] )
    assert_equal( :cons, s[2].ingests.nth_slaw(0).type_tag )
    assert_equal( "ingest", s[2].ingests.nth_slaw(0).car.emit )
    assert_equal( 24.0, s[2].ingests.nth_slaw(0).cdr.emit )
  end

  def test_list
    s = Slaw.new( [1,2,3,4] )
    assert_equal( :list, s.type_tag )
    check_respond_to s,
    { :each => true, :keys => false, :values => false,
      :car => false, :cdr => false,
      :[] => true, :nth_slaw => true, :each_slaw => true, :find_slaw => false,
      :to_a => true, :to_ary => true, :to_hash => false,
      :to_str => false,
      :+ => true,
      :emit_binary => false,
      :to_i => false, :to_int => false, :to_f => false }
  end

  def test_map
    s = Slaw.new( {1 => 2, 3 => 4} )
    assert_equal( :map, s.type_tag )
    check_respond_to s,
    { :each => true, :keys => true, :values => true,
      :car => false, :cdr => false,
      :[] => true, :nth_slaw => true, :each_slaw => true, :find_slaw => true,
      :to_a => true, :to_ary => false, :to_hash => true,
      :to_str => false,
      :+ => false,
      :emit_binary => false,
      :to_i => false, :to_int => false, :to_f => false }
  end

  def test_cons
    s = Slaw.new( [1,2], :cons)
    assert_equal( :cons, s.type_tag )
    # removed ":[] => false" because even though cons says it does not
    # respond to [], it just returns nil rather than throwing an exception
    # as we expect.
    check_respond_to s,
    { :each => false, :keys => false, :values => false,
      :car => true, :cdr => true,
      :nth_slaw => false, :each_slaw => false, :find_slaw => false,
      :to_a => false, :to_ary => false, :to_hash => false,
      :to_str => false,
      :+ => false,
      :emit_binary => false,
      :to_i => false, :to_int => false, :to_f => false }
  end

  def test_descrips
    p = MoveRequest(923, 111, "behind my desk", "Rhode Island")
    assert_equal("layout-manager", p.descrips[0])
  end

  def test_spaceship_operator
    s1 = shuffled_slaw_list
    s2 = shuffled_slaw_list
    s1.sort!
    s2.sort!
    assert_equal( s1, s2 )
  end

  def test_yaml_that_was_segfaulting
    s = Slaw.from_yaml(MATTIE_EXAMPLE)
    badness = s.ingests.to_hash
    assert_equal( 0, badness["anatomy"]["protagonist"].length )
  end

  def test_empty_list_too
    s = Slaw.from_yaml(PATRICK_EXAMPLE)
    badness = s.ingests.to_hash
    assert_equal( 0, badness["anatomy"]["protagonist"].length )
  end

  def test_illegal_yaml
    assert_raises(SlawIOError) { Slaw.from_yaml('"This is illegal') }
  end

  def test_indigestion
    s = Slaw.from_yaml(PATRICK_INDIGESTION_EXAMPLE)
    a = Array.new
    a.push(s.descrips.descrips.nth_slaw(0))
    a.push(s.descrips.ingests.nth_slaw(0).car)
    a.push(s.descrips.ingests.nth_slaw(0).cdr)
    a.push(s.ingests.descrips.nth_slaw(0).car)
    a.push(s.ingests.descrips.nth_slaw(0).cdr)
    a.each do |x|
      obsert_equal("I ♥ toxic waste", x.descrips.nth_slaw(0).emit)
      obsert_equal("large alarmed secret", x.ingests.nth_slaw(0).car.emit)
      obsert_equal("oblolate the syntax information",
                   x.ingests.nth_slaw(0).cdr.emit)
    end
  end

  def test_indigestion2
    s = Slaw.from_yaml(MATTIE_INDIGESTION_EXAMPLE)
    data_sources = s.ingests.nth_slaw(1).cdr
    a = Array.new
    data_sources.count.times do |n|
      p = data_sources.nth_slaw(n)
      gest = p.ingests
      file_path = gest.nth_slaw(1).cdr.emit
      a.push(File.basename(file_path, ".upipe"))
    end
    assert_equal(5, a.length)
    assert_equal("Country-Nodes-Rev02-RED", a[0])
    assert_equal("Country-Nodes-Rev02-YELLOW", a[1])
    assert_equal("Country-Nodes-Rev02-GREEN", a[2])
    assert_equal("Supplier-Nodes-Rev01", a[3])
    assert_equal("Research_Entities01", a[4])
  end

  def test_encoding
    str = PATRICK_INDIGESTION_EXAMPLE
    if str.respond_to?(:encode)
      str = str.encode("UTF-16BE")
    end
    s = Slaw.from_yaml(str)
    a = Array.new
    a.push(s.descrips.descrips.nth_slaw(0))
    a.push(s.descrips.ingests.nth_slaw(0).car)
    a.push(s.descrips.ingests.nth_slaw(0).cdr)
    a.push(s.ingests.descrips.nth_slaw(0).car)
    a.push(s.ingests.descrips.nth_slaw(0).cdr)
    a.each do |x|
      obsert_equal("I ♥ toxic waste", x.descrips.nth_slaw(0).emit)
      obsert_equal("large alarmed secret", x.ingests.nth_slaw(0).car.emit)
      obsert_equal("oblolate the syntax information",
                   x.ingests.nth_slaw(0).cdr.emit)
    end
  end

  def test_encoding2
    str = "I ♥ toxic waste"
    if str.respond_to?(:encode)
      str = str.encode("UTF-16BE")
    end
    s = Slaw.new(str)
    obsert_equal("I ♥ toxic waste", s.emit)
  end

  def test_find_slaw
    s = Slaw.from_yaml(MATTIE_INDIGESTION_EXAMPLE)
    data_sources = s.ingests.find_slaw("data-sources")
    a = Array.new
    data_sources.each_slaw do |p|
      gest = p.ingests
      file_path = gest.find_slaw("file-path")
      a.push(File.basename(file_path, ".upipe"))
    end
    assert_equal(5, a.length)
    assert_equal("Country-Nodes-Rev02-RED", a[0])
    assert_equal("Country-Nodes-Rev02-YELLOW", a[1])
    assert_equal("Country-Nodes-Rev02-GREEN", a[2])
    assert_equal("Supplier-Nodes-Rev01", a[3])
    assert_equal("Research_Entities01", a[4])
  end

  def test_merge
    # merging no maps should result in an empty map
    assert_equal(Slaw.new(Hash.new), Slaw.merge_maps())

    s = Slaw.from_yaml(MATTIE_INDIGESTION_EXAMPLE)
    data_sources = s.ingests.find_slaw("data-sources")
    m1 = data_sources.nth_slaw(0).ingests
    m2 = s.ingests.find_slaw("global-configurations")
    m3 = Slaw.new(["What are you doing here?", "I carried a watermelon"], :map)
    m = Slaw.merge_maps(m1, m2, m3)
    assert_equal(10.0, m["sml-font-size"])
    assert_equal("check_mark.png", File.basename(m["filter-icon-path"]))
    assert_equal("countries-red", m["group-name"])
    assert_equal("I carried a watermelon", m["What are you doing here?"])
  end

  def test_merge2
    s = Slaw.from_yaml(MATTIE_INDIGESTION_EXAMPLE)
    m1 = s.ingests.find_slaw("global-configurations")
    m2 = Slaw.new(["What are you doing here?", "I carried a watermelon"], :map)
    m = m1.merge(m2)
    assert_equal(10.0, m["sml-font-size"])
    assert_equal("check_mark.png", File.basename(m["filter-icon-path"]))
    assert_equal("I carried a watermelon", m["What are you doing here?"])
  end

  def test_put
    m1 = Slaw.new({"foo" => "bar", "bob" => "baz"})
    m2 = m1.map_put("crud", "stuff")
    m3 = m2.map_put("foo", "oof")
    assert_equal("bar", m2["foo"])
    assert_equal("baz", m2["bob"])
    assert_equal("stuff", m2["crud"])
    assert_equal("oof", m3["foo"])
    assert_equal("baz", m3["bob"])
    assert_equal("stuff", m3["crud"])
  end

  def test_map_remove
    m1 = Slaw.new({"foo" => "bar", "baz" => "bob", "crud" => "stuff"})
    m2 = m1.map_remove("baz")
    assert_equal("bar", m2["foo"])
    assert_equal(nil, m2["baz"])
    assert_equal("stuff", m2["crud"])
  end

  def test_append
    s1 = Slaw.new([1, 2, 3, 4])
    s2 = s1.append(5)
    assert_equal( 5, s2.count )
    assert_equal( [1,2,3,4,5], s2.emit )
  end

  def test_prepend
    s1 = Slaw.new([1, 2, 3, 4])
    s2 = s1.prepend(5)
    assert_equal( 5, s2.count )
    assert_equal( [5,1,2,3,4], s2.emit )
  end

  def test_insert_list
    s1 = Slaw.new([1, 5])
    s2 = Slaw.new([2, 3, 4])
    s3 = s1.insert_list(1, s2)
    assert_equal( 5, s3.count )
    assert_equal( [1,2,3,4,5], s3.emit )
  end

  def test_insert
    s1 = Slaw.new([1, 2, 4, 5])
    s2 = s1.insert(2, 3)
    s3 = s2.insert(5, 6, 7)
    assert_equal( 5, s2.count )
    assert_equal( [1,2,3,4,5], s2.emit )
    assert_equal( [1,2,3,4,5,6,7], s3.emit )
  end

  def test_list_remove
    s1 = Slaw.new([1, 2, 3, 4, 5])
    s2 = s1.list_remove(2)
    s3 = s1.list_remove(0)
    s4 = s1.list_remove(4)
    s5 = s1.list_remove(1..3)
    assert_equal( [1,2,4,5], s2.emit )
    assert_equal( [2,3,4,5], s3.emit )
    assert_equal( [1,2,3,4], s4.emit )
    assert_equal( [1,5], s5.emit )
  end

  def test_rude
    rude = [0xbadf00d0f1ea5eed].pack("Q")
    p1 = Protein.new(["whatever"], {"foo" => "bar"}, rude)
    p2 = Slaw.from_yaml(p1.to_s)
    [p1,p2].each do |p|
      d = p.descrips[0]
      i = p.ingests["foo"]
      r = p.rude_data
      number = r.unpack("Q")[0]
      obsert_equal("whatever", d)
      obsert_equal("bar", i)
      assert_equal(0xbadf00d0f1ea5eed, number)
      obsert_equal(rude, r)
    end
  end

  def test_create_binary
    data = Base64.decode64("wAHQDQ==")
    a1 = Slaw.new(data, :unt8_array)
    a2 = Slaw.from_yaml(a1.to_s)
    a3 = Slaw.new(data, :int8_array)
    a4 = Slaw.from_yaml(a3.to_s)
    [a1,a2].each do |a|
      assert_equal([0xc0, 0x01, 0xd0, 0x0d], a.emit)
    end
    [a3,a4].each do |a|
      assert_equal([-64, 1, -48, 13], a.emit)
    end
  end

  def test_emit_binary
    a1 = Slaw.new([0xc0, 0x01, 0xd0, 0x0d], :unt8_array)
    a2 = Slaw.from_yaml(a1.to_s)
    a3 = Slaw.new([-64, 1, -48, 13], :int8_array)
    a4 = Slaw.from_yaml(a3.to_s)
    p = Slaw.from_yaml(RUDE_EXAMPLE)
    [a1,a2,a3,a4,p].each do |a|
      s = a.emit_binary
      number = s.unpack("N")[0]
      assert_equal(0xc001d00d, number)
    end
  end

  def test_container
    memories = Array.new
    4.times do
      250.times do
        forgettable = Slaw.from_yaml(MATTIE_EXAMPLE)
        memories.push(Slaw.new(forgettable.descrips.nth_slaw(2)))
      end
      GC.start
    end
    emissions = memories.map {|s| s.emit}
    assert_equal(["tapir"], emissions.uniq)
  end

  def test_bug1578
    wands = [{:index => "tapir", :channel => 4}]
    params = {:wand_tapir_channel => 5}
    wand_channel_provisions = []
    wands.each do |wand|
      new_channel = params[:"wand_#{wand[:index]}_channel"].to_i
      wand_channel_provisions.push Slaw.new(["channel", wand[:channel] == new_channel ? nil : new_channel], :map)
    end
    obi_wand_kenobi = Slaw.new(wand_channel_provisions)
    yam = obi_wand_kenobi.to_s
    assert_equal(<<-'END', yam)
%YAML 1.1
%TAG ! tag:oblong.com,2009:slaw/
---
- !!omap
  - channel: !i64 5
...
    END
  end

  def test_bad_tag_not_a_symbol
    assert_raises(SlawTypeTagError) { Slaw.new("foo", STDERR) }
  end

  def test_bad_tag_scalar
    assert_raises(SlawTypeTagError) { Slaw.new("split", :bananagram) }
  end

  def test_bad_tag_list
    assert_raises(SlawTypeTagError) { Slaw.new(["split", "peel"],
                                               :bananagram) }
  end

  def test_bad_tag_list2
    # assert_raises(SlawTypeTagError) { s = Slaw.new([], :bananagram) }
  end

  def test_bad_tag_map
    # assert_raises(SlawTypeTagError) { s = Slaw.new({"split" => "peel"},
    #                                                :bananagram) }
  end

  def test_bad_tag_arrayish_looking
    assert_raises(SlawTypeTagError) { Slaw.new(["RFC", 4042],
                                               :unt18_array) }
  end

  def test_bad_tag_arrayish_looking2
    assert_raises(SlawTypeTagError) { Slaw.new([1, 2, 3], :array) }
  end

  def test_list_of_tags
    s = Slaw.new( [12, 13.0, 14, "oranges"], [:int32,:int32,:int32] )
    assert_equal(s[1], 13)
    assert_equal(s[3], "oranges")
  end

  def test_non_map_ingests
    # ingests is a list
    p1 = Protein.new(["descrips"], Slaw.new(["ingests"]))
    # ingests is a string
    p2 = Protein.new(["descrips"], "ingests")
    # ingests is an array
    s = Slaw.new([1, 2, 3], :unt8_array)
    p3 = Protein.new(["descrips"], s)
    # ingests is a protein
    p4 = Protein.new(["descrips"], Slaw.from_yaml(RUDE_EXAMPLE))
    yam = Slaw.new([p1, p2, p3, p4]).to_s
    assert_equal(<<-'END', yam)
%YAML 1.1
%TAG ! tag:oblong.com,2009:slaw/
---
- !protein
  descrips:
  - descrips
  ingests:
  - ingests
- !protein
  descrips:
  - descrips
  ingests: ingests
- !protein
  descrips:
  - descrips
  ingests: !array [!u8 1, !u8 2, !u8 3]
- !protein
  descrips:
  - descrips
  ingests: !protein
    rude_data: !!binary |-
      wAHQDQ==
...
    END
  end

  def test_non_list_descrips
    # descrips is a map
    p1 = Protein.new({"union of the" => "snake"}, {"hungry like the" => "wolf"})
    # descrips is a string
    p2 = Protein.new("descrips", {"hungry like the" => "wolf"})
    # descrips is an array
    s = Slaw.new([1, 2, 3], :unt8_array)
    p3 = Protein.new(s, {"hungry like the" => "wolf"})
    # descrips is a protein
    p4 = Protein.new(Slaw.from_yaml(RUDE_EXAMPLE), {"hungry like the" => "wolf"})
    yam = Slaw.new([p1, p2, p3, p4]).to_s
    assert_equal(<<-'END', yam)
%YAML 1.1
%TAG ! tag:oblong.com,2009:slaw/
---
- !protein
  descrips: !!omap
  - union of the: snake
  ingests: !!omap
  - hungry like the: wolf
- !protein
  descrips: descrips
  ingests: !!omap
  - hungry like the: wolf
- !protein
  descrips: !array [!u8 1, !u8 2, !u8 3]
  ingests: !!omap
  - hungry like the: wolf
- !protein
  descrips: !protein
    rude_data: !!binary |-
      wAHQDQ==
  ingests: !!omap
  - hungry like the: wolf
...
    END
  end

  def test_bogus_thing_as_slaw
    assert_raises(ArgumentError) { Slaw.new(STDERR) }
  end

  def test_bogus_thing_as_slaw_string
    s = Slaw.new(STDERR, :string)
    assert( s.emit =~ /#<IO:0x[0-9a-f]+>/ )
  end

  def test_bogus_thing_as_slaw_float
    assert_raises(TypeError) { Slaw.new(STDERR, :float64) }
  end

  def test_bogus_thing_as_slaw_int
    assert_raises(TypeError) { Slaw.new(STDERR, :int64) }
  end

  def test_bogus_thing_as_slaw_v3float64
    assert_raises(SlawTypeTagError) { Slaw.new(STDERR, :v3float64) }
  end

  def test_bogus_map_key
    assert_raises(ArgumentError) do
      Slaw.new({Dir.new(".") => "you don't exist, go away"})
    end
  end

  def test_bogus_map_value
    assert_raises(ArgumentError) { Slaw.new({"foo" => /bar/}) }
  end

  def test_symbols_in_map
    s = Slaw.new({:bat => :mitzvah})
    q = Slaw.new({"bat" => "mitzvah"})
    obsert_equal(s.to_s, q.to_s)
  end

  def obsert_equal(a, b)
    if (a.respond_to?(:encoding))
      assert_equal(a.encoding, b.encoding)
    end
    assert_equal(a, b)
  end

  def actually_responds_to?(s, meth)
    begin
      # it would be nice if all these illegal duck-typed methods
      # throw the SlawTypeNoMethodError rather than an
      # ArgumentError, no matter how they are invoked. maybe some
      # more fiddling in Slaw.rb could accomplish that (gracefully,
      # harumph).
      #   p "calling :#{meth} on #{s.type_tag}"
      if (meth == :each  or  meth == :each_slaw)
        s.method(meth).call {}
      elsif (meth == :[]  or  meth == :nth_slaw  or  meth == :find_slaw)
        s.method(meth).call 0
      elsif (meth == :+)
        s.method(meth).call s
      else
        s.method(meth).call
      end
    rescue SlawTypeNoMethodError
      return false
    else
      return true
    end
  end

  def check_respond_to(s, meths_map)
    meths_map.each_pair do |meth, bool|
      assert_equal( bool, s.respond_to?(meth), ":#{meth} on #{s.type_tag}" )
      assert_equal( bool, actually_responds_to?(s,meth),
                    ":#{meth} on #{s.type_tag}" )
    end
  end

  def shuffled_slaw_list
    # shuffle is only in 1.9
    # sort_by {rand} is okay, since the random numbers are generated once
    # sort {rand} would, however, be the infamous "browser ballot bug"
    # http://www.robweir.com/blog/2010/02/microsoft-random-browser-ballot.html
    return [Slaw.new( 1, :int64 ),
            Slaw.new( 1, :float64 ),
            Slaw.new( 1, :unt64 ),
            Slaw.new( 1, :int8 ),
            Slaw.new( 2, :int8 ),
            Slaw.new( 1, :unt8 ),
            Slaw.new( 2, :float32 ),
            Slaw.new( ["large", "alarmed", "secret"] ),
            Slaw.new( "knismesis" ),
            Slaw.new( "gargalesis" )].sort_by {rand}
  end

  # These next two methods are adapted from Mattie's CMC code
  def MoveRequest(layout_index, targetID, newLoc, provenance)
    des = RequestDescrips( "move", layout_index, targetID )
    ing = Slaw.new( {"phys-loc"=>newLoc,
                     "provenance"=>provenance})
    return Protein.new( des, ing )
  end

  def RequestDescrips(request, layout_index, targetID)
    request_id_counter = 1
    source_id = 2
    return Slaw.new( ["layout-manager", "request", layout_index, source_id,
                      request_id_counter, request, targetID] )
  end

end
