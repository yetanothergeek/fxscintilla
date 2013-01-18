#!/bin/bash -e

CLEANUP="./util/cleanup.sh"

if [ -e "${CLEANUP}" ] && [ -x "${CLEANUP}" ]
then 
  echo "Running '${CLEANUP}'"
  "${CLEANUP}"
else
  echo "Skipping '${CLEANUP}'"
fi

for CMD in \
 'mkdir -p m4' \
 'aclocal --force' \
 'libtoolize --force --copy' \
 'automake --foreign --add-missing --copy' \
 'autoconf'
do
  echo "Running '${CMD}'"
  ${CMD}
done
