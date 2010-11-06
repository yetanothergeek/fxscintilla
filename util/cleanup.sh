#!/bin/bash -e


abs=$(readlink -f "${0}")
rel="./util/${0##*/}"


if [ -f "${rel}" ] && [ -f "${abs}" ] && [ $(stat -c "%i" ${abs}) -eq $(stat -c "%i" ${rel}) ]
then
  unset abs
  unset rel
else
  dir=${abs%/*}
  printf "***FATAL***\nWorking directory must be:\n  %s/\n" "${dir%/*}" 1>&2
  exit 1
fi


rm -f configure tests/test fox/version.h fxscintilla-*.tar.gz

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


