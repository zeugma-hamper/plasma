package Doc;

use warnings;
use strict;

sub Doc { [ 'Doc', @_ ] }

sub Section { [ 'Section', @_ ] }

sub Paragraph { [ 'Paragraph', @_ ] }

sub OrderedList { [ 'OrderedList', @_ ] }

sub UnOrderedList { [ 'UnOrderedList', @_ ] }

sub ListItem { [ 'ListItem', @_ ] }

sub Variable { [ 'Variable', @_ ] }

sub NoSpace { [ 'NoSpace', @_ ] }

sub Anchor { [ 'Anchor', @_ ] }

1;
