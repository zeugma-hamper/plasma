#!/usr/bin/perl -w

# Example usage:
# find libAfferent libBasement libImpetus libLoam libMedia libNoodoo libPlasma libTwillig system projects -name "*.[Cch]" | xargs libLoam/c/funny-codes.pl

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
    if ($line =~ /\bOB_[A-Z_]+_CODE\s*\(\s*([0-9a-fA-FxX]+)\s*,/) {
        $c = $1;
        $c = oct($c) if $c =~ /^0/;
    }

    return $c;
}

$Current = undef;

while (<>) {
    my $c = GetCode ($_);
    if ($c != 0) {
        my $prefix = ($c >> 16);
        if (defined $Current and $Current != $prefix) {
            print $ARGV, ':', $., ": ", sprintf ("0x%08x", $c), "\n";
        }
        $Current = $prefix;
    }
} continue {
    # reset line numbering on each input file (see "perldoc -f eof")
    if (eof) {     # Not eof()!
        close ARGV;
        $Current = undef;
    }
}
