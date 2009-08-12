#!/bin/bash

rm -f configure tests/test src/version.h fxscintilla-*.tar.gz

find  -name 'Makefile' \
  -or -name 'Makefile.in' \
  -or -name '*.o' \
  -or -name '*.lo' \
  -or -name '*.la' \
  -or -name '.deps' \
  -or -name '.libs' \
  -or -name 'autom4te.cache' \
  -or -name 'config.guess' \
  -or -name 'config.sub' \
  -or -name 'depcomp' \
  -or -name 'install-sh' \
  -or -name 'ltmain.sh' \
  -or -name 'missing' \
  -or -name '*.m4' \
  -or -name 'm4' \
  -or -name 'missing' \
  -or -name 'config.log' \
  -or -name 'config.status' \
  -or -name 'libtool' \
  -or -name 'fxscintilla.pc' \
| while read name
do
  rm -rf ${name}
done


