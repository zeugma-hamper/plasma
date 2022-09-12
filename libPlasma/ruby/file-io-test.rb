
require_relative './test-helpers.rb'

require 'SlawInputFile'
require 'SlawOutputFile'
include Plasma

FNAME =  "scratch/slaw-out-test.slawx"

class FileIOTest < Test::Unit::TestCase
  # Old Test::Unit ran classes in alphabetical order.
  # New Minitest::Unit runs classes and cases in random order by default.
  # It's hard to be portable between the two.
  # So let's simplly put all our cases in one big happy case.

  def test_file_io
    begin
      File.unlink FNAME
    rescue Errno::ENOENT
    end
    outf = SlawOutputFile.new FNAME
    (0..9).each do |q|
      outf.write Protein.new( ["out-test"],
                              { "q" => q } )
    end
    assert( true, "finished writing without a hitch" )
    outf = nil

    inf = SlawInputFile.new FNAME
    q = 0
    while (s = inf.read)
      assert_equal( q, s.ingests["q"] )
      q = q+1
    end
    assert_equal( q, 10 )
    inf = nil

    begin
      File.unlink FNAME
    rescue Errno::ENOENT
    end
    outf = SlawOutputFile.new FNAME, :binary
    (0..9).each do |qq|
      outf.write Protein.new( ["out-test"],
                              { "q" => qq } )
    end
    assert( true, "finished writing without a hitch" )
    outf = nil

    inf = SlawInputFile.new FNAME
    q = 0
    while (s = inf.read)
      assert_equal( q, s.ingests["q"] )
      q = q+1
    end
    assert_equal( q, 10 )
  end
end
