#!/usr/bin/ruby

$VERBOSE = 1

require 'Pool'
include Plasma

PNAME = "many-participates-test-pool"

prot = Protein.new( ["descrip"],
                    { "ingest" => "something" } )

begin
  Pool.dispose(PNAME)
rescue
  # don't worry about it, you know
end

Pool.create(PNAME)
1.upto(10000) do |n|
  printf "%5d...\n", n
  h = Pool.participate(PNAME)
  h.deposit(prot)
  # uncomment the line below if you want to make it to 10000
  # GC.start
end
Pool.dispose(PNAME)
