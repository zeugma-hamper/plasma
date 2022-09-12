# Package Ruby Plasma up as a gem.

## HOWTO

### Build

    gem build rubyPlasma.gemspec

### Install

After building:

    gem install rubyPlasma-*.gem

or possibly

    sudo gem install rubyPlasma-*.gem

### Using the gem

    irb -rubygems
    >>> require 'Pool'
    >>> include Plasma
    >>> h = Pool.participate('wands')
    >>> # etc.
