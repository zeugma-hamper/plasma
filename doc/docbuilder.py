#!/usr/bin/env python

import os, sys

if len(sys.argv) != 3:
  print('usage: docbuilder.py odir ifile')
  print('Does "cd odir; mkdir -p build; doxygen ifile" portably.')
  sys.exit(1)

odir = sys.argv[1]
ifile = sys.argv[2]
os.chdir(odir)
if not os.path.isdir('build'):
  os.mkdir('build')
os.execlp('doxygen', 'doxygen', ifile)
