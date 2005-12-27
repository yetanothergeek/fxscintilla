AC_DEFUN([CHECK_LIBFOX__],
dnl $1 = FOX_VERSION_STRING ("" => 1.0, "-1.1" => 1.1)
#
# Internal Function to check a given Fox version
#
[
#
# FLAGS backup
#
FOX_OLD_CPPFLAGS=$CPPFLAGS
FOX_OLD_LDFLAGS=$LDFLAGS
FOX_OLD_LIBS=$LIBS
AC_LANG_SAVE

#
# Temporary FLAGS setting
#
CPPFLAGS="$CPPFLAGS -I${FOX_INCLUDE_DIR}"
LDFLAGS="$LDFLAGS -L${FOX_LIB_DIR}"
LIBS=-lFOX$1
AC_LANG_CPLUSPLUS

if test x"$HAVE_CYGWIN" = xyes -o x"$HAVE_MINGW32" = xyes; then
  LDFLAGS="$LDFLAGS -mwindows"
	if test x"$HAVE_CYGWIN" = xyes; then
		CPPFLAGS="$CPPFLAGS -DWIN32"
	fi
fi

#
# Check Fox headers
#
AC_CHECK_HEADER(fox$1/fx.h, have_FOX=yes, have_FOX=no)

if test x"$have_FOX" = xyes; then
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
		CPPFLAGS="$PTHREAD_CFLAGS $CPPFLAGS"
	  AC_MSG_CHECKING(for FOX library with pthread)
	  AC_TRY_LINK_FUNC(fxfindfox, have_FOX=yes, have_FOX=no)
	  AC_MSG_RESULT($have_FOX)
	fi
fi

#
# End
#
CPPFLAGS=$FOX_OLD_CPPFLAGS
LDFLAGS=$FOX_OLD_LDFLAGS
LIBS=$FOX_OLD_LIBS
AC_LANG_RESTORE
AC_SUBST(FOX_INCLUDE_DIR)
AC_SUBST(FOX_LIB_DIR)
])


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
AC_ARG_WITH(fox-1-0, [  --with-fox-1-0          force Fox 1.0 usage],
        WITH_FOX_1_0=$withval,)
AC_ARG_WITH(fox-1-2, [  --with-fox-1-2          force Fox 1.2 usage],
        WITH_FOX_1_2=$withval,)
AC_ARG_WITH(fox-1-4, [  --with-fox-1-4          force Fox 1.4 usage],
        WITH_FOX_1_4=$withval,)
AC_ARG_WITH(fox-1-6, [  --with-fox-1-6          force Fox 1.6 usage],
        WITH_FOX_1_6=$withval,)
AC_ARG_WITH(foxinclude, [  --with-foxinclude=DIR   use Fox includes from DIR],
        FOX_INCLUDE_DIR=$withval, FOX_INCLUDE_DIR=/usr/local/include)
AC_ARG_WITH(foxlib,     [  --with-foxlib=DIR       use Fox libs from DIR],
        FOX_LIB_DIR=$withval, FOX_LIB_DIR=/usr/local/lib)

if test x"$WITH_FOX_1_6" = xyes; then
	CHECK_LIBFOX__(-1.6)
	if test x"$have_FOX" = xyes; then
		AC_DEFINE(HAVE_FOX_1_6,1,[Define FOX version 1.6.])
	fi
    have_FOX_1_6=$have_FOX
fi
AM_CONDITIONAL(HAVE_FOX_1_6, test x"$have_FOX" = xyes)

if test x"$WITH_FOX_1_2" != xno -a x"$WITH_FOX_1_0" != xyes -a x"$WITH_FOX_1_4" != xyes -a x"$WITH_FOX_1_6" != xyes; then
	CHECK_LIBFOX__(-1.2)
	if test x"$have_FOX" = xyes; then
		AC_DEFINE(HAVE_FOX_1_2,1,[Define FOX version 1.2.])
	fi
    have_FOX_1_2=$have_FOX
fi
AM_CONDITIONAL(HAVE_FOX_1_2, test x"$have_FOX_1_6" != xyes -a x"$have_FOX" = xyes)

if test x"$have_FOX" != xyes -a x"$WITH_FOX_1_4" != xno -a x"$WITH_FOX_1_0" != xyes -a x"$WITH_FOX_1_2" != xyes -a x"$WITH_FOX_1_6" != xyes; then
	CHECK_LIBFOX__(-1.4)
	if test x"$have_FOX" = xyes; then
		AC_DEFINE(HAVE_FOX_1_4,1,[Define FOX version 1.4.])
	fi
    have_FOX_1_4=$have_FOX
fi
AM_CONDITIONAL(HAVE_FOX_1_4, test x"$have_FOX_1_6" != xyes -a x"$have_FOX_1_2" != xyes -a x"$have_FOX" = xyes)

if test x"$have_FOX" != xyes -a x"$WITH_FOX_1_0" != xno -a x"$WITH_FOX_1_2" != xyes -a x"$WITH_FOX_1_4" != xyes -a x"$WITH_FOX_1_6" != xyes; then
	CHECK_LIBFOX__()
	if test x"$have_FOX" = xyes; then
		AC_DEFINE(HAVE_FOX_1_0,1,[Define FOX version 1.0.])
	fi
fi
AM_CONDITIONAL(HAVE_FOX_1_0, test x"$have_FOX_1_6" != xyes -a x"$have_FOX_1_2" != xyes -a x"$have_FOX_1_4" != xyes -a x"$have_FOX" = xyes)

#
# Abort if no FOX lib or multiple --with-fox-1.x
#
count_WITH_FOX=$((0))
if test x"$WITH_FOX_1_0" == xyes; then
	count_WITH_FOX=$(($count_WITH_FOX+1))
fi
if test x"$WITH_FOX_1_2" == xyes; then
	count_WITH_FOX=$(($count_WITH_FOX+1))
fi
if test x"$WITH_FOX_1_4" == xyes; then
	count_WITH_FOX=$(($count_WITH_FOX+1))
fi
if test x"$WITH_FOX_1_6" == xyes; then
	count_WITH_FOX=$(($count_WITH_FOX+1))
fi
if test $count_WITH_FOX -gt 1; then
  AC_MSG_ERROR(Options --with-fox-1.x are mutually exclusive)
fi
if test x"$have_FOX" = xno; then
  AC_MSG_ERROR(FOX needed)
fi
])
