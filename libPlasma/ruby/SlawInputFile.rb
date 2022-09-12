
require 'Protein'
require 'Slaw'


module Plasma

class SlawInputFile


  def initialize(fname)
    _c_input_open fname
  end

  def read()
    _c_input_read
  end

end

end
