AC_DEFUN([CHECK_FOX],
# $1 = FOX MAJOR VERSION
# $2 = FOX_MINOR_VERSION

#
# Handle user hints
#
[
AC_ARG_WITH(foxinclude, [  --with-foxinclude=DIR   use Fox 1.0 includes from DIR],
        FOX_INCLUDE_DIR=$withval, FOX_INCLUDE_DIR=/usr/local/include)
AC_ARG_WITH(foxlib,     [  --with-foxlib=DIR       use Fox 1.0 libs from DIR],
        FOX_LIB_DIR=$withval, FOX_LIB_DIR=/usr/local/lib)

#
# -pthread trick for FreeBSD
#
case "${host}" in
*-*-freebsd*)
  ACX_PTHREAD
  ;;
esac

#
# Check Fox headers and library
#
FOX_OLD_CPPFLAGS=$CPPFLAGS
FOX_OLD_LDFLAGS=$LDFLAGS
CPPFLAGS="$PTHREAD_CFLAGS $CPPFLAGS -I${FOX_INCLUDE_DIR}"
LDFLAGS="$LDFLAGS -L${FOX_LIB_DIR}"
AC_LANG_SAVE
AC_LANG_C
AC_CHECK_HEADER(fox/fx.h,, AC_MSG_ERROR(FOX needed))
AC_CHECK_LIB(FOX, fxfindfox, LIBS=${LIBS}, AC_MSG_ERROR(FOX needed))
#
# Check Fox version
#
AC_MSG_CHECKING(FOX version $1.$2.x)
AC_TRY_RUN([
	#include <fox/fxver.h>
	int main(int argc, char** argv) {
		return (FOX_MAJOR == $1 && FOX_MINOR == $2) ? 0 : -1;
	}
	], AC_MSG_RESULT(yes), AC_MSG_RESULT(no); AC_MSG_ERROR(Incompatible FOX version), AC_MSG_WARN(Cross compiling))
#
# End
#
CPPFLAGS=$FOX_OLD_CPPFLAGS
LDFLAGS=$FOX_OLD_LDFLAGS
AC_LANG_RESTORE
AC_SUBST(FOX_INCLUDE_DIR)
AC_SUBST(FOX_LIB_DIR)
])
