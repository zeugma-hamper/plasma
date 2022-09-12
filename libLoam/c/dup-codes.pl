#!/usr/bin/perl -w

# Finds duplicate logging codes, some of which might be intentional
# (because the same or a very similar error is reported in more than one
# place in the code) but others might not be (cut and paste errors)
#
# Example usage:
# find libAfferent libBasement libImpetus libLoam libMedia libNoodoo libPlasma libTwillig system projects -name "*.[Cch]" | xargs libLoam/c/dup-codes.pl

sub GetCode {
    my $c = 0;
    my $line = $_[0];
    foreach my $func (qw(ob_log ob_log_fatal)) {
        if ($line =~ /\b$func\s*\(\s*[\&\w]+\s*,\s*([0-9a-fA-FxX]+)\s*,/) {
            $c = $1;
            $c = oct($c) if $c =~ /^0/;
        }
    }
    foreach my $func (qw(ob_log_loc ob_log_loc_fatal)) {
        if ($line =~ /\b$func\s*\([^,]+,[^,]+,\s*[\&\w]+\s*,\s*([0-9a-fA-FxX]+)\s*,/) {
            $c = $1;
            $c = oct($c) if $c =~ /^0/;
        }
    }
    if ($line =~ /\bOB[A-Z_]+_CODE\s*\(\s*([0-9a-fA-FxX]+)\s*,/) {
        $c = $1;
        $c = oct($c) if $c =~ /^0/;
    }
    if ($line =~ /\bpool_tcp_close\s*\(\s*([0-9a-fA-FxX]+)\s*,/) {
        $c = $1;
        $c = oct($c) if $c =~ /^0/;
    }

    return $c;
}

while (<>) {
    my $c = GetCode ($_);
    if ($c != 0) {
        push @{$Codes{$c}}, $ARGV . ':' . $.;
    }
} continue {
    # reset line numbering on each input file (see "perldoc -f eof")
    close ARGV if eof;     # Not eof()!
}

foreach my $code (sort {$a <=> $b} keys %Codes) {
    my @x = @{$Codes{$code}};
    if ($#x > 0) {
        printf "0x%08x:\n", $code;
        foreach my $y (@x) {
            print "    ", $y, "\n";
        }
    }
}
