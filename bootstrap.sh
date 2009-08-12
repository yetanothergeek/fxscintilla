#!/bin/bash -e

./util/cleanup.sh

aclocal --force
libtoolize --force --copy
automake --foreign --add-missing --copy
autoconf
