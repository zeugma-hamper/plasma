#!/bin/sh
# Subset of bs_funcs.sh, just enough to provide the function bs_stamp_debian_changelog()
set -e

#---- begin verbatim from bs_funcs.sh ----

# Print a message and terminate with nonzero status.
bs_abort() {
    echo fatal error: $*
    exit 1
}

# Echo the version number of this product as given by git
# This works for projects that name branches like kernel.org, Wine, or Node do
bs_get_version_git() {
    # git describe --long's output looks like
    # name-COUNT-CHECKSUM
    # or, if at a tag,
    # name
    d1=`git describe --long`
    # Strip off -CHECKSUM suffix, if any
    d2=`echo $d1 | sed 's/-g[a-z0-9][a-z0-9][a-z0-9][a-z0-9][a-z0-9][a-z0-9][a-z0-9]*$//'`
    # Strip off -COUNT suffix, if any
    d3=`echo $d2 | sed 's/-[0-9]*$//'`
    # Remove non-numeric prefix (e.g. rel- or debian/), if any
    d4=`echo $d3 | sed 's/^[^0-9]*//'`
    # Remove non-numeric suffix (e.g. -mz-gouda), if any
    d5=`echo $d4 | sed 's/-[^0-9]*$//'`
    # Remove suffixes that contain internal digits but not dashes (eg -p2mz)
    d6=`echo $d5 | sed 's/-[^0-9][^-]*$//'`
    # FIXME: we can't handle suffixes that contain dashes and digits
    case "$d6" in
    "") bs_abort "can't parse version number from git describe --long's output $d1";;
    esac
    echo $d6
}

# Echo the change number since the start of this branch as given by git
bs_get_changenum_git() {
    # git describe --long's output looks like
    # name-COUNT-CHECKSUM
    # First strip off the checksum field, then the name.
    if ! d1=`git describe --long 2> /dev/null`
    then
        # No releases!  Just count changes since epoch.
        git log --oneline | wc -l | sed 's/^[[:space:]]*//'
        return 0
    fi
    d2=`echo $d1 | sed 's/-g[a-z0-9][a-z0-9][a-z0-9][a-z0-9][a-z0-9][a-z0-9][a-z0-9]*$//'`
    d3=`echo $d2 | sed 's/^.*-//'`
    case "$d3" in
    "") bs_abort "can't parse change number from git describe --long's output $d1";;
    esac
    echo $d3
}

#---- end verbatim from bs_funcs.sh ----

case $1 in
version) bs_get_version_git;;
changenum) bs_get_changenum_git;;
*) bs_abort "usage: $0 [version|changenum]";;
esac
