#!/usr/bin/perl

use warnings;
use strict;

use File::Basename;
use FindBin ();

use lib "$FindBin::RealBin";
use Doc;
use DocToHtml;

exit( Main() == 0 );

sub Main
{
    if ($ARGV[0] eq '-o') {
        shift @ARGV;
        my $outfile;
        open $outfile, '>', $ARGV[0] || die "can't create ".$ARGV[0];
        shift @ARGV;
        select $outfile;
    }
    my @sortedArgv = sort { fileparse( $a ) cmp fileparse( $b ) } @ARGV;

    my @htmlManPages = map { Pa( HrefAndName( $_ ) ) } @sortedArgv;

    my $doc = Doc::Doc(
        'g-speak documentation'
        ,
        Doc::Section(
            'g-speak documentation',
            Doc::Section(
                'native HTML',
                Pa( 'option-map-keys.html', 'Pool option map keys' ),
                Pa( 'plasma-tls.html', 'Transport Layer Security for plasma™' ),
                Pa( 'retorts.html', 'ob_retort documentation' )
            ),
            Doc::Section(
                'man pages in HTML format',
                @htmlManPages
            )
        )
        );

    print DocToHtml::Gen( $doc );

    1;
}

sub Pa
{
    my ( $href, $body ) = @_;
    Doc::Paragraph( Doc::Anchor( $href, $body ) );
}

sub HrefAndName
{
    my ( $htmlFileRelativeToSrcDir ) = @_;

    my $nameNoDotPod = fileparse( $htmlFileRelativeToSrcDir, '.pod', '-man.html' );

    ( "htmlman/$nameNoDotPod-man.html", $nameNoDotPod );
}
