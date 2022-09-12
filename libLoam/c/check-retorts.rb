#!/usr/bin/env ruby
$VERBOSE = true

require 'set'

def check(h,c,base)
  hkeys = Set.new
  rex = Regexp.new(base)
  continued = nil
  IO.foreach(h) do |line|
    if line =~ rex and line =~ /^\#define\s+(\w+)\s+/ and $1 != base
      hkeys.add($1)
      continued = nil
    elsif line =~ /^\#define\s+(\w+)\s+\\/
      continued = $1
    elsif line =~ rex and continued
      hkeys.add(continued)
      continued = nil
    else
      continued = nil
    end
  end
  ckeys = Set.new
  IO.foreach(c) do |line|
    if line =~ /^\s+E\s*\((\w+)\)/
      ckeys.add($1)
    elsif line =~ /^\s+F\s*\((\w+),/
      ckeys.add($1)
    end
  end
  (hkeys - ckeys).each do |retort|
    print "#{retort} is in #{h} but not #{c}\n"
  end
  (ckeys - hkeys).each do |retort|
    print "#{retort} is in #{c} but not #{h}\n"
  end
end

def checkRange(h,c,first,last)
  hkeys = Set.new
  r1 = Regexp.new(first)
  rl = Regexp.new(last)
  inside = false
  IO.foreach(h) do |line|
    if line =~ r1
      inside = true
    elsif line =~ rl
      inside = false
    elsif inside and line =~ /^\#define\s+(\w+)\s+/
      hkeys.add($1)
    end
  end
  ckeys = Set.new
  IO.foreach(c) do |line|
    if line =~ /^\s+E\s*\((\w+)\)/
      ckeys.add($1)
    end
  end
  (hkeys - ckeys).each do |retort|
    print "#{retort} is in #{h} but not #{c}\n"
  end
  (ckeys - hkeys).each do |retort|
    print "#{retort} is in #{c} but not #{h}\n"
  end
end

Dir.chdir(File.dirname(__FILE__) + '/../..')
checkRange("libLoam/c/ob-retorts.h", "libLoam/c/ob-retorts.c", "common codes", "add new success codes here")
checkRange("libPlasma/c/plasma-retorts.h", "libPlasma/c/slaw.c", "Slaw error codes", "endif")
check("libAfferent/AfferentRetorts.h", "libAfferent/AfferentRetorts.cpp", "OB_RETORTS_AFFERENT_FIRST")
check("libBasement/BasementRetorts.h", "libBasement/BasementRetorts.cpp", "OB_RETORTS_BASEMENT_FIRST")
check("libImpetus/ImpetusRetorts.h", "libImpetus/ImpetusRetorts.cpp", "OB_RETORTS_IMPETUS_FIRST")
check("libLoam/c++/LoamxxRetorts.h", "libLoam/c++/LoamxxRetorts.cpp", "OB_RETORTS_LOAMXX_FIRST")
check("libMedia/JpegImageClot.h", "libMedia/JpegImageClot.cpp", "OB_JPEG_RETORTS")
check("libNoodoo/NoodooRetorts.h", "libNoodoo/NoodooRetorts.cpp", "OB_RETORTS_NOODOO_FIRST")
check("system/libProtist/ProtistRetorts.h", "system/libProtist/ProtistRetorts.cpp", "OB_RETORTS_PROTIST_FIRST")
check("libGanglia/GangliaRetorts.h", "libGanglia/GangliaRetorts.cpp", "OB_RETORTS_GANGLIA_FIRST")
check("libPlasma/c++/PlasmaxxRetorts.h", "libPlasma/c++/Slaw.cpp", "OB_RETORTS_PLASMAXX_FIRST")
