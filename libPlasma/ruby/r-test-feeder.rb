
# used by pool-tests.rb to deposit proteins into multi-awaiting gang
# pools

require 'Pool'
include Plasma

pname                   = ARGV[0]
num_proteins_to_deposit = ARGV[1].to_i
start_delay             = ARGV[2].to_f
inter_protein_delay     = ARGV[3].to_f

sleep start_delay

h = Pool.participate pname

(0...num_proteins_to_deposit).each do |q|
  prot = Protein.new( ["r-test-feeder.rb"],
                      { "count" => q } )
  h.deposit prot
  sleep inter_protein_delay
end

