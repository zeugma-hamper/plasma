package ImHash;

use warnings;
use strict;

use Carp;

sub Get {
    my ( $rh, $key ) = @_;

    my $e = "error during Get of key '$key'";

    my $ar = ref( $rh );
    my $er = 'HASH';
    if ( $ar ne $er ) {
        confess( "$e: unexpected ref to '$ar'; expected ref to '$er'" );
    }

    if ( ! exists $rh->{$key} ) {
        confess( "$e: key does not exist" );
    }

    $rh->{$key};
}

sub Set {
    my ( $rh, $key, $val ) = @_;

    Get( $rh, $key );

    return { %$rh, $key => $val };
}

sub SetBasedOnSelf {
    my ( $rh, $key, $f ) = @_;

    my $val = $f->( Get( $rh, $key ) );

    return { %$rh, $key => $val };
}

sub Next { $_[0] + 1 }

sub Increment { SetBasedOnSelf( @_, \&Next ); }

1;
