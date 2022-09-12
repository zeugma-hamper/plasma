require 'Pool'

# #s = Plasma::Slaw.new(23.0, :int64)
# s = Plasma::Slaw.new(23.0)
# s._c_spew_stderr
# s = Plasma::Slaw.new(23)
# s._c_spew_stderr
# s = Plasma::Slaw.new("23")
# s._c_spew_stderr
# s = Plasma::Slaw.new(true)
# s._c_spew_stderr
# s = Plasma::Slaw.new(nil)
# s._c_spew_stderr

s = slaw(nil)
s._c_spew_stderr

s = Plasma::Slaw.new(23.0)
s._c_spew_stderr

# t = Plasma::Slaw.new(23.0)

# p s==t
# p s.eql?(t)

# p s==23.0
# p s==23

# p s.eql?(23.0)
# p s.eql?(23)
# exit

s = Plasma::Slaw.new( [1, 2, 3], :v3float32 );
s._c_spew_stderr

s = Plasma::Slaw.new( [1, 2, 3, 4], :v4unt64 );
s._c_spew_stderr

s = Plasma::Slaw.new( [1, 2], :v2unt8 );
s._c_spew_stderr

s = Plasma::Slaw.new( [1, 2, 3, 4], [:float32, :int8, :float64] )
s._c_spew_stderr

s = Plasma::Slaw.new( [1, 2, 3], :float32_array );
s._c_spew_stderr

s = Plasma::Slaw.new( [1, 2, 3, 4, 5, 6, 7], :unt16_array );
s._c_spew_stderr

s = Plasma::Slaw.new( [[1, 2, 3],[4, 5, 6]], :v3float32 );
s._c_spew_stderr

s = Plasma::Slaw.new( [[1, 2, 3],[4, 5, 6]], :v3float32_array );
s._c_spew_stderr

s = Plasma::Slaw.new( [[1, 2, 3, 4],[4, 5, 6, 7]], :v4int8_array );
s._c_spew_stderr

# fix:
s = v4int8(1,2,3,4);
s._c_spew_stderr

# s = Plasma::Slaw.new(400, :string)
# s._c_spew_stderr

t = Plasma::Slaw.new(23.0)
s = Plasma::Slaw.new([1, 2, t], :int8);
s._c_spew_stderr

u = Plasma::Slaw.new("two-point-two")
s = Plasma::Slaw.new( { "one" => 1.0,
                        "two" => 2.0,
                        u     => 2.2 } )
s._c_spew_stderr

p u.map?
p s.map?

print "----------\n"
t = Plasma::Slaw.new([1, 2, 3, 4])

u = Plasma::Slaw.new("two-point-two")
t = Plasma::Slaw.new( { "one" => 1.0,
                        "two" => 2.0,
                        u     => 2.2 } )

p t["two-point-two"]

s = slaw(4)
t = slaw(s)

p = protein(["one", "two", "three"],
            { "one" => 1.0,
              "two" => 2.0,
              u     => 2.2 })
p._c_spew_stderr

print "connecting to pool 'rt'\n"
ph = Plasma::Pool.new "rt"
print "depositing\n"
ph.deposit p

print "nexting\n"
prot = ph.next
p prot
p prot.protein?
p prot.descrips[0]
p prot.descrips[1]
p prot.descrips[2]
p prot.ingests["two-point-two"]
p prot.timestamp
p prot.index

print "rewind\n"
ph.rewind
prot = ph.next
p prot.index

print "seek by\n"
ph.seekby(1)
prot = ph.next
p prot.index

print "seek to\n"
ph.seekto(0)
prot = ph.next
p prot.index

print "to last\n"
ph.tolast
prot = ph.next
p prot.index


