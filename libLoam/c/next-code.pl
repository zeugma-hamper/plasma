#!/usr/bin/perl -w

$Code = 0;

while (<>) {
    foreach my $func (qw(ob_log ob_log_fatal)) {
        if (/\b$func\s*\(\s*[\&\w]+\s*,\s*([0-9a-fA-FxX]+)\s*,/) {
            my $c = $1;
            $c = oct($c) if $c =~ /^0/;
            $Code = $c if ($c > $Code);
        }
    }
    foreach my $func (qw(ob_log_loc ob_log_loc_fatal)) {
        if (/\b$func\s*\([^,]+,[^,]+,\s*[\&\w]+\s*,\s*([0-9a-fA-FxX]+)\s*,/) {
            my $c = $1;
            $c = oct($c) if $c =~ /^0/;
            $Code = $c if ($c > $Code);
        }
    }
    if (/\bOB[A-Z_]+_CODE\s*\(\s*([0-9a-fA-FxX]+)\s*,/) {
        my $c = $1;
        $c = oct($c) if $c =~ /^0/;
        $Code = $c if ($c > $Code);
    }
    if (/\bpool_tcp_close\s*\(\s*([0-9a-fA-FxX]+)\s*,/) {
        my $c = $1;
        $c = oct($c) if $c =~ /^0/;
        $Code = $c if ($c > $Code);
    }
}

if ($Code == 0) {
    print "No codes found in $ARGV.  You'll need to allocate a range for it.\n";
    print "Look for a file LogCodeAssignments.txt in this directory or a parent\n";
    print "directory, or else look in libLoam/c/ob-log.h.\n";
    exit 1;
} else {
    printf "0x%08x\n", 1 + $Code;
}
