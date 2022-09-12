package DocToHtml;

use warnings;
use strict;

use Carp;
use FindBin ();

use lib "$FindBin::RealBin";
use MlGen;

sub HeadingLevel { 'HeadingLevel' }
sub NoSpace { 'NoSpace' }

sub Gen {
    my $html = Html( { HeadingLevel() => 1 }, $_[0] );
    my $docType =
        '<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" '
        .
        '"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">'
        .
        "\n";

    $docType . MlGen::Gen( $html );
}


#

sub Html {
    my ( $htmlInfo, $docElement ) = @_;

    my $refDocElement = ref $docElement;

    if ( $refDocElement eq '' ) {
        return $docElement;
    }

    if ( $refDocElement ne 'ARRAY' ) {
        confess "expected ref to ARRAY, got ref to $refDocElement";
    }

    my ( $tag, @rest ) = @$docElement;
    if ( $tag eq 'Doc' ) {
        my ( $titleText, @body ) = @rest;

        my $htmlAttr = {
            'xmlns' => 'http://www.w3.org/1999/xhtml',
            'lang' => 'en',
            'xml:lang' => 'en'
        };

        my $meta = [ 'meta', {
            'http-equiv' => 'Content-Type',
            'content' => 'text/html;charset=utf-8' } ];

        my $title = Tag( 'title', $htmlInfo, $titleText );

        my $head = [ 'head', {}, $meta, $title ];

        return [
            'html',
            $htmlAttr,
            $head,
            Tag( 'body', $htmlInfo, @body ) ];
    }
    elsif ( $tag eq 'Section' ) {
        my ( $title, @body ) = @rest;
        my $ht = 'h' . ImHash::Get( $htmlInfo, HeadingLevel() );
        return (
            Tag( $ht, $htmlInfo, $title ),
            MapHtml( ImHash::Increment( $htmlInfo, HeadingLevel ), @body ) );
    }
    elsif ( $tag eq 'NoSpace' ) {
        return Tag( 'NoSpace', $htmlInfo, @rest );
    }
    elsif ( $tag eq 'Paragraph' ) {
        return Tag( 'p', $htmlInfo, @rest );
    }
    elsif ( $tag eq 'OrderedList' ) {
        return Tag( 'ol', $htmlInfo, @rest );
    }
    elsif ( $tag eq 'UnOrderedList' ) {
        return Tag( 'ul', $htmlInfo, @rest );
    }
    elsif ( $tag eq 'ListItem' ) {
        return Tag( 'li', $htmlInfo, @rest );
    }
    elsif ( $tag eq 'Variable' ) {
        return Tag( 'i', $htmlInfo, @rest );
    }
    elsif ( $tag eq 'Anchor' )
    {
        my ( $href, @body ) = @rest;
        return
            [
             'a',
             { 'href' => $href },
             MapHtml( $htmlInfo, @body )
            ];
    }
    else {
        confess "unrecognized tag $tag";
    }
}

sub Tag {
    my ( $tag, $htmlInfo, @rest ) = @_;
    [ $tag, {}, MapHtml( $htmlInfo, @rest ) ];
}

sub MapHtml {
    my ( $htmlInfo, @rest ) = @_;
    map { Html( $htmlInfo, $_ ) } @rest;
}

1;
