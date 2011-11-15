#!/bin/bash -eu

# Upgrade to a newer version of scintilla
# Note: source archive must be in *.tgz format! 
#
# Example:
#   % ./util/sci-update.sh /your/path/to/scintilla174.tgz
#
# Or update directly from a scintilla CVS checkout:
#   % ./util/sci-update.sh /your/path/to/scintilla


fail ()
{
  echo "${0##*/}: ERROR"
  printf "$1" >&2
  echo
  exit 1
}


list_files () {
  (
    stat -c "%n" 'version.txt' 'License.txt' 'doc/ScintillaDoc.html' 'include/Scintilla.iface'
    find include lexlib lexers src -type f \( \
      -name '*.h' \
      -or -name '*.cxx' \
    \) -not -name 'FXScintilla.h'
  ) | sort
}


backup="../fxscintilla-backup-$(date +%Y-%m-%d).tar.gz"

backup_old ()
{
  [ -e "${backup}" ] && fail \
    "Will not overwrite existing file:\n  ${backup}\n(You should remove it first.)"
  [ -e "$PWD/tmp" ] && fail \
    "Temporary directory $PWD/tmp already exists"
  mkdir tmp
  list_files > ./tmp/old.files
  tar -zcf "${backup}" $(list_files)
}


abs=$(readlink -f "${0}")
rel="./util/${0##*/}"


if [ -f "${rel}" ] && [ -f "${abs}" ] && [ $(stat -c "%i" ${abs}) -eq $(stat -c "%i" ${rel}) ]
then
  unset abs
  unset rel
else
  dir=${abs%/*}
  fail "Working directory should be:\n ${dir%/*}/"
fi

[ $# -eq 1 ] || fail "Usage ${0##*/} <scintilla-archive>"

[ -e "${1}" ] || fail "File not found: ${1}"

[ -r "${1}" ] || fail "Access denied: ${1}"


if [ -f "${1}" ]
then 
  case "${1##*/}" in
    scintilla?*.tgz)
    ;;
    *)
    fail "Filename should match pattern: 'scintilla?*.tgz'\nGot: '${1##*/}'"
  esac
  tar -ztf "${1}" &> /dev/null || fail "Invalid or corrupted archive:\n ${1}"
  scidir="$(tar -ztf "${1}" | while read entry; do echo "${entry%%/*}"; done | sort | uniq)"
  [ "${scidir}" = "scintilla" ] || fail "Archive has an unexpected directory structure."
  backup_old
  tar -C ./tmp -zxf "${1}"
else 
  [ -d "${1}" ] || fail "Source object is not a file or a directory:\n ${1}"
  [ -x "${1}" ] || fail "Search permission denied for directory:\n ${1}"
  [ "${1##*/}" = "scintilla" ] || fail "Source directory must be named 'scintilla'"
  backup_old
  cp -ai "$1" "./tmp/"
fi


ALLOW_PATCH_FAIL=${ALLOW_PATCH_FAIL-false}

(
  cd './tmp/scintilla/include'
  patch --quiet -p0 < ../../../util/Platform.h.patch || $ALLOW_PATCH_FAIL
)

(
  cd './tmp/scintilla'
  list_files > '../new.files'
)

for somefile in $(cat ./tmp/old.files)
do
  if ! [ -e "./tmp/scintilla/${somefile}" ]
  then
    rm ${somefile}
  fi
done > './tmp/removed.files'


for somefile in $(cat ./tmp/new.files)
do
  if ! [ -e "${somefile}" ]
  then
    cp "./tmp/scintilla/${somefile}" "${somefile}"
    echo ${somefile}
  fi
done > './tmp/added.files'


for somefile in $(cat ./tmp/new.files)
do
  if [ -e "${somefile}" ]
  then
    if ! diff -q "${somefile}" "./tmp/scintilla/${somefile}" > /dev/null
    then
      cp -a "./tmp/scintilla/${somefile}" "${somefile}"
      echo ${somefile}
    fi
  fi
done > './tmp/changed.files'

rm -rf './tmp/scintilla' './tmp/new.files' './tmp/old.files'

for filelist in 'added' 'changed' 'removed'
do
  count=$(cat ./tmp/${filelist}.files | wc -l)
  if [ ${count}  -eq 0 ]
  then
    rm "./tmp/${filelist}.files"
  else
    echo "${count} files have been ${filelist}.  See: tmp/${filelist}.files"
  fi
done


if [ $(find tmp -mindepth 1 | wc -l) -eq 0 ]
then
  echo "Looks like nothing has changed."
  rmdir tmp
  rm -f "${backup}"
else
  echo "Some things have changed."
  echo "For details, see: ${PWD}/tmp"
  echo "Original files saved to $(readlink -f ${backup})"
fi

