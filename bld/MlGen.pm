package MlGen;

use warnings;
use strict;

use Carp;

use ImHash;

sub IndentLevel { 'IndentLevel' }
sub NoSpace { 'NoSpace' }

sub Gen {
    Flatten( { IndentLevel() => 0, NoSpace() => 0 }, $_[0] ) . "\n";
}

sub Indent { sprintf( "%*.s", $_[0] * 2, ' ' ) }

sub Flatten {
    my ( $flattenInfo, $mlElement ) = @_;

    my $i = ImHash::Get( $flattenInfo, IndentLevel() );
    my $noSpace = ImHash::Get( $flattenInfo, NoSpace() );

    my $indent = $noSpace ? '' : Indent( $i );

    my $refMlElement = ref $mlElement;

    if ( $refMlElement eq '' ) {
        return $indent . EscapeMl( $mlElement );
    }

    if ( $refMlElement ne 'ARRAY' ) {
        confess "expected ref to ARRAY, got ref to $refMlElement";
    }

    my ( $tag, $attributes, @rest ) = @$mlElement;

    if ( $tag eq 'NoSpace' ) {
        return $indent . join(
            '',
            MapFlatten( ImHash::Set( $flattenInfo, NoSpace, 1 ), @rest ) );
    }
    elsif ( ! @rest ) {
        return $indent . SelfCloseTag( $tag, $attributes );
    }

    join(
        $noSpace ? '' : "\n",
        $indent . BeginTag( $tag, $attributes ),
        MapFlatten( ImHash::Increment( $flattenInfo, IndentLevel ), @rest ),
        $indent . EndTag( $tag ) );
}

sub BeginOrSelfCloseTag {
    my ( $tag, $attributes, $maybeSlash ) = @_;

    # xxx don't attributes need additional escaping, e.g. for double quotes?
    my @attributeAssignments = map { $_ . '="' . EscapeMl( $attributes->{$_} ) . '"' } sort keys %$attributes;

    # xxx shouldn't attributes be one per line and indented in case there are lots of them?
    my $tagAndAttr = join( ' ', $tag, @attributeAssignments );

    '<' . $tagAndAttr . $maybeSlash . '>';
}

sub BeginTag { BeginOrSelfCloseTag( @_, '' ) }

sub SelfCloseTag { BeginOrSelfCloseTag( @_, '/' ) }

sub EndTag {
    my ( $tag ) = @_;
    BeginTag( '/' . $tag, {} );
}

sub EscapeMl {
    $_[0] =~ s/&/&amp;/g;
    $_[0] =~ s/</&lt;/g;
    $_[0] =~ s/>/&gt;/g;
    $_[0];
}

sub MapFlatten {
    my ( $flattenInfo, @rest ) = @_;
    map { Flatten( $flattenInfo, $_ ) } @rest;
}

1;
