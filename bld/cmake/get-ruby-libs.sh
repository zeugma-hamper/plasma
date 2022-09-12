#!/bin/sh
ruby -rrbconfig -e "puts RbConfig::CONFIG['LIBRUBYARG_SHARED']"
