AC_DEFUN([CHECK_LIBFOX],
[
#
# Checking for Cygwin and MinGW32
#
AC_REQUIRE([AC_CANONICAL_HOST])[]dnl
case $host_os in
  *cygwin* ) HAVE_CYGWIN=yes;;
  *mingw32* ) HAVE_MINGW32=yes;;
         * ) ;;
esac
#
# Handle user hints
#
AC_ARG_WITH(foxinclude, [  --with-foxinclude=DIR   use Fox 1.0 includes from DIR],
        FOX_INCLUDE_DIR=$withval, FOX_INCLUDE_DIR=/usr/local/include)
AC_ARG_WITH(foxlib,     [  --with-foxlib=DIR       use Fox 1.0 libs from DIR],
        FOX_LIB_DIR=$withval, FOX_LIB_DIR=/usr/local/lib)

#
# FLAGS backup
#
FOX_OLD_CXXFLAGS=$CXXFLAGS
FOX_OLD_LDFLAGS=$LDFLAGS
FOX_OLD_LIBS=$LIBS
AC_LANG_SAVE

#
# Temporary FLAGS setting
#
CXXFLAGS="$CXXFLAGS -I${FOX_INCLUDE_DIR}"
LDFLAGS="$LDFLAGS -L${FOX_LIB_DIR}"
LIBS=-lFOX
AC_LANG_CPLUSPLUS

if test x"$HAVE_CYGWIN" = xyes -o x"$HAVE_MINGW32" = xyes; then
  LDFLAGS="$LDFLAGS -mwindows"
	if test x"$HAVE_CYGWIN" = xyes; then
		CXXFLAGS="$CXXFLAGS -DWIN32"
	fi
fi

#
# Check Fox headers
#
AC_CHECK_HEADER(fox/fx.h,, AC_MSG_ERROR(FOX needed))

#
# Check FOX lib without pthread
#
dnl LDFLAGS="-mwindows $LDFLAGS"
AC_MSG_CHECKING(for FOX library)
AC_TRY_LINK_FUNC(fxfindfox, have_FOX=yes, have_FOX=no)
AC_MSG_RESULT($have_FOX)

#
# Check FOX lib with pthread
# (needed under FreeBSD when using MesaGL build with pthread support)
#
if test x"$have_FOX" = xno; then
  ACX_PTHREAD
	CXXFLAGS="$PTHREAD_CFLAGS $CXXFLAGS"
  AC_MSG_CHECKING(for FOX library with pthread)
  AC_TRY_LINK_FUNC(fxfindfox, have_FOX=yes, have_FOX=no)
  AC_MSG_RESULT($have_FOX)
fi

#
# Abort if no FOX lib
#

if test x"$have_FOX" = xno; then
  AC_MSG_ERROR(FOX needed)
fi

#
# End
#
CXXFLAGS=$FOX_OLD_CXXFLAGS
LDFLAGS=$FOX_OLD_LDFLAGS
LIBS=$FOX_OLD_LIBS
AC_LANG_RESTORE
AC_SUBST(FOX_INCLUDE_DIR)
AC_SUBST(FOX_LIB_DIR)

# Finally, execute ACTION-IF-OK/ACTION-IF-NOT-OK:
if test x"$have_FOX" = xyes; then
        ifelse([$1],,AC_DEFINE(HAVE_FOX,1,[Define you have FOX library and header files.]),[$1])
        :
else
        $2
        :
fi
])

AC_DEFUN([CHECK_LIBFOX_VERSION],
dnl $1 = FOX MAJOR VERSION
dnl $2 = FOX_MINOR_VERSION
dnl $3 = ACTION-IF-OK
dnl $4 = ACTION-IF-NOT-OK
[
#
# FLAGS backup
#
FOX_OLD_CXXFLAGS=$CXXFLAGS
FOX_OLD_LDFLAGS=$LDFLAGS
AC_LANG_SAVE

#
# Temporary FLAGS setting
#
CXXFLAGS="$CXXFLAGS $PTHREAD_CFLAGS -I${FOX_INCLUDE_DIR}"
LDFLAGS="$LDFLAGS -L${FOX_LIB_DIR} -lFOX"
AC_LANG_CPLUSPLUS

#
# Check Fox version
#
AC_MSG_CHECKING(FOX version $1.$2.x)
AC_TRY_RUN([
	#include <fox/fxver.h>
	int main(int argc, char** argv) {
		return (FOX_MAJOR == $1 && FOX_MINOR == $2) ? 0 : -1;
	}
	], AC_MSG_RESULT(yes); check_libfox_version_ok=yes, AC_MSG_RESULT(no), AC_MSG_WARN(Cross compiling))

#
# Restore FLAGS
#
CXXFLAGS=$FOX_OLD_CXXFLAGS
LDFLAGS=$FOX_OLD_LDFLAGS
AC_LANG_RESTORE

# Finally, execute ACTION-IF-OK/ACTION-IF-NOT-OK:
if test x"$check_libfox_version_ok" = xyes; then
        ifelse([$3],,AC_DEFINE(HAVE_FOX_$1_$2,1,[Define FOX version $1.$2.]),[$3])
        :
else
        check_libfox_version_ok=no
        $4
fi
])
