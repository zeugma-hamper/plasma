require 'Slaw'
require 'base64'


module Plasma

# A protein is the message envelope in Plasma. A protein is composed of:
# descrips:: An Array of message subject which can be used in filtering.
# ingests:: A Hash that is the message payload. The hash can be arbitrarily complex.
# rude_data:: Completely unstructured binary data. This can be absolutely anything.
class Protein < Slaw
  attr_accessor :hose
  attr_accessor :index
  attr_accessor :timestamp

  def initialize(descrips, ingests, rude="", _construct_type=:protein)
    super([descrips,ingests,rude], _construct_type)
  end

  # Returns the list of descrips
  def descrips
    _c_slaw_protein_descrips
  end

  # returns the Hash of ingests
  def ingests
    _c_slaw_protein_ingests
  end

  # returns the rude data
  def rude_data
    _c_slaw_protein_rude_data
  end

  # identity: returns this object
  def emit
    self
  end

  # Creates and returns a protein from a JSON object. The keys in the JSON object should be descrips, ingests, and optionally rude_data.
  def self.json_create(o)
    des = o['descrips']
    ing = o['ingests']
    rude = ""
    if des.nil?
      des = []
    end
    if ing.nil?
      ing = {}
    end
    if o.has_key?("rude_data")
      rude = Base64.decode64(o["rude_data"])
    end
    Protein.new(des, ing, rude)
  end
end

end
