AC_INIT([fxscintilla],3.3.9,[yetanothergeek@gmail.com])
dnl AC_CONFIG_AUX_DIR(config)
AC_CONFIG_MACRO_DIR([m4])


dnl Check Cygwin and MinGW32
AC_CANONICAL_HOST
case $host_os in
  *cygwin* )  HAVE_CYGWIN=yes;;
  *mingw32* ) HAVE_MINGW32=yes;;
  * ) ;;
esac

AM_CONDITIONAL(HAVE_CYGWIN, test x"$HAVE_CYGWIN" = xyes)
AM_CONDITIONAL(HAVE_MINGW32, test x"$HAVE_MINGW32" = xyes)

dnl Using libtool
if test x"$HAVE_CYGWIN" = xyes -o x"$HAVE_MINGW32" = xyes; then
  AC_DISABLE_SHARED
fi
AM_PROG_LIBTOOL

dnl Must have a C++ compiler
AC_PROG_CXX

dnl Versioning
dnl - FXScintilla versioning scheme
MAJOR_VERSION=3
MINOR_VERSION=3
PATCH_LEVEL=9
AC_SUBST(MAJOR_VERSION)
AC_SUBST(MINOR_VERSION)
AC_SUBST(PATCH_LEVEL)
AM_INIT_AUTOMAKE([-Wall])

dnl - Libtool versioning scheme
dnl   current:revision:age
dnl   . start with 0:0:0
dnl   . if the source code changed, then increment revision
dnl   . if any interface has been added, removed or changed, increment
dnl     current and set revision to 0
dnl   . if backward compatibility has been preserved, then increment age
dnl   . if backward compatibility has been broken, then set age to 0
LT_FXSCINTILLA_CURRENT=24
LT_FXSCINTILLA_REVISION=3
LT_FXSCINTILLA_AGE=2
AC_SUBST(LT_FXSCINTILLA_CURRENT)
AC_SUBST(LT_FXSCINTILLA_REVISION)
AC_SUBST(LT_FXSCINTILLA_AGE)

LT_FXSCINTILLANOLEXER_CURRENT=21
LT_FXSCINTILLANOLEXER_REVISION=0
LT_FXSCINTILLANOLEXER_AGE=1
AC_SUBST(LT_FXSCINTILLANOLEXER_CURRENT)
AC_SUBST(LT_FXSCINTILLANOLEXER_REVISION)
AC_SUBST(LT_FXSCINTILLANOLEXER_AGE)

dnl Enabling libfxscintillanolexer
AC_MSG_CHECKING(for nolexer library build)
AC_ARG_ENABLE(nolexer, [  --enable-nolexer        enable libfxscintillanolexer [default=no]],,
              enable_nolexer=no)
AC_MSG_RESULT([$enable_nolexer])
AM_CONDITIONAL(NOLEXER, test x"$enable_nolexer" = xyes)

dnl #Let pkgconfig handle FOX flags unless they are set in the environment
if test "x${FOX_LIBS}${FOX_CFLAGS}" = "x"
then
  PKG_PROG_PKG_CONFIG()
  FOX_PKG_NAME=fox
  for L in fox18 fox17 fox
  do
    AC_MSG_CHECKING([for $L.pc])
    if ${PKG_CONFIG} --exists ${L}
    then
      AC_MSG_RESULT([yes])
      FOX_PKG_NAME=${L}
      break
    else
      AC_MSG_RESULT([no])
    fi
  done
  PKG_CHECK_MODULES([FOX], [${FOX_PKG_NAME} >= 1.7],
    [
      AC_MSG_RESULT([Found FOX-1.7 or newer])
      AC_DEFINE(FOX_1_7)
    ],
    [
      AC_MSG_RESULT([Could not find FOX-1.7 or newer])
      PKG_CHECK_MODULES([FOX], [fox >= 1.6],
        AC_MSG_RESULT([Found FOX-1.6.x])
        AC_DEFINE(FOX_1_6))
    ]
  )
else
  if test "x${FOX_LIBS}" = "x"
  then
    AC_MSG_ERROR([If you set FOX_CFLAGS, you must also set FOX_LIBS])
  fi
  if test "x${FOX_CFLAGS}" = "x"
  then
    AC_MSG_ERROR([If you set FOX_LIBS, you must also set FOX_CFLAGS])
  fi
  AC_MSG_CHECKING([for FOX version in FOX_LIBS])
  case "${FOX_LIBS}" in
    *FOX-1.6*)
      AC_MSG_RESULT([looks like version 1.6])
      AC_DEFINE(FOX_1_6)
    ;;
    *FOX-1.7*|*FOX-1.8*)
      AC_MSG_RESULT([looks like version 1.7 or newer])
      AC_DEFINE(FOX_1_7)
    ;;
    *)
      AC_MSG_RESULT([FAILED])
      AC_MSG_WARN([could not find FOX version, assuming 1.6])
      AC_DEFINE(FOX_1_6)
    ;;
  esac
fi
AC_SUBST(FOX_CFLAGS)
AC_SUBST(FOX_LIBS)


dnl Generated makefiles
AC_OUTPUT( \
  lexers/Makefile \
  lexlib/Makefile \
  src/Makefile \
  include/Makefile \
  fox/Makefile \
  fox/version.h \
  tests/Makefile \
  Makefile \
  fxscintilla.pc)
