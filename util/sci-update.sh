#!/bin/bash -eu

# Upgrade to a newer version of scintilla
# Note: source archive must be in *.tgz format! 
# Example:
#   % sci-update.sh /your/path/to/scintilla174.tgz


fail ()
{
  echo "${0##*/}: ERROR"
  printf "$1" >&2
  echo
  exit 1
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

[ -f "${1}" ] || fail "Not a regular file: ${1}"

[ -r "${1}" ] || fail "Access denied: ${1}"



case "${1##*/}" in
  scintilla?*.tgz)
  ;;
  *)
  fail "Filename should match pattern: 'scintilla?*.tgz'\nGot: '${1##*/}'"
esac

tar -ztf "${1}" &> /dev/null || fail "Invalid or corrupted archive:\n ${1}"

scidir="$(tar -ztf "${1}" | while read entry; do echo "${entry%%/*}"; done | sort | uniq)"

[ "${scidir}" = "scintilla" ] || fail "Archive has an unexpected directory structure."

backup="scintilla.old"

if [ -e include ] || [ -e scintilla ]
then

  backup="scintilla.old"
  [ -d  ${backup} ] && fail \
    "Directory already exists:\n  ${backup}\n(You should remove it first.)"  
  [ -e "${backup}" ] && fail \
    "Will not overwrite existing file:\n  ${backup}\n(You should remove it first.)"

  mkdir "${backup}"

  if [ -e include ]
  then
    if [ -d include ] 
    then
      cp -a include "${backup}/."
    else
      mv include "${backup}/."
      mkdir include
    fi
  fi

  [ -e scintilla ] && mv scintilla "${backup}"

fi


tar -zxf "${1}"

mv scintilla/include/Scintilla.h include
mv scintilla/include/SciLexer.h include


for dir in "scintilla" "scintilla/src"
do
  if [ -f "${backup}/${dir}/Makefile.am" ]
  then
    if [ -e "./${dir}/Makefile.am" ]
    then
      echo "Warning: Not updating existing file: ./${dir}/Makefile.am" >&2
    else
      cp "${backup}/${dir}/Makefile.am" "./${dir}/Makefile.am"
    fi
  else
    echo "Warning: file not found: ${backup}/${dir}/Makefile.am" >&2
  fi
done

rm -f "scintilla/src/SciTE.properties"
mkdir "scintilla/doctmp/"
mv "scintilla/doc/ScintillaDoc.html" "scintilla/doctmp/."

( cd scintilla; rm -rf *.bat macosx tgzsrc vcbuild win32 bin doc gtk cocoa test )
mv "scintilla/doctmp" "scintilla/doc"
( cd scintilla/include; patch -p0 < ../../util/Platform.h.patch )



(
  cd 'scintilla.old/scintilla'
  find -type d -name 'CVS' | while read dir
  do
    dest="../../scintilla/${dir}"
    [ -e "${dest}" ] && continue
    cp -ai "${dir}" "${dest}" || true
  done
)

if [ -f "scintilla/version.txt" ]
then
  version=$(head -n 1 "scintilla/version.txt")
  case "${version}"
  in 
    [0-9][0-9][0-9])
      major_version="${version:0:1}"
      minor_version="${version:1:2}"
    ;;
    *)
      major_version=""
      minor_version=""
    ;;
  esac
  if [ "${major_version}" ] && [ "${minor_version}" ]
  then
    sed -i "s/^MAJOR_VERSION=.*/MAJOR_VERSION=${major_version}/" configure.in
    sed -i "s/^MINOR_VERSION=.*/MINOR_VERSION=${minor_version}/" configure.in
    sed -i "s/^PATCH_LEVEL=.*/PATCH_LEVEL=0/" configure.in
    grep "^MAJOR_VERSION=" configure.in
    grep "^MINOR_VERSION=" configure.in
    grep "^PATCH_LEVEL=" configure.in
  else
    echo "Warning: could not parse scintilla/version.txt" >&2
  fi
else
  echo "Warning: could not find scintilla/version.txt" >&2
fi


