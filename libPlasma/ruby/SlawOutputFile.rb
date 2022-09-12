

require 'Protein'
require 'Slaw'


module Plasma

class SlawOutputFile


  def initialize(fname, mode = :text)
    if (mode == :text)
      _c_output_open_text fname
    elsif (mode == :binary)
      _c_output_open_binary fname
    else
      raise "unrecognized slaw output mode #{mode}"
    end
  end

  def write(slaw)
    _c_output_write slaw
  end

end

end
