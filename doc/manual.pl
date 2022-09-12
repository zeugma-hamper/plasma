#!/usr/bin/perl
use strict;

# usage: perl manual.pl ../apps/GrabLet.h ../apps/GrabLet.cpp [...]

my $files = join " ", @ARGV;
open (IFILE, "Doxyfile-manual-template") || die "could not open config read\n";
open (OFILE, ">Doxyfile-single")         || die "could not open config write\n";

while (<IFILE>) {
    s/^INPUT\s+=(.*)/INPUT = $files/;
    print OFILE "$_";
}

exec "doxygen Doxyfile-single"
