#!/usr/bin/perl -w
use strict;
use warnings;

my $opener = <<'END';
class CLASSNAME
#ifdef PARENTCLASSNAME
                 :  public PARENTCLASSNAME
#endif
{
  public:
END

my $closer = <<'END';
#undef CLASSNAME
#undef CLASSNAMESTRING
#undef PARENTCLASSNAME
END


while ( <> ) {
  if ( index($_, '#include "libBasement/patella-helper.h"') == 0 ) {
    print $opener;
  } elsif ( index($_, '#include "libBasement/h-file-cleanup.h"') == 0 ){
    print $closer;
  } else {
    print;
  }
}
